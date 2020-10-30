import numpy as np
from mpi4py import MPI
import os

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

from parameter import Parameters
from temperature import Temperatures
from simulation import Simulation
from particleswarm import ParticleSwarmParameters
from particle import Particle
from utility import Utility

class PSO:
  def __init__(self, numIt, nPop, filename):
    it = 0

    filename = os.getcwd() + '/' + filename
    
    # Read input file
    if rank == 0:
      self.parameters = Parameters(filename)
      self.temperatures = Temperatures(filename)
      self.simulation = Simulation(filename)
      self.psoparameters = ParticleSwarmParameters(filename)
    else:
      self.parameters = None
      self.temperatures = None
      self.simulation = None
      self.psoparameters = None
        
    self.parameters = comm.bcast(self.parameters, root=0)
    self.temperatures = comm.bcast(self.temperatures, root=0)
    self.simulation = comm.bcast(self.simulation, root=0)
    self.psoparameters = comm.bcast(self.psoparameters, root=0)

    # Initilize some variables
    number_of_temperatures = self.temperatures.get_dim()
    w = self.psoparameters.w
    c1 = self.psoparameters.c1
    c2 = self.psoparameters.c2
    
    if rank == 0:
      swarm = [Particle(self.parameters, self.temperatures, self.simulation) for i in range(nPop)]
    else:
      swarm = None
    
    if rank == 0:
      Utility.log_message('Scattering the initial swarm')
    particle = comm.scatter(swarm, root=0)
    
    if rank == 0:
      Utility.log_message('Initializing the costs for the swarm')
    particle.evaluate(it)
    particle.update_best_position()
    comm.barrier()
    
    if rank == 0:
      Utility.log_message('Gathering all the costs back to node 0')
    swarm = comm.gather(particle, root=0)
    
    if rank == 0:
      length = len(swarm)
      for i in range(length):
        if i % number_of_temperatures == 0:
          Utility.print_coordinates(it, swarm[i])
            
    if rank == 0:
      best_particle = Utility.get_best_particle(swarm, number_of_temperatures)
      Utility.log_message('Best global cost: {}, {}, {}, {}'.format(
        best_particle.cost,
        best_particle.pars,
        best_particle.pos,
        best_particle.dens))
    it += 1
    while it <= numIt:
      if rank == 0:
        Utility.log_message('Starting iteration {}'.format(it))
        for i in range(len(swarm)):
          if i % number_of_temperatures == 0:
            p = swarm[i]
            p.calculate_next_velocity(w, c1, c2, best_particle.pos)
            p.calculate_next_position()
            
      particle = comm.scatter(swarm, root=0)
      particle.evaluate(it)
      particle.update_best_position()
      swarm = comm.gather(particle, root=0)
    
      if rank == 0:
        length = len(swarm)
        for i in range(length):
          if i % number_of_temperatures == 0:
            Utility.print_coordinates(it, swarm[i])
  
      if rank == 0:
        best_p = Utility.get_best_particle(swarm, number_of_temperatures)
        if best_p.cost < best_particle.cost:
          best_particle = best_p
          Utility.log_message('Found better global cost: {}, {}, {}'
                              .format(best_particle.cost,
                                     best_particle.pos,
                                     best_particle.dens))
        else:
          Utility.log_message(
            'Old global best is still better! {}, {}, {}'
            .format(best_particle.cost, best_particle.pos,
                    best_particle.dens))
      it += 1