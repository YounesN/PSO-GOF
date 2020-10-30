import numpy as np
from mpi4py import MPI
import os
import datetime
import fileinput
from pathlib import Path
import shutil

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
        with fileinput.FileInput(filename, inplace=True) as file:
            for line in file:
                print(line.replace(text_to_search, replacement_text), end='')
    
    @staticmethod
    def ReplaceParameters(particle, directory, tempinfo):
        temperatures = tempinfo.temperatures
        for temp in temperatures:
            for index in range(len(particle.pars)):
                parinfo = particle.parameter_info.parameters
                file = directory + '/T_' + temp.temperature + '/Liq/Parameters.par'
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
    def RunSimulation(temperatures, directory):
        loadmodule = 'module swap gnu7/7.3.0 intel/2019;'
        cd = 'cd ' + directory
        end_part = './GOMC_CPU_NPT in.conf > out.log 2>&1'
        temp = temperatures[int(rank%3)]
        folder = '/T_' + temp.temperature + '/Liq;'
        command = loadmodule + cd + folder + end_part
        os.system(command)
            
    @staticmethod
    def GetCost(particle, directory, tempinfo):
        temperatures = tempinfo.temperatures
        folders = []
        target_densities = []
        for temp in temperatures:
            folders.append('/T_' + temp.temperature + '/Liq/')
            target_densities.append(float(temp.expt_liq))
        densities = []
        for folder in folders:
            filename = directory + folder + 'Blk_PRODUCTION_BOX_0.dat'
            density = 0
            with open(filename, 'r') as file:
                lines = []
                numlines = 0
                for line in file:
                    lines.append(line)
                    numlines += 1
                if numlines < 10:
                    Utility.LogMessage('Error reading file from ' +
                                       directory + folder)
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
    def GetBestParticle(swarm):
        best_particle = swarm[0]
        length = len(swarm)
        for i in range(length):
            if i % 3 == 0:
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
    def GenerateFilesForEquilibrate(temperatures, parameters, system, charges):
        base_directory = os.getcwd()
        shutil.rmtree('Equilibrate', ignore_errors=True)
        for temp in temperatures.temperatures:
            directory = "Equilibrate/T_" + temp.temperature + "/Liq/"
            Utility.MakeDirectory(directory)
            Utility.CopyDirectory("BUILD/model/*", directory)
            Utility.CopyDirectory("BUILD/pack/*", directory)
            Utility.CopyDirectory("BUILD/pdb/*", directory)
            os.chmod(directory + 'packmol', 509)
                
            os.chdir(directory)
            Utility.ReplaceText("pack.inp",
                                system.molname_pattern,
                                system.molname)
            Utility.ReplaceText("pack.inp", temp.molnumber_liq_pattern, 
                                temp.molnumber_liq)
            Utility.ReplaceText("pack.inp", temp.boxsize_liq_pattern,
                                temp.boxsize_liq)
            Utility.ReplaceText("build.tcl", system.resname_pattern,
                                system.resname)
                
            pars = ['epsilon', 'sigma', 'n']
            for par in pars:
                parameter = parameters.get_parameter_by_name(par)
                Utility.ReplaceText("Parameters.par", parameter.pattern,
                                    parameter.reference)
                
            os.system('./packmol < pack.inp' + '>> build_error.log 2>&1')
            loadmodule = 'module load vmd;'
            os.system(loadmodule + 'vmd -dispdev text < build.tcl' + '>> build_error.log 2>&1')
                
            # return to base directory
            os.chdir(base_directory)
        
    @staticmethod
    def RunEquilibrate(temperatures):
        base_directory = os.getcwd()
        
        directories = []
        temps = []
        for temp in temperatures.temperatures:
            directory = "Equilibrate/T_" + temp.temperature + "/Liq/"
            directories.append(directory)
            temps.append(temp)
        
        if rank < len(directories):
            i = rank
            directory = directories[i]
            Utility.MakeDirectory(directory)

            # Copy executable and input file from sim directory
            Utility.CopyDirectory("BUILD/sim/GOMC_CPU_NPT", directory)
            os.chmod(directory + 'GOMC_CPU_NPT', 509)
            Utility.CopyDirectory("BUILD/sim/eq.conf", directory)
            
            # Go to the simulation directory
            os.chdir(directory)
            
            # Replace parameters with values
            Utility.ReplaceText('eq.conf', temps[i].pressure_pattern,
                                temps[i].pressure)
            Utility.ReplaceText('eq.conf', temps[i].temperature_pattern,
                                temps[i].temperature)
            Utility.ReplaceText('eq.conf', temps[i].eq_step_pattern,
                                temps[i].eq_step)
            Utility.ReplaceText('eq.conf', temps[i].boxsize_liq_pattern,
                                temps[i].boxsize_liq)
            
            # Run the equilibrium simulation
            loadmodule = 'module swap gnu7/7.3.0 intel/2019;'
            os.system(loadmodule + './GOMC_CPU_NPT eq.conf &> out.log')
            
            # Go back to base directory
            os.chdir(base_directory)

    @staticmethod
    def GenerateRunFiles(directory, temperatures):
        Utility.MakeDirectory(directory)
        Utility.CopyDirectory('Equilibrate/*', directory)
        os.system('echo ' + directory +
                  '/*/*/ | xargs -n 1 cp BUILD/sim/in.conf')
        os.system('echo ' + directory +
                  '/*/*/ | xargs -n 1 cp BUILD/model/Parameters.par')
        for temp in temperatures:
            folder = directory + '/T_' + temp.temperature + '/Liq/'
            Utility.ReplaceText(folder + 'in.conf', temp.run_step_pattern,
                                temp.run_step)
            Utility.ReplaceText(folder + 'in.conf',
                                temp.temperature_pattern, temp.temperature)
            Utility.ReplaceText(folder + 'in.conf', temp.pressure_pattern,
                                temp.pressure)
            Utility.ReplaceText(folder + 'in.conf', 
                                temp.boxsize_liq_pattern, temp.boxsize_liq)

