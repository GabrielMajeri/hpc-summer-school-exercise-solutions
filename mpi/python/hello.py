from mpi4py import MPI

world_size = MPI.COMM_WORLD.size

world_rank = MPI.COMM_WORLD.rank

print(f"Hello world, from rank {world_rank} out of {world_size} processes")
