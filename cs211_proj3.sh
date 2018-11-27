#!/bin/bash
#SBATCH --job-name=proj3
#SBATCH --output=base.txt
#SBATCH -N 1
#SBATCH -t 00:59:00

mpirun -np 10 ./cs211_proj3_3 500
