import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import argparse
import os
from pathlib import Path

def make_plot(x_label, y_label, fontsize, plot_name, plot_dir, nw, standard_parallel_time, ff_time, sequential_time = None):
    plt.xticks(fontsize = fontsize)
    plt.yticks(fontsize = fontsize)
    plt.xlabel(x_label, fontsize=fontsize)
    plt.ylabel(y_label, fontsize=fontsize)
    if sequential_time is not None:
        plt.plot(nw, sequential_time, label="Sequential", linestyle="dashed", color="gray", linewidth=3)
        plt.plot(nw, standard_parallel_time, label="Standard Parallel", linestyle="dashdot", color="dimgray", linewidth=3)
        plt.plot(nw, ff_time, label="FastFlow", linestyle='dashed', color="darkgray", linewidth=3)
        plt.legend(fontsize = fontsize, bbox_to_anchor =(0.65, 1.26))
    else:
        plt.plot(nw, standard_parallel_time, label="Standard Parallel", linestyle="dashdot", color="dimgray", linewidth=3)
        plt.plot(nw, ff_time, label="FastFlow", linestyle='dashed', color="darkgray", linewidth=3)
        plt.legend(fontsize = fontsize, bbox_to_anchor =(0.65, 1.19))

    plot = os.path.join(plot_dir, plot_name)
    plt.savefig(plot, dpi=160, bbox_inches='tight')
    print(f"{plot} saved \n")
    plt.close()

def get_speedup(sequential_time, parallel_time):
    return np.array([sequential_time[i]/parallel_time[i] for i in range(len(parallel_time))])

def main(nw):
    parent_dir = Path(__file__).parents[1]
    results_dir = os.path.join(parent_dir, "results") 
    seq_path, std_par_path , ff_path = 0,0,0
    fontsize = 13

    for file in os.listdir(results_dir):
        if file.startswith("fixed_nw"):
            if file.endswith(f"seq.csv"):
                print(os.path.join(results_dir, file))
                seq_path = os.path.join(results_dir, file)
            elif file.endswith(f"std_par-{nw}.csv"):
                print(os.path.join(results_dir, file))
                std_par_path = os.path.join(results_dir, file)
            elif file.endswith(f"ff-{nw}.csv"):
                print(os.path.join(results_dir, file))
                ff_path = os.path.join(results_dir, file)

    if seq_path == '' or std_par_path == '' or ff_path == '':
        raise Exception(f"Does not exist enough precomputed results with Number of Workers = {nw}! \n ")

    df_seq = pd.read_csv(f"{seq_path}", sep='\t', names=["n","seq_time"])
    df_seq = df_seq.drop_duplicates(subset=['n'], keep='first')
    df_std_par = pd.read_csv(f"{std_par_path}", sep='\t', names=["n","std_par_time"])
    df_std_par = df_std_par.drop_duplicates(subset=['n'], keep='first')
    df_ff = pd.read_csv(f"{ff_path}", sep='\t', names=["n","ff_time"])
    df_ff = df_ff.drop_duplicates(subset=['n'], keep='first')
    df_par = pd.merge(df_std_par, df_ff, on ="n", how="outer")

    if df_par.isnull().values.any():
        raise Exception("Size between FastFlow and Standard Parallel is different!")
    
    df = pd.merge(df_seq, df_par, on ="n", how="outer")

    if df.isnull().values.any():
        raise Exception("Size between Sequential and Parallel results is different!")

    n = df["n"].to_numpy()
    sequential_time = df_seq["seq_time"].to_numpy()
    standard_parallel_time = df_par["std_par_time"].to_numpy()
    ff_time = df_par["ff_time"].to_numpy()
    
    plot_dir = os.path.join(results_dir, 'plots')
    plot_dir = os.path.join(plot_dir, 'fixed_nw')
    isExist = os.path.exists(plot_dir)
    if not isExist:
        os.makedirs(plot_dir)
    
    make_plot("Size", "Time (microseconds)", fontsize, f"fixed_nw_time-nw_{nw}.png", plot_dir, n, standard_parallel_time, ff_time, sequential_time)

    standard_parallel_speedup = get_speedup(sequential_time, standard_parallel_time)
    ff_speedup = get_speedup(sequential_time, ff_time)
    make_plot("Size", "Speedup", fontsize, f"fixed_nw_speedup-nw_{nw}.png", plot_dir, n, standard_parallel_speedup, ff_speedup)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Read the .csv files to get the plots.')
    parser.add_argument('-nw', help='Number of workers', default = 22)
    args = parser.parse_args()

    try:
        main(nw = args.nw)
    except Exception as e: 
                print(e)
