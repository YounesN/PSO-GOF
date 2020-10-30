import numpy as np
from mpi4py import MPI
import xml.etree.ElementTree


comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()


class Temperature:
    def __init__(self, pars):
        self.temperature = pars['temperature']
        self.temperature_pattern = pars['temperature_pattern']
        self.expt_dens = pars['expt_dens']
        self.pressure = pars['pressure']
        self.pressure_pattern = pars['pressure_pattern']


class Temperatures:
    def __init__(self, input_file):
        self.temperatures = []

        e = xml.etree.ElementTree.parse(input_file).getroot()
        for temp in e.find('data').findall('temperature'):
            pars = {'temperature': temp.find('temp').text,
                    'temperature_pattern': temp.find('temp').get('pattern'),
                    'expt_dens': temp.find('expt_dens').text,
                    'pressure': temp.find('pressure').text,
                    'pressure_pattern': temp.find('pressure').get('pattern')}

            self.temperatures.append(Temperature(pars))

    def get_dim(self):
        return len(self.temperatures)
