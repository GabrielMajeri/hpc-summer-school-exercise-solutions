# High-Performance Computing Summer School &mdash; Homework solutions

## Description

This repository contains my solutions to the homework exercises assigned during the [Măgurele Summer School for Computing in a Rapidly Evolving Society: Parallel Algorithms and Optimizations](https://indico.ictp.it/event/10842/).

The lecture materials, sample code and problem statements can be found [here](https://github.com/ICTP/SMR4077). They were developed locally and then ran/tested on the [Leonardo supercomputer](https://leonardo-supercomputer.cineca.eu/) of CINECA, to which we had access during the school.

## Setup

Running the code in this repository requires a working installation of an [MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface) implementation (I've developed/tested it using [OpenMPI](https://www.open-mpi.org/)). The easiest way to install the required dependencies is by using [conda](https://anaconda.org/):

```shell
conda create -n hpc
conda activate hpc
conda install python numpy mpi4py
```

## License

All of the code in this repository is available under a permissive [MIT license](LICENSE.txt).
