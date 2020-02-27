import numpy as np
from mpi4py import MPI

class Simulation:
  def __init__(self, inputfile):
    e = xml.etree.ElementTree.parse(inputfile).getroot()
    
    self.executable = e.find('simulation').find('executable').text