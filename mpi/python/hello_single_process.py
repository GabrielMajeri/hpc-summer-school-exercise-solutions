import math
from typing import cast
from mpi4py import MPI


comm = MPI.COMM_WORLD
rank = comm.Get_rank()
assert comm.Get_size() == 1, (
    "This example should be run with exactly one node and one task "
)

print("Hello from rank", rank)

data = math.pi

response = cast(float, comm.sendrecv(data, 0))

print("I sent", data, "and I received back", response, "from myself")
