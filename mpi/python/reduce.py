from mpi4py import MPI

communicator = MPI.COMM_WORLD

# Determine the running process' rank
rank = communicator.rank

print(f"Process #{rank}, computing my part of the sequence")

value = (rank + 1)**2

result = communicator.reduce(value, MPI.SUM, root=0)

# Back at the root process, display the result of the computation
if rank == 0:
    print("Sum of squares is:")
    sequence = range(1, communicator.size + 1)
    terms = map(str, sequence)
    sum = " + ".join(terms)
    print(sum, "=", result)
