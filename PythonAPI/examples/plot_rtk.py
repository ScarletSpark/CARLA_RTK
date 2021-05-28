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

plt.plot(sim_timestamp, [abs(ele) for ele in baseline_error])
plt.ylim([0, 0.7])
# plt.set_xlabel('Baseline Length')
# plt.set_ylabel('diff')

# plt.hist(baseline_error, bins=18)

plt.show()
