import numpy as np
import matplotlib.pyplot as plt
filename = "./Output/Outputs/r3output.txt"
resultCompressionRate = 10;

# import os
# print(os.getcwd())

def read_integers_from_file(filename):
    with open(filename, 'r') as file:
        data = file.read()
    
    integer_list = [int(x) for x in data.strip().split(',')]

    return integer_list

def partitionArray(arr, x):
    new_counters = []
    counter = 0
    current_sum = 0
    n = len(arr)
    for i in range(n):
        if counter % x == 0 and counter != 0:
            new_counters.append(current_sum)
            current_sum = 0
        current_sum += arr[i]
        counter += 1

    return new_counters

counters = read_integers_from_file(filename)
new_counters = partitionArray(counters, resultCompressionRate)
time_steps = [x for x in range(len(new_counters))]

fig, ax = plt.subplots(figsize=(12, 8))
ax.plot(time_steps, new_counters, label='Molecules Reaching Receiver', linewidth=1.5)
ax.set_xlabel('Time (s)', fontsize=12)
ax.set_ylabel('Count of Molecules', fontsize=12)
ax.set_title('Molecules Reaching Receiver Over Time', fontsize=14)
ax.grid(True, linestyle='--', alpha=0.7)
ax.legend(fontsize=12)
plt.show()