class Particle:
  def __init__(self, pars, temps, sim):
    self.exec = sim.executable
    self.dim = pars.GetDim()
    self.tempdim = temps.GetDim()
    self.pos = np.random.uniform(0.0, 1.0, self.dim)
    self.pars = np.copy(self.pos)
    self.vel = np.zeros(shape=[self.dim], dtype=np.float32)
    self.dens = np.zeros(shape=[self.tempdim], dtype=np.float32)
    self.best_pos = np.copy(self.pos)
    self.cost = np.finfo(np.float32).max
    self.best_cost = self.cost
    self.parinfo = pars
    self.tempinfo = temps
      
  def CalculateNextVelocity(self, w, c1, c2, global_best_pos):
    self.vel = w * self.vel + c1 * np.random.uniform(0.0, 1.0, self.dim) *
    (self.best_pos - self.pos) + c2 * np.random.uniform(0.0, 1.0, self.dim) *
    (global_best_pos - self.pos)
    self.vel = np.minimum(self.vel, np.repeat(0.1, self.dim))
    self.vel = np.maximum(self.vel, np.repeat(-0.1, self.dim))
      
  def CalculateNextPosition(self):
    self.pos = self.pos + self.vel

    self.pos = np.minimum(self.pos, np.repeat(1.0, self.dim))
    self.pos = np.maximum(self.pos, np.repeat(0.0, self.dim))

  def UpdateBestPosition(self):
    if self.cost < self.best_cost:
      self.best_cost = self.cost
      self.best_pos = self.pos

  def ConvertPosToPars(self):
    for index in range(len(self.parinfo.parameters)):
      parameter = self.parinfo.parameters[index]
      kind = parameter.kind
      if kind == 'discrete':
        self.pars[index] = Utility.ScaleDiscrete(self.pos[index],
                                                  parameter.start,
                                                  parameter.end)
      else:
        self.pars[index] = Utility.ScaleContinuous(self.pos[index],
                                                    parameter.start,
                                                    parameter.end)

  def Evaluate(self, it):
    directory = 'runs/it{}/run{}'.format(it, int(rank/self.tempdim))

    if rank % self.tempdim == 0:
      shutil.rmtree(directory, ignore_errors=True)
      Utility.GenerateRunFiles(directory, self.tempinfo.temperatures)
      self.ConvertPosToPars()
      Utility.ReplaceParameters(self, directory, self.tempinfo)

    comm.barrier()
    Utility.RunSimulation(self.tempinfo.temperatures, directory, self.exec,
                          self.tempdim)
    
    comm.barrier()
    if rank % self.tempdim == 0:
      self.cost = Utility.GetCost(self, directory, self.tempinfo)