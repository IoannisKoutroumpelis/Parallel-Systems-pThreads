#!/bin/bash

# Configuration
RESULTS_DIR="test/results"
FIG_DIR="test/fig"

# Ensure directories exist
mkdir -p $RESULTS_DIR
mkdir -p $FIG_DIR

# Compile everything
echo "--- Compiling ---"
make clean
make all
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# --- Q1.1: Polynomial Multiplication ---
echo "--- Running Q1.1 (Polynomial Multiplication) ---"
OUT_FILE="$RESULTS_DIR/q1_1.txt"
echo "N Threads Serial_Time Parallel_Time" > "$OUT_FILE"

# Vary Degree N and Threads
for n in 10000 50000; do
    for t in 1 2 4 8; do
        echo "  Running N=$n Threads=$t"
        output=$(./bin/q1_1 $n $t)
        
        # Output: "Serial time: X sec" / "Parallel time: Y sec"
        s_time=$(echo "$output" | grep "Serial time" | awk '{print $3}')
        p_time=$(echo "$output" | grep "Parallel time" | awk '{print $3}')
        
        echo "$n $t $s_time $p_time" >> "$OUT_FILE"
    done
done

# --- Q1.2: Shared Variable Update ---
echo "--- Running Q1.2 (Shared Variable Update) ---"
OUT_FILE="$RESULTS_DIR/q1_2.txt"
echo "Threads Impl Time" > "$OUT_FILE"

# Impl: 1=Atomic, 2=Mutex, 3=RWLock
for t in 1 2 4 8; do
    for impl in 1 2 3; do
        impl_name="Atomic"
        if [ "$impl" -eq 2 ]; then impl_name="Mutex"; fi
        if [ "$impl" -eq 3 ]; then impl_name="RWLock"; fi
        
        echo "  Running Threads=$t Impl=$impl_name"
        output=$(./bin/q1_2 $impl $t)
        
        # Output formats:
        # "Mutex lock implementation time: (X threads): Y sec" -> $7
        # "Read-write lock implementation time: (X threads): Y sec" -> $7
        # "atomic operations implementation time: (X threads): Y sec" -> $7
        time_val=$(echo "$output" | grep "time:" | awk '{print $7}')
        
        echo "$t $impl_name $time_val" >> "$OUT_FILE"
    done
done

# --- Q1.3: Array Stats ---
echo "--- Running Q1.3 (Array Stats) ---"
OUT_FILE="$RESULTS_DIR/q1_3.txt"
echo "Size Serial_Time Parallel_Time" > "$OUT_FILE"

# Vary Array Size
for size in 100000 1000000 10000000; do
    echo "  Running Size=$size"
    output=$(./bin/q1_3 $size)
    
    # Output: "Serial Execution Time: X sec" -> $4
    s_time=$(echo "$output" | grep "Serial Execution Time" | awk '{print $4}')
    p_time=$(echo "$output" | grep "Parallel Execution Time" | awk '{print $4}')
    
    echo "$size $s_time $p_time" >> "$OUT_FILE"
done

# --- Q1.4: Bank Simulation ---
echo "--- Running Q1.4 (Bank Simulation) ---"
OUT_FILE="$RESULTS_DIR/q1_4_read.txt"
echo "ReadPerc Strategy Time" > "$OUT_FILE"

# Fixed parameters
ACCOUNTS=1000
TRANS=20000
THREADS=4

# Vary Read Percentage
for rp in 0 50 90 100; do
    for strat in 1 2 3 4; do
        strat_name="CoarseMutex"
        if [ "$strat" -eq 2 ]; then strat_name="CoarseRW"; fi
        if [ "$strat" -eq 3 ]; then strat_name="FineMutex"; fi
        if [ "$strat" -eq 4 ]; then strat_name="FineRW"; fi
        
        echo "  Running Read%=$rp Strategy=$strat_name"
        # Usage: ./q1_4 <accounts> <trans> <read%> <strat> <threads>
        output=$(./bin/q1_4 $ACCOUNTS $TRANS $rp $strat $THREADS)
        
        # Output is time on the first line
        time_val=$(echo "$output" | head -n 1)
        
        echo "$rp $strat_name $time_val" >> "$OUT_FILE"
    done
done

# --- Q1.5: Barriers ---
echo "--- Running Q1.5 (Barriers) ---"
OUT_FILE="$RESULTS_DIR/q1_5.txt"
echo "Threads Barrier Time" > "$OUT_FILE"

ITERS=100000

for t in 2 4 8; do
    echo "  Running Threads=$t"
    
    # Pthread Barrier: "Pthread Barrier Time: X sec" -> $4
    out=$(./bin/q1_5_pthread $t $ITERS)
    time_val=$(echo "$out" | grep "Time:" | awk '{print $4}')
    echo "$t Pthread $time_val" >> "$OUT_FILE"
    
    # Cond Var Barrier: "Condition Variable Barrier Time: X sec" -> $5
    out=$(./bin/q1_5_cond $t $ITERS)
    time_val=$(echo "$out" | grep "Time:" | awk '{print $5}')
    echo "$t CondVar $time_val" >> "$OUT_FILE"
    
    # Sense Reversal Barrier: "Sense Reversal Barrier Time: X sec" -> $5
    out=$(./bin/q1_5_sense $t $ITERS)
    time_val=$(echo "$out" | grep "Time:" | awk '{print $5}')
    echo "$t SenseRev $time_val" >> "$OUT_FILE"
done

# Generate Plots
echo "--- Generating plots ---"
if command -v python3 &> /dev/null; then
    python3 plot.py
else
    echo "Python3 not found, skipping plot generation."
fi

echo "Done. Results in $RESULTS_DIR, Plots in $FIG_DIR"