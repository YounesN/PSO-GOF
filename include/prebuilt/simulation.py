import numpy as np


class Simulation:
  def __init__(self, inputfile):
    e = xml.etree.ElementTree.parse(inputfile).getroot()
    
    self.executable = e.find('simulation').find('executable').text