#!/usr/bin/env bash
#SBATCH --job-name=my_first_job
#SBATCH --time=00:00:30
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem=1GB
#SBATCH --partition=dcgp_usr_prod
#SBATCH --account=tra25_ictp_rom_0

hostname
echo 'Hello World'
sleep 5
