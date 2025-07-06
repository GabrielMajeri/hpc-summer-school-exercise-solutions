from time import perf_counter_ns

from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

SIZE = 256 * 1024 * 1024

if rank == 0:
    data = np.arange(SIZE, dtype=np.int32)

    send_start = perf_counter_ns()
    comm.Send(data, dest=1)
    send_end = perf_counter_ns()

    send_duration = send_end - send_start
    print("Sending took", send_duration / 1_000_000, "milliseconds")
elif rank == 1:
    data = np.empty(SIZE, dtype=np.int32)

    recv_start = perf_counter_ns()
    comm.Recv(data, source=0)
    recv_end = perf_counter_ns()

    recv_duration = recv_end - recv_start
    print("Receiving took", recv_duration / 1_000_000, "milliseconds")
