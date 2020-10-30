import numpy as np
import xml.etree.ElementTree


class Simulation:
    def __init__(self, input_file):
        e = xml.etree.ElementTree.parse(input_file).getroot()

        self.executable = e.find('simulation').find('executable').text