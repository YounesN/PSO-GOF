import numpy as np
from mpi4py import MPI
import shutil
from utility import Utility

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()


class Particle:
    def __init__(self, pars, temps, sim):
        self.exec = sim.executable
        self.number_of_particles = pars.get_dim()
        self.number_of_temperatures = temps.get_dim()
        self.pos = np.random.uniform(0.0, 1.0, self.number_of_particles)
        self.pars = np.copy(self.pos)
        self.vel = np.zeros(shape=[self.number_of_particles], dtype=np.float32)
        self.dens = np.zeros(shape=[self.number_of_temperatures], dtype=np.float32)
        self.best_pos = np.copy(self.pos)
        self.cost = np.finfo(np.float32).max
        self.best_cost = self.cost
        self.parameter_info = pars
        self.temperature_info = temps

    def calculate_next_velocity(self, w, c1, c2, global_best_pos):
        self.vel = w * self.vel + c1 * np.random.uniform(0.0, 1.0, self.number_of_particles) * (self.best_pos - self.pos) + c2 * np.random.uniform(0.0, 1.0, self.number_of_particles) * (global_best_pos - self.pos)
        self.vel = np.minimum(self.vel, np.repeat(0.1, self.number_of_particles))
        self.vel = np.maximum(self.vel, np.repeat(-0.1, self.number_of_particles))
      
    def calculate_next_position(self):
        self.pos = self.pos + self.vel

        self.pos = np.minimum(self.pos, np.repeat(1.0, self.number_of_particles))
        self.pos = np.maximum(self.pos, np.repeat(0.0, self.number_of_particles))

    def update_best_position(self):
        if self.cost < self.best_cost:
            self.best_cost = self.cost
            self.best_pos = self.pos

    def convert_position_to_parameters(self):
        for index in range(len(self.number_of_particles)):
            parameter = self.parameter_info.parameters[index]
            kind = parameter.kind
            if kind == 'discrete':
                self.pars[index] = Utility.scale_discrete(self.pos[index], parameter.start, parameter.end)
            else:
                self.pars[index] = Utility.scale_continuous(self.pos[index], parameter.start, parameter.end)

    def evaluate(self, it):
        directory = 'runs/it{}/run{}'.format(it, int(rank / self.number_of_temperatures))

        if rank % self.number_of_temperatures == 0:
            shutil.rmtree(directory, ignore_errors=True)
            Utility.generate_run_files(directory, self.temperature_info.temperatures)
            self.convert_position_to_parameters()
            Utility.replace_parameters(self, directory)

        comm.barrier()
        Utility.run_simulation(self.temperature_info.temperatures, directory, self.exec, self.number_of_temperatures)

        comm.barrier()
        if rank % self.number_of_temperatures == 0:
            self.cost = Utility.get_cost(self, directory, self.temperature_info)
