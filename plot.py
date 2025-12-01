import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

# Constants
FIG_DIR = "test/fig"
RESULTS_DIR = "test/results"

# Ensure figure directory exists
os.makedirs(FIG_DIR, exist_ok=True)

def plot_q1_1():
    file_path = os.path.join(RESULTS_DIR, "q1_1.txt")
    if not os.path.exists(file_path): return
    
    df = pd.read_csv(file_path, sep=' ')
    
    # Calculate Speedup
    df['Speedup'] = df['Serial_Time'] / df['Parallel_Time']
    
    plt.figure(figsize=(10, 6))
    for n in df['N'].unique():
        subset = df[df['N'] == n]
        plt.plot(subset['Threads'], subset['Speedup'], marker='o', label=f'N={n}')
        
    plt.xlabel('Number of Threads')
    plt.ylabel('Speedup (Serial / Parallel)')
    plt.title('Q1.1: Polynomial Multiplication Speedup')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(FIG_DIR, 'q1_1_speedup.png'))
    plt.close()

def plot_q1_2():
    file_path = os.path.join(RESULTS_DIR, "q1_2.txt")
    if not os.path.exists(file_path): return
    
    df = pd.read_csv(file_path, sep=' ')
    
    plt.figure(figsize=(10, 6))
    for impl in df['Impl'].unique():
        subset = df[df['Impl'] == impl]
        plt.plot(subset['Threads'], subset['Time'], marker='o', label=impl)
        
    plt.xlabel('Number of Threads')
    plt.ylabel('Execution Time (sec)')
    plt.title('Q1.2: Shared Variable Update Performance')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(FIG_DIR, 'q1_2_time.png'))
    plt.close()

def plot_q1_3():
    file_path = os.path.join(RESULTS_DIR, "q1_3.txt")
    if not os.path.exists(file_path): return
    
    df = pd.read_csv(file_path, sep=' ')
    
    plt.figure(figsize=(10, 6))
    plt.plot(df['Size'], df['Serial_Time'], marker='o', label='Serial')
    plt.plot(df['Size'], df['Parallel_Time'], marker='o', label='Parallel (4 Threads)')
    
    plt.xlabel('Array Size')
    plt.ylabel('Execution Time (sec)')
    plt.xscale('log')
    plt.title('Q1.3: Array Stats Performance')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(FIG_DIR, 'q1_3_time.png'))
    plt.close()

def plot_q1_4():
    file_path = os.path.join(RESULTS_DIR, "q1_4_read.txt")
    if not os.path.exists(file_path): return
    
    df = pd.read_csv(file_path, sep=' ')
    
    plt.figure(figsize=(10, 6))
    for strat in df['Strategy'].unique():
        subset = df[df['Strategy'] == strat]
        plt.plot(subset['ReadPerc'], subset['Time'], marker='o', label=strat)
        
    plt.xlabel('Read Percentage (%)')
    plt.ylabel('Execution Time (sec)')
    plt.title('Q1.4: Bank Simulation (Varying Read Workload)')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(FIG_DIR, 'q1_4_read_perf.png'))
    plt.close()

def plot_q1_5():
    file_path = os.path.join(RESULTS_DIR, "q1_5.txt")
    if not os.path.exists(file_path): return
    
    df = pd.read_csv(file_path, sep=' ')
    
    plt.figure(figsize=(10, 6))
    for barrier in df['Barrier'].unique():
        subset = df[df['Barrier'] == barrier]
        plt.plot(subset['Threads'], subset['Time'], marker='o', label=barrier)
        
    plt.xlabel('Number of Threads')
    plt.ylabel('Execution Time (sec)')
    plt.title('Q1.5: Barrier Implementation Comparison')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(FIG_DIR, 'q1_5_barriers.png'))
    plt.close()

if __name__ == "__main__":
    try:
        plot_q1_1()
        plot_q1_2()
        plot_q1_3()
        plot_q1_4()
        plot_q1_5()
        print("Plots generated successfully.")
    except Exception as e:
        print(f"Error generating plots: {e}")