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
        self.molnumber_liq = pars['molnumber_liq']
        self.molnumber_liq_pattern = pars['molnumber_liq_pattern']
        self.boxsize_liq = pars['boxsize_liq']
        self.boxsize_liq_pattern = pars['boxsize_liq_pattern']
        self.eq_step = pars['eq_step']
        self.eq_step_pattern = pars['eq_step_pattern']
        self.run_step = pars['run_step']
        self.run_step_pattern = pars['run_step_pattern']
        self.pressure = pars['pressure']
        self.pressure_pattern = pars['pressure_pattern']
        self.expt_liq = pars['expt_liq']

class Temperatures:
    def __init__(self, inputfile):
        self.temperatures = []
        
        e = xml.etree.ElementTree.parse(inputfile).getroot()
        for temp in e.find('data').findall('temperature'):
            pars = {}
            pars['temperature'] = temp.find('temp').text
            pars['temperature_pattern'] = temp.find('temp').get('pattern')
            pars['molnumber_liq'] = temp.find('molnumber_liq').text
            pars['molnumber_liq_pattern'] = temp.find('molnumber_liq').get('pattern')
            pars['boxsize_liq'] = temp.find('boxsize_liq').text
            pars['boxsize_liq_pattern'] = temp.find('boxsize_liq').get('pattern')
            pars['eq_step'] = temp.find('eq_step').text
            pars['eq_step_pattern'] = temp.find('eq_step').get('pattern')
            pars['run_step'] = temp.find('run_step').text
            pars['run_step_pattern'] = temp.find('run_step').get('pattern')
            pars['pressure'] = temp.find('pressure').text
            pars['pressure_pattern'] = temp.find('pressure').get('pattern')
            pars['expt_liq'] = temp.find('expt_liq').text
                        
            self.temperatures.append(Temperature(pars))
    
    def GetDim(self):
        return len(self.temperatures)