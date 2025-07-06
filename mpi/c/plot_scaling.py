import csv
import matplotlib.pyplot as plt


ns = []
compute_durations = []
reduce_durations= []
total_durations = []

with open("output.txt", "r") as fin:
    reader = csv.reader(fin, delimiter=' ')

    for row in reader:
        n = int(row[0])
        compute_duration, reduce_duration, total_duration = map(float, row[1:])

        ns.append(n)
        compute_durations.append(compute_duration)
        reduce_durations.append(reduce_duration)
        total_durations.append(total_duration)

ns, compute_durations, reduce_durations, total_durations = zip(*sorted(zip(ns, compute_durations, reduce_durations, total_durations)))

plt.plot(ns, compute_durations, label='Compute time')
plt.plot(ns, reduce_durations, label='Reduce time')
plt.plot(ns, total_durations, label='Total time')
plt.grid(True)
plt.legend()
plt.savefig("scale.png")
