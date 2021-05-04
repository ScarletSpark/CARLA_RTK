import numpy as np
import matplotlib.pyplot as plt
import math
import statistics
from scipy import stats

lines = []

with open('plot_data.txt') as f:
    lines = f.readlines()

parsed_lines = []

for line in lines:
    parsed_lines.append(line.split('\t'))

deg = []
vec_x = []
vec_y = []
vec_z = []

for line in parsed_lines:
    deg.append(float(line[8]))
    vec_x.append(float(line[9]))
    vec_y.append(float(line[10]))
    vec_z.append(float(line[11]))
    # print(line)

v_lenth = []
v_angle = []

# print(vec_x)
for i in range( len(vec_x) ):
    v_lenth.append( math.sqrt( pow(vec_x[i],2) + pow(vec_y[i],2) + pow(vec_z[i],2) ) )

    v_angle.append(360 - (math.degrees(math.atan2(vec_x[i], vec_y[i])) - 90)%360)

len_m = statistics.mean(v_lenth)
len_std = statistics.stdev(v_lenth)

# expected = stats.norm.ppf(0.95, loc = len_m)
x_exp = np.linspace(min(v_lenth), max(v_lenth), 3500)
x_deg = np.linspace(min(deg), max(deg), 3500)
# rv = stats.norm.pdf(x_exp, loc=len_m)
rv = np.random.normal(len_m, 0.0022, 3500)
# print(rv)


len_chi_2, len_p_2 = stats.chisquare(v_lenth, rv)
len_chi, len_p = stats.normaltest(v_lenth) 
print("len_chi = {}, len_p = {}".format(len_chi, len_p))


ang_m = statistics.mean(deg)
ang_std = statistics.stdev(deg)
# ang_chi = stats.chisquare(deg)
ang_chi, ang_p = stats.normaltest(deg) 
print("ang_chi = {}, ang_p = {}".format(ang_chi, ang_p))

"""Chi-squared test for normal destribution"""

fr_num = 16

bins = [-float('inf')]
for i in range(fr_num - 1):
    p = (i+1)/fr_num; z = stats.norm.ppf(p); bound = len_m + z*len_std
    print(p, '{:6.4f}'.format(z), '{:6.4f}'.format(bound))
    bins.append(bound)
bins.append(float('inf'))

frequency = []
for i in range(fr_num):
    observed, expected = sum(num >= bins[i] and num < bins[i+1] for num in v_lenth), (3500/( fr_num ))
    print('{:2d}'.format(observed), expected)
    frequency.append((observed, expected))

chi_square = sum([(x[0]-x[1])**2./x[1] for x in frequency])

crit = stats.chi2.ppf(0.95, (fr_num) - 3)

p_value = 1 - stats.chi2.cdf(chi_square, (fr_num) - 3)

print("\nCustom Chi2")
print("Chi = {}\tCriteria = {}\tP-value = {}\n".format(chi_square, crit, p_value))
#--------------------------------------------#
print("Normaltest K**2")
print("K = {}\tP-value = {}\n".format(len_chi, len_p))
print("Chisquered SciPy")
print("Chi = {}\tCriteria = {}\tP-value = {}\n".format(len_chi_2, crit, len_p_2))



f,axs = plt.subplots(1,2)

axs[0].hist(v_lenth, bins=(fr_num))
# axs[0].plot(x_exp, stats.norm.pdf(x_exp, len_m, len_std))
# axs[0].plot(x_exp, rv)
axs[0].set_xlabel('Values, m')
axs[0].set_ylabel('Density')
axs[0].set_title('Base vector lenth')
# axs[0].plot(, rv*1000)
# axs[0].legend("m = {}, STD = {}".format(len_m, len_std))
# axs[0].axis([2.075, 2.095, 0, 100])
axs[0].grid()

axs[1].hist(deg, bins=35)
# axs[1].plot(x_deg, stats.norm.pdf(x_deg, ang_m, ang_std))
axs[1].set_xlabel('Values, °')
axs[1].set_ylabel('Density')
axs[1].set_title('Azimuth')
axs[1].grid()

plt.figtext(0.125,0.97, "\nMean = {: .4f}\nSTD = {: .4f}".format(len_m, len_std),
            horizontalalignment ="left",  
            verticalalignment ="center",  
            wrap = True, 
            in_layout = True,
            # fontsize = 10,
            # color ="blue"
            )

plt.figtext(0.48,0.97, "\nP-value = {: .4f}\n".format(len_p),
            horizontalalignment ="right",  
            verticalalignment ="center",  
            wrap = True, 
            in_layout = True,
            # fontsize = 10,
            # color ="blue"
            )

plt.figtext(0.9,0.97, "\nP-value = {: .4f}\n".format(ang_p),
            horizontalalignment ="right",  
            verticalalignment ="center",  
            wrap = True, 
            in_layout = True,
            # fontsize = 10,
            # color ="blue"
            )

plt.figtext(0.55,0.97, "\nMean = {: .4f}\nSTD = {: .4f}".format(ang_m, ang_std),
            horizontalalignment ="left",  
            verticalalignment ="center",  
            wrap = True, 
            in_layout = True,
            # fontsize = 10,
            # color ="blue"
            )

# axs[2].hist(v_angle, bins=35)
# axs[2].set_xlabel('Values, °')
# axs[2].set_ylabel('Density')
# axs[2].set_title('Azimuth')
# axs[2].grid()

plt.show()
# plt.savefig("matplotlib.png")  #savefig, don't show



# plt.hist(x, bins=35)

# plt.xlabel('Values, m')
# plt.ylabel('Density')
# plt.title('Base vector lenth')
# plt.axis([2.075, 2.095, 0, 100])
# plt.grid()
# plt.show()
