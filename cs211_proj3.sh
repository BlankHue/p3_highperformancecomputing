#!/bin/bash
#SBATCH --job-name=proj3
#SBATCH --output=base.txt
#SBATCH -N 4
#SBATCH -t 00:59:00

mpirun -np 128 ./cs211_proj3_2 10000000000
