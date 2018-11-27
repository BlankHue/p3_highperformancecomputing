#!/bin/bash
#SBATCH --job-name=proj3
#SBATCH --output=base.txt
#SBATCH -N 2
#SBATCH -t 00:59:00

mpirun -np 64 ./cs211_proj3_3 10000000000
