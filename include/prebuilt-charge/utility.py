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
    def make_directory(directory):
        os.system('mkdir -p ' + directory)

    @staticmethod
    def copy_directory(source, destination):
        command = 'cp -r ' + source + ' ' + destination
        os.system(command)

    @staticmethod
    def replace_text(filename, text_to_search, replacement_text):
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
    def find_last_charge_and_format_charges(charges):
        sum_of_charges = 0.0
        for idx in range(len(charges)):
            # Round the charge to 6th decimal place
            charges[idx] = round(charges[idx], 6)
            # Add the charge to sum variable
            sum_of_charges = sum_of_charges + charges[idx]

        # Calculate the value of the last charge
        last_charge = -1 * sum_of_charges
        # Round the last charge
        last_charge = round(last_charge, 6)
        # Add the last charge to the list
        charges.append(last_charge)

        # Make a string list and format all charges
        for idx in range(len(charges)):
            charges[idx] = Utility.format_charge_string(charges[idx])
        return charges

    @staticmethod
    def replace_parameters(particle, directory):
        temperatures = particle.temperature_info.temperatures
        for temperature in temperatures:
            all_parameters = particle.parameter_info.parameters

            # Store information for charge parameters
            variable_charge_parameters = []
            charge_parameters_value = []
            # Last parameter should be fixed, so keep it separate
            fixed_charge_parameter = []

            # Replace non-charge parameters and set aside the charges
            for parameter_index in range(len(all_parameters)):
                # Parameter is a charge parameters
                if all_parameters[parameter_index].is_charge == 'true':
                    # Parameter is a variable
                    if all_parameters[parameter_index].is_fixed == 'false':
                        variable_charge_parameters.append(all_parameters[parameter_index])
                        charge_parameters_value.append(particle.pars[parameter_index])
                    # Parameter is fixed and depend on other charged parameters
                    else:
                        fixed_charge_parameter.append(all_parameters[parameter_index])
                else:
                    file = directory + '/' + temperature.temperature + 'K/' + all_parameters[parameter_index].filename
                    parameter_value = particle.pars[parameter_index]
                    Utility.replace_text(file, all_parameters[parameter_index].pattern, str(parameter_value))

            # Replace temperature and pressure inside in.conf
            file = directory + '/' + temperature.temperature + 'K/in.conf'
            Utility.replace_text(file, temperature.temperature_pattern, temperature.temperature)
            Utility.replace_text(file, temperature.pressure_pattern, temperature.pressure)

            # Now process charge parameters
            variable_charge_parameters.extend(fixed_charge_parameter)
            charge_parameters_value = Utility.find_last_charge_and_format_charges(charge_parameters_value)
            for idx in range(len(variable_charge_parameters)):
                file = directory + '/' + temperature.temperature + 'K/' + variable_charge_parameters[idx].filename
                Utility.replace_text(file, variable_charge_parameters[idx].pattern, charge_parameters_value[idx])

    @staticmethod
    def scale_continuous(position, scale_min, scale_max):
        scale_pos = (scale_max - scale_min) * position
        scale_pos += scale_min
        return scale_pos

    @staticmethod
    def scale_discrete(position, scale_min, scale_max):
        scale_pos = Utility.scale_continuous(position, scale_min, scale_max + 1)
        scale_pos = int(scale_pos)
        if scale_pos == scale_max + 1:
            scale_pos = scale_max
        return scale_pos

    @staticmethod
    def run_simulation(temperatures, directory, executable, number_of_temperatures):
        load_module = ''
        cd = 'cd ' + directory
        temp = temperatures[int(rank % number_of_temperatures)]
        folder = '/' + temp.temperature + 'K/;'
        end_part = executable + ' in.conf > out.log 2>&1'
        command = load_module + cd + folder + end_part
        ret = os.system(command)
        if ret != 0:
            Utility.log_message('Simulation ' + directory + ' returned with ' + str(ret) + ' return code!')

    @staticmethod
    def get_cost(particle, directory, temperature_info):
        temperatures = temperature_info.temperatures
        folders = []
        target_densities = []
        density = 0
        for temp in temperatures:
            folders.append('/' + temp.temperature + 'K/')
            target_densities.append(float(temp.expt_dens))

        densities = []
        for folder in folders:
            filename = directory + folder + 'Blk_C2OH_BOX_0.dat'
            my_file = Path(filename)
            if not my_file.is_file(): # simulation failed for some reason
                Utility.log_message('Error reading file ' + filename)
                densities.append(9999)   # return 9999 as the density
                continue                 # continue to the next folder

            with open(filename, 'r') as file:
                lines = []
                number_of_lines = 0
                for line in file:
                    lines.append(line)
                    number_of_lines += 1

                if number_of_lines < 10:
                    Utility.log_message('File exists but does not have enough data: ' + filename)
                    density = 9999
                else:
                    start_line = int(number_of_lines * 0.8)
                    length = number_of_lines - start_line
                    for i in range(length):
                        index = i + start_line
                        line = lines[index]
                        line = re.sub(' +', ' ', line).strip()
                        columns = line.split(' ')
                        density += float(columns[10])

                    density = density / length

                densities.append(density)

        np.copyto(particle.dens, densities)
        return Utility.cost_function(target_densities, densities, temperatures)

    @staticmethod
    def cost_function(target_densities, densities, temperatures):
        liquid_coefficient = 0.91
        slope_coefficient = 0.09
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

        final = np.sum(errors) * liquid_coefficient + sum_of_slops * slope_coefficient
        return final

    @staticmethod
    def get_best_particle(swarm, num_of_temps):
        best_particle = swarm[0]
        length = len(swarm)
        for i in range(length):
            if i % num_of_temps == 0:
                particle = swarm[i]
                if particle.cost < best_particle.cost:
                    best_particle = particle
        return best_particle

    @staticmethod
    def log_message(message):
        if rank == 0:
            with open('log.txt', 'a') as file:
                out = str(datetime.datetime.now()) + ' - '
                out += str(rank) + ' - '
                out += message + '\n'
                file.write(out)
                file.flush()

    @staticmethod
    def print_coordinates(it, p):
        with open('data.csv', 'a') as file:
            file.write('{},{},{},{},{},{},{}\n'.format(it, p.pos, p.pars, p.dens, p.vel, p.best_pos, p.cost))
            file.flush()

    @staticmethod
    def generate_run_files(directory, temperatures):
        Utility.make_directory(directory)
        # Let's not copy the executable and just run one from one specified in PATH
        for temp in temperatures:
            Utility.copy_directory('PREBUILT/', directory + '/' + temp.temperature + 'K')
