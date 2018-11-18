#!/bin/bash
#SBATCH --job-name=proj3
#SBATCH --output=res.txt
#SBATCH -N 1
#SBATCH -n 32
#SBATCH -t 00:59:00

module purge
module load openmpi-3.0/gcc-4.8.5 
mpirun ./cs211_proj3_1 100
