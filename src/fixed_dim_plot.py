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

def make_expected_plot(x_label, y_label, fontsize, plot_name, plot_dir, nw, parallel_time, expected_time, parallel_label, expected_label):
    plt.xticks(fontsize = fontsize)
    plt.yticks(fontsize = fontsize)
    plt.xlabel(x_label, fontsize=fontsize)
    plt.ylabel(y_label, fontsize=fontsize)

    plt.plot(nw, parallel_time, label= parallel_label, linestyle="dashdot", color="dimgray", linewidth=3)
    plt.plot(nw, expected_time, label=expected_label, linestyle='dashed', color="darkgray", linewidth=3)
    plt.legend(fontsize = fontsize, bbox_to_anchor =(0.65, 1.19))

    plot = os.path.join(plot_dir, plot_name)
    plt.savefig(plot, dpi=160, bbox_inches='tight')
    print(f"{plot} saved \n")
    plt.close()

def get_speedup(sequential_time, parallel_time):
    return np.array([sequential_time[i]/parallel_time[i] for i in range(len(parallel_time))])

def get_scalability(parallel_time):
    return np.array([parallel_time[0]/parallel_time[i] for i in range(len(parallel_time))])

def get_efficiency(speedups, nw):
    return np.array([speedups[i]/nw[i] for i in range(len(speedups))])

def main(size):
    parent_dir = Path(__file__).parents[1]
    results_dir = os.path.join(parent_dir, "results") 
    seq_path, std_par_path , ff_path, std_overhead_path, ff_overhead_path = 0,0,0,0,0
    fontsize = 13

    for n in size:
        for file in os.listdir(results_dir):
            if file.startswith("fixed_dim"):
                if file.endswith(f"seq-{n}.csv"):
                    print(os.path.join(results_dir, file))
                    seq_path = os.path.join(results_dir, file)
                elif file.endswith(f"std_par-{n}.csv"):
                    print(os.path.join(results_dir, file))
                    std_par_path = os.path.join(results_dir, file)
                elif file.endswith(f"ff-{n}.csv"):
                    print(os.path.join(results_dir, file))
                    ff_path = os.path.join(results_dir, file)
            if file.startswith("standard_overhead"):
                if file.endswith(f"-{n}.csv"):
                    std_overhead_path = os.path.join(results_dir, file)
            if file.startswith("ff_overhead"):
                if file.endswith(f"-{n}.csv"):
                    ff_overhead_path = os.path.join(results_dir, file)
            

        if seq_path == '' or std_par_path == '' or ff_path == '':
            raise Exception(f"Does not exist enough precomputed results with N = {n}! \n ")
        
        if std_overhead_path == '' or ff_overhead_path == '':
            raise Exception(f"Please run the overhead script! \n ")

        df_seq = pd.read_csv(f"{seq_path}", sep='\t', names=["seq_time"])
        df_seq = df_seq.head(1)
        df_std_par = pd.read_csv(f"{std_par_path}", sep='\t', names=["nw","std_par_time"])
        df_std_par = df_std_par.drop_duplicates(subset=['nw'], keep='first')
        df_ff = pd.read_csv(f"{ff_path}", sep='\t', names=["nw","ff_time"])
        df_ff = df_ff.drop_duplicates(subset=['nw'], keep='first')
        df_par = pd.merge(df_std_par, df_ff, on ="nw", how="outer")

        if df_par.isnull().values.any():
            raise Exception("Number of workers between Standard Parallel and FastFlow is different!")
        
        df_std_over = pd.read_csv(f"{std_overhead_path}", sep='\t', names=["std_over_time"])
        df_ff_over = pd.read_csv(f"{ff_overhead_path}", sep='\t', names=["ff_over_time"])

        nw = df_par["nw"].to_numpy()

        sequential_time = df_seq["seq_time"].to_numpy()
        seq_time = sequential_time[0]
        for _ in range(len(nw)-1):
            sequential_time = np.append(sequential_time, seq_time)
        
        standard_parallel_time = df_par["std_par_time"].to_numpy()
        ff_time = df_par["ff_time"].to_numpy()

        std_over_time = df_std_over["std_over_time"].to_numpy()
        ff_over_time = df_ff_over["ff_over_time"].to_numpy()

        expected_std_time = np.array([((sequential_time[i]/nw[i]) + std_over_time[i]) for i in range(len(nw))])
        expected_ff_time = np.array([((sequential_time[i]/nw[i]) + ff_over_time[i]) for i in range(len(nw))])
        
        plot_dir = os.path.join(results_dir, 'plots')
        plot_dir = os.path.join(plot_dir, 'fixed_dim')
        isExist = os.path.exists(plot_dir)
        if not isExist:
            os.makedirs(plot_dir)
         
        make_expected_plot("Number of Workers", "Time (microseconds)", fontsize, f"fixed_dim_expected_std_time-nw_{n}.png", plot_dir, nw, standard_parallel_time, expected_std_time, "Std Par Time", "Expected Std Par Time" )

        make_expected_plot("Number of Workers", "Time (microseconds)", fontsize, f"fixed_dim_expected_ff_time-nw_{n}.png", plot_dir, nw, ff_time, expected_ff_time, "FF Time", "Expected FF Time" )

        make_plot("Number of Workers", "Time (microseconds)", fontsize, f"fixed_dim_time-nw_{n}.png", plot_dir, nw, standard_parallel_time, ff_time, sequential_time)

        standard_parallel_speedup = get_speedup(sequential_time, standard_parallel_time)
        ff_speedup = get_speedup(sequential_time, ff_time)
        make_plot("Number of Workers", "Speedup", fontsize, f"fixed_dim_speedup-nw_{n}.png", plot_dir, nw, standard_parallel_speedup, ff_speedup)

        standard_parallel_scalability = get_scalability(standard_parallel_time)
        ff_scalability = get_scalability(ff_time)
        make_plot("Number of Workers", "Scalability", fontsize, f"fixed_dim_scalability-nw_{n}.png", plot_dir, nw, standard_parallel_scalability, ff_scalability)

        standard_parallel_efficiency = get_efficiency(standard_parallel_speedup, nw)
        ff_efficiency = get_efficiency(ff_speedup, nw)
        make_plot("Number of Workers", "Efficiency", fontsize, f"fixed_dim_efficiency-nw_{n}.png", plot_dir, nw, standard_parallel_efficiency, ff_efficiency)

        

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Read the .csv files to get the plots.')
    parser.add_argument('-n', help='Size of the linear system. If you want to run it with multiple arguments use the comma , between each number...',
                        type=lambda s: [int(item) for item in s.split(',')], default = "2000,5600,11000")
    args = parser.parse_args()

    try:
        main(size = args.n)
    except Exception as e: 
                print(e)
