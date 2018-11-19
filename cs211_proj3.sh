#!/bin/bash
#SBATCH --job-name=proj3
#SBATCH --output=res.txt
#SBATCH -N 1
#SBATCH -t 00:59:00

mpirun -np 32 ./cs211_proj3_1 10000000000
