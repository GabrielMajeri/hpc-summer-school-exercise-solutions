"""Estimates the value of pi by numerically computing the integral
    integral from 0 to 1 of 4/(1 + x^2) dx
with work distributed across multiple processes, using MPI.
"""

import numpy as np
from mpi4py import MPI

communicator = MPI.COMM_WORLD
rank = communicator.rank

if rank == 0:
    print("Beginning computation of pi")

# n_points = 1_000

n_points = 1_000_000

# Warning: requires a lot of RAM!
# n_points = 1_000_000_000

assert n_points >= 2, "Need at least two points for numerical integration"

start = 0
end = 1
length = end - start
dx = length / n_points

num_processes = communicator.size
assert n_points % num_processes == 0, (
    "Code only supports running with a number of processes which divides the total number of integration sample points / function evaluation points"
)

# Determine how many points each worker will handle
n_points_per_process = n_points // num_processes

integration_interval = np.linspace(
    start + dx * n_points_per_process * rank,
    start + dx * n_points_per_process * (rank + 1) - dx,
    num=n_points_per_process,
)
# Sanity check, to make sure that `dx` is respected
assert np.allclose(dx, np.diff(integration_interval))

midpoints = integration_interval + dx / 2

# Midpoint rule
f_x = 4 / (1 + midpoints**2)
partial_result = np.sum(dx * f_x)

result = communicator.reduce(partial_result, op=MPI.SUM, root=0)
if rank == 0:
    assert result is not None
    print("Estimated value of pi:", result)
    print("True(r) value:", np.pi)

    absolute_error = abs(np.pi - result)
    print("Absolute error:", absolute_error)
    relative_error = absolute_error / np.pi
    print("Relative error:", f"{relative_error * 100:.4g}%")
