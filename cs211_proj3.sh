#!/bin/bash
#SBATCH --job-name=proj3
#SBATCH --output=res.txt
#SBATCH -N 4
#SBATCH -t 00:59:00

mpirun -np 128 ./cs211_proj3_1 100000
