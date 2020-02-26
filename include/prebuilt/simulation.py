import numpy as np
from mpi4py import MPI
import errno
import shutil
import fileinput
import xml.etree.ElementTree
import glob

class Simulation:
  def __init__(self, inputfile):
    e = xml.etree.ElementTree.parse(inputfile).getroot()
    
    self.executable = e.find('simulation').find('executable').text