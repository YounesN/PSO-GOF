import numpy as np
from mpi4py import MPI
import errno
import shutil
import fileinput
import os
import sys
import datetime
import re
import xml.etree.ElementTree
import glob

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

sys.path.append("./include/prebuilt/")
import parameter
import temperature
import particleswarm
import utility
import particle




class PSO:
  def __init__(self, numIt, nPop, filename):
    it = 0
    
    # Read input file
    if rank == 0:
      self.parameters = Parameters(filename)
      self.temperatures = Temperatures(filename)
      self.system = System(filename)
      self.psoparameters = ParticleSwarmParameters(filename)
    else:
      self.parameters = None
      self.temperatures = None
      self.system = None
      self.psoparameters = None
        
    self.parameters = comm.bcast(self.parameters, root=0)
    self.temperatures = comm.bcast(self.temperatures, root=0)
    self.system = comm.bcast(self.system, root=0)
    self.psoparameters = comm.bcast(self.psoparameters, root=0)

    # Equilibrate all simulations
    if rank == 0:
      Utility.LogMessage('Generate files for equilibrium')
      Utility.GenerateFilesForEquilibrate(self.temperatures, 
                                          self.parameters, self.system)
      Utility.LogMessage('Done generating equilibrium files')
      Utility.LogMessage('Running equilibrium simulations')
    comm.barrier()
    Utility.RunEquilibrate(self.temperatures)
    comm.barrier()
    if rank == 0:
      Utility.LogMessage('Done equilibrating simulations')

    # Initilize some variables
    dim = self.parameters.GetDim()
    w = self.psoparameters.w
    c1 = self.psoparameters.c1
    c2 = self.psoparameters.c2
    
    if rank == 0:
      swarm = [Particle(self.parameters, self.temperatures) for i in range(nPop)]
    else:
      swarm = None
    
    if rank == 0:
      Utility.LogMessage('Scattering the initial swarm')
    particle = comm.scatter(swarm, root=0)
    
    if rank == 0:
      Utility.LogMessage('Initializing the costs for the swarm')
    particle.Evaluate(it)
    particle.UpdateBestPosition()
    comm.barrier()
    
    if rank == 0:
      Utility.LogMessage('Gathering all the costs back to node 0')
    swarm = comm.gather(particle, root=0)
    
    if rank == 0:
      length = len(swarm)
      for i in range(length):
        if i % 3 == 0:
          p = swarm[i]
          Utility.PrintCoordinates(it, p)
            
    if rank == 0:
      best_particle = Utility.GetBestParticle(swarm)
      Utility.LogMessage('Best global cost: {}, {}, {}, {}'.format(
        best_particle.cost,
        best_particle.pars,
        best_particle.pos,
        best_particle.dens))
    it += 1
    while it <= numIt:
      if rank == 0:
        Utility.LogMessage('Starting iteration {}'.format(it))
        for i in range(len(swarm)):
          if i % 3 == 0:
            p = swarm[i]
            p.CalculateNextVelocity(w, c1, c2, best_particle.pos)
            p.CalculateNextPosition()
            
      particle = comm.scatter(swarm, root=0)
      particle.Evaluate(it)
      particle.UpdateBestPosition()
      swarm = comm.gather(particle, root=0)
    
      if rank == 0:
        length = len(swarm)
        for i in range(length):
          if i % 3 == 0:
            p = swarm[i]
            Utility.PrintCoordinates(it, p)
  
      if rank == 0:
        best_p = Utility.GetBestParticle(swarm)
        if best_p.cost < best_particle.cost:
          best_particle = best_p
          Utility.LogMessage('Found better global cost: {}, {}, {}'
                             .format(best_particle.cost,
                                     best_particle.pos,
                                     best_particle.dens))
        else:
          Utility.LogMessage(
            'Old global best is still better! {}, {}, {}'
            .format(best_particle.cost, best_particle.pos,
                    best_particle.dens))
      it += 1

pso = PSO(30, 72, 'par.xml')
