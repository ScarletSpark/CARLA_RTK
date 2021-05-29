import numpy as np
import matplotlib.pyplot as plt
import math
import statistics
from scipy import stats

rtk_base_x = -29820
rtk_base_y = -36820
rtk_base_z =  3390

infilename = "log_rtk.txt"
outfilename = "new_data.txt"

lines_seen = set() # holds lines already seen
outfile = open(outfilename, "w")
for line in open(infilename, "r"):
    if line not in lines_seen: # not a duplicate
        outfile.write(line)
        lines_seen.add(line)
outfile.close()

lines = []

with open(outfilename) as f:
    lines = f.readlines()

parsed_lines = []

for line in lines:
    parsed_lines.append(line.split('\t'))

sim_timestamp = []
x = []
y = []
z = []
baseline_len = []
real_baseline = []
baseline_error = []

for line in parsed_lines:
    sim_timestamp.append(float(line[1]))
    x.append(float(line[2]))
    y.append(float(line[3]))
    z.append(float(line[4]))
    baseline_len.append(float(line[5]))
    real_baseline.append(float(line[6]))
    baseline_error.append(float(line[7]))
    # print(line)

# rtk_len = []

# for t in range(len(x)):
#     rtk_len.append( math.sqrt( pow( x[t] - rtk_base_x ,2) + pow( y[t] - rtk_base_y ,2) + pow( z[t] - rtk_base_z ,2) ) )

plt.plot(np.array(sim_timestamp)-sim_timestamp[0], [abs(ele) for ele in baseline_error], 'bo', scalex='True',label='Baseline error', markersize=0.2, linewidth=0.2)
plt.ylim([0.02, 0.05])
# plt.xlim([0, sim_timestamp[len(sim_timestamp)-1] - sim_timestamp[0]])
plt.grid()
plt.xlabel('Simulation time (sec)')
plt.ylabel('Baseline error (m)')

# plt.hist(baseline_error, bins=18)

plt.show()
