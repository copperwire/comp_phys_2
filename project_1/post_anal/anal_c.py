import matplotlib.pyplot as plt
import pandas as pd
import sys
import os
import re
import seaborn as sns
from numpy import log2, zeros, mean, var, sum, loadtxt, arange, array, cumsum, dot, transpose, diagonal, floor
from numpy.linalg import inv
from time import time
import numpy as np



def plotter_func(A, N_d, N_p, N_mc, alphas):
    #A is dataframe  with columns of 
    return


filenames = np.array(os.listdir("../data/temp_data"))

#With error plot
alpha_regex = r".+_a_(0.\d{6}).+"
alpha_regex_obj = re.compile(alpha_regex)

header_regex =r"# N_p:(\s\d+)\| N_d:(\s\d+)\| N_mc:(\s\d+)"
header_regex_obj = re.compile(header_regex)

mask = [not f.endswith("data.csv") for f in filenames]
filenames = sorted(filenames[mask], key = lambda x: float(alpha_regex_obj.search(x).group(1)))
stds = [0, 0]

N_ps = (1, 10, 100, 500)
N_mc = 0
N_ds = (1, 2, 3)
sim_types = [v for v in sys.argv[1:]]

for N_d_i in N_ds:
    for N_p_i in N_ps:
        mean_n_times = pd.read_csv("../data/temp_data/"+
                sim_types[0]+
                "_"+sim_types[1][3:]+
                "_meta"+
                "_np_"+str(N_p_i)+
                "_nd_"+str(N_d_i)+
                "_data.csv")
        
        for i in range(len(sim_types)):
            sim_type = sim_types[i]
            tmp = []
            for filename in filenames: 
                if filename.startswith(sim_type):
                    t_filename = "../data/temp_data/"+filename
                    
                    f_o = open(t_filename)
                    header = f_o.readline()
                    f_o.close()

                    match_o = header_regex_obj.search(header)
                    N_p = match_o.group(1)
                    N_d = match_o.group(2)
                    N_mc = match_o.group(3)

                    A = loadtxt(t_filename, delimiter = "\n", skiprows = 1)
                    plotter_func(A, N_p, N_d, N_mc, mean_n_times.alpha )