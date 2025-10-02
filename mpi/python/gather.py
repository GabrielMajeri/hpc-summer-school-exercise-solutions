"""This program has each process generate a random number,
then they are all gathered back at the root process and displayed.
"""

from typing import cast

import numpy as np
from mpi4py import MPI

# Use the global (world) communicator
communicator = MPI.COMM_WORLD

# Determine this process' rank
rank = communicator.rank

# Generate a different random number for each process
rng = np.random.default_rng(rank)
secret = rng.choice(range(32))

print(f"Process #{rank} chose the number {secret}")

data = communicator.gather(secret, root=0)

# Back at the central process, receive and print the generated numbers
if rank == 0:
    print("This is the root process (#0) speaking!")

    data = cast(list[int], data)
    data = [int(n) for n in data]
    print("The numbers are:", data)
    print("Their sum is:", sum(data))
