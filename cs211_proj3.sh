#!/bin/bash
#SBATCH --job-name=proj3
#SBATCH --output=base.txt
#SBATCH -N 9
#SBATCH -t 00:59:00

mpirun -np 286 ./cs211_proj3_2 10000000000
