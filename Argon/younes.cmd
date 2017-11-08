#!/bin/bash
#SBATCH --time=900:00:00
#SBATCH --ntasks=2   # number of processor cores (i.e. tasks)
#SBATCH --nodes=1   # number of nodes
#SBATCH --exclude=potoff[16,40,43]
echo Running on host `hostname`
echo Time is `date`

cd /home4/younes/Desktop/Optimization/Argon/
echo Directory is `pwd`

# Run job
./main