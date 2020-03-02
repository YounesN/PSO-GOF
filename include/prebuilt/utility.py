import numpy as np
from mpi4py import MPI
import os
import datetime
import fileinput

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

class Utility:
  @staticmethod
  def MakeDirectory(directory):
    os.system('mkdir -p ' + directory)
  
  @staticmethod
  def CopyDirectory(src, dest):
    os.system('cp -r ' + src + ' ' + dest)
  
  @staticmethod
  def ReplaceText(filename, text_to_search, replacement_text):
    filename = os.getcwd() + '/' + filename

    with fileinput.FileInput(filename, inplace=True) as file:
      for line in file:
        print(line.replace(text_to_search, replacement_text), end='')
  
  @staticmethod
  def ReplaceParameters(particle, directory, tempinfo):
    temperatures = tempinfo.temperatures
    for temp in temperatures:
      for index in range(len(particle.pars)):
        parinfo = particle.parinfo.parameters
        file = directory + '/' + temp.temperature + 'K/' + parinfo[index].filename
        val = particle.pars[index]
        Utility.ReplaceText(file, parinfo[index].pattern, str(val))
              
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
    loadmodule = 'module swap gnu7/7.3.0 intel/2019;'
    cd = 'cd ' + directory
    end_part = './' + executable + ' in.conf > out.log 2>&1'
    temp = temperatures[int(rank % number_of_temperatures)]
    folder = '/' + temp.temperature + 'K/;'
    command = loadmodule + cd + folder + end_part
    os.system(command)
          
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
      filename = directory + folder + 'Blk_SPCE_BOX_0.dat'
      density = 0
      with open(filename, 'r') as file:
        lines = []
        numlines = 0
        for line in file:
          lines.append(line)
          numlines += 1
        if numlines < 10:
          Utility.LogMessage('Error reading file ' + directory + folder)
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
    os.system('echo ./PREBUILT/RunFiles/*K/ | xargs -n 1 cp ./PREBUILT/*')
    Utility.CopyDirectory('PREBUILT/RunFiles/*', directory)