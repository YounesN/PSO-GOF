import numpy as np
from mpi4py import MPI
import os
import datetime
import fileinput
import re
from pathlib import Path

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

class Utility:
  @staticmethod
  def MakeDirectory(directory):
    os.system('mkdir -p ' + directory)
  
  @staticmethod
  def CopyDirectory(src, dest):
    command = 'cp -r ' + src + ' ' + dest
    os.system(command)
  
  @staticmethod
  def ReplaceText(filename, text_to_search, replacement_text):
    filename = os.getcwd() + '/' + filename

    with fileinput.FileInput(filename, inplace=True) as file:
      for line in file:
        print(line.replace(text_to_search, replacement_text), end='')
  
  @staticmethod
  def format_charge_string(s):
    if s < 0:
      string = str(s)
      while len(string) < 9:
        string += '0'
      return string
    else:
      string = str(s)
      while len(string) < 8:
        string += '0'
      return ' ' + string

  @staticmethod
  def format_charges(h, o):
    h = round(h, 6)
    o = round(o, 6)
    c = -1 * (h + o)
    c = round(c, 6)
    h = Utility.format_charge_string(h)
    o = Utility.format_charge_string(o)
    c = Utility.format_charge_string(c)
    return h, o, c
  
  @staticmethod
  def ReplaceParameters(particle, directory):
    temperatures = particle.tempinfo.temperatures
    for temp in temperatures:
      # replace parameters
      parinfo = particle.parinfo.parameters
      hcharge_val = 0
      ocharge_val = 0
      ccharge_val = 0
      for index in range(len(particle.pars)):
        if parinfo[index].name == 'hcharge':
          hcharge_val = particle.pars[index]
          continue
        if parinfo[index].name == 'ocharge':
          ocharge_val = particle.pars[index]
          continue
        file = directory + '/' + temp.temperature + 'K/' + parinfo[index].filename
        val = particle.pars[index]
        Utility.ReplaceText(file, parinfo[index].pattern, str(val))
      
      # replace temperature and pressure inside in.conf
      file = directory + '/' + temp.temperature + 'K/in.conf'
      Utility.ReplaceText(file, temp.temperature_pattern, temp.temperature)
      Utility.ReplaceText(file, temp.pressure_pattern, temp.pressure)

      # replace ccharge which should add all charges to zero
      hcharge_val, ocharge_val, ccharge_val = Utility.format_charges(hcharge_val, ocharge_val)
      file = directory + '/' + temp.temperature + 'K/C2OH_merged.psf'
      Utility.ReplaceText(file, 'HCHARGEEE', hcharge_val)
      Utility.ReplaceText(file, 'OCHARGEEE', ocharge_val)
      Utility.ReplaceText(file, 'CCHARGEEE', ccharge_val)
              
  @staticmethod
  def ScaleContinuous(position, scale_min, scale_max):
    scale_pos = (scale_max - scale_min) * position
    scale_pos += scale_min
    return scale_pos

  @staticmethod
  def ScaleDiscrete(position, scale_min, scale_max):
    scale_pos = Utility.ScaleContinuous(position, scale_min, scale_max + 1)
    scale_pos = int(scale_pos)
    if scale_pos == scale_max + 1:
      scale_pos = scale_max
    return scale_pos

  @staticmethod
  def RunSimulation(temperatures, directory, executable, number_of_temperatures):
    #loadmodule = 'module swap gnu7/7.3.0 intel/2018;'
    loadmodule = ''
    cd = 'cd ' + directory
    temp = temperatures[int(rank % number_of_temperatures)]
    folder = '/' + temp.temperature + 'K/;'
    end_part = executable + ' in.conf > out.log 2>&1'
    command = loadmodule + cd + folder + end_part
    ret = os.system(command)
    if ret != 0:
      Utility.LogMessage('Simulation ' + directory + ' returned with ' + str(ret) + ' return code!')
          
  @staticmethod
  def GetCost(particle, directory, tempinfo):
    temperatures = tempinfo.temperatures
    folders = []
    target_densities = []
    for temp in temperatures:
      folders.append('/' + temp.temperature + 'K/')
      target_densities.append(float(temp.expt_dens))
    densities = []
    for folder in folders:
      filename = directory + folder + 'Blk_C2OH_BOX_0.dat'
      density = 0
      my_file = Path(filename)
      if(not my_file.is_file()): # simulation failed for some reason
        Utility.LogMessage('Error reading file ' + filename)
        densities.append(9999)   # return 9999 as the density
        continue                 # continue to the next folder
      with open(filename, 'r') as file:
        lines = []
        numlines = 0
        for line in file:
          lines.append(line)
          numlines += 1
        if numlines < 10:
          Utility.LogMessage('File exists but doesn\'t have enough data: ' + filename)
          density = 9999
        else:
          start_line = int(numlines * 0.8)
          length = numlines - start_line
          for i in range(length):
            index = i + start_line
            line = lines[index]
            line = re.sub(' +', ' ', line).strip()
            columns = line.split(' ')
            density += float(columns[10])
          density = density / length
      densities.append(density)
    np.copyto(particle.dens, densities)
    return Utility.CostFunction(target_densities, densities, temperatures)
  
  @staticmethod
  def CostFunction(target_densities, densities, temperatures):
    liq_coeff = 0.91
    slope_coeff = 0.09
    errors = []
    temps = []
    for i in range(len(densities)):
      error = abs(densities[i] - target_densities[i]) / target_densities[i]
      errors.append(error)
      temps.append(float(temperatures[i].temperature))
    
    sum_of_slops = 0.0
    for i in range(len(errors)-1):
      slope = (errors[i+1]-errors[i])/(temps[i+1]-temps[i])
      sum_of_slops += slope

    final = np.sum(errors) * liq_coeff + sum_of_slops * slope_coeff
    return final
  
  @staticmethod
  def GetBestParticle(swarm, num_of_temps):
    best_particle = swarm[0]
    length = len(swarm)
    for i in range(length):
      if i % num_of_temps == 0:
        particle = swarm[i]
        if particle.cost < best_particle.cost:
          best_particle = particle
    return best_particle

  @staticmethod
  def LogMessage(message):
    if rank == 0:
      with open('log.txt', 'a') as file:
        out = str(datetime.datetime.now()) + ' - '
        out += str(rank) + ' - '
        out += message + '\n'
        file.write(out)
        file.flush()
  
  @staticmethod
  def PrintCoordinates(it, p):
    with open('data.csv', 'a') as file:
      file.write('{},{},{},{},{},{},{}\n'
                 .format(it, p.pos, p.pars, p.dens, p.vel, p.best_pos, p.cost))
      file.flush()
      
  @staticmethod
  def GenerateRunFiles(directory, temperatures):
    Utility.MakeDirectory(directory)
    # Let's not copy the executable and just run one from one specified in PATH
    for temp in temperatures:
      Utility.CopyDirectory('PREBUILT/', directory + '/' + temp.temperature + 'K')
