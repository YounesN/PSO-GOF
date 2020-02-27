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
from pso import PSO

pso = PSO(30, 80, 'par.xml')
