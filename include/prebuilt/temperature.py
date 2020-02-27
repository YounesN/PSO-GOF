import numpy as np
from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

class Temperature:
  def __init__(self, pars):
    self.temperature = pars['temperature']
    self.expt_dens = pars['expt_dens']

class Temperatures:
  def __init__(self, inputfile):
    self.temperatures = []
    
    e = xml.etree.ElementTree.parse(inputfile).getroot()
    for temp in e.find('data').findall('temperature'):
      pars = {}
      pars['temperature'] = temp.find('temp').text
      pars['expt_dens'] = temp.find('expt_dens').text
                  
      self.temperatures.append(Temperature(pars))
  
  def GetDim(self):
      return len(self.temperatures)