
# Process Scheduling Simulation (FCFS & SJF)

## Description

A C program that simulates two non-preemptive process scheduling algorithms:

- First Come First Served (FCFS)
- Shortest Job First (SJF)

For each algorithm, the program displays a Gantt chart, process timing metrics, and average waiting, turnaround, and response times.

## Requirements

- GCC compiler
- Linux environment

## Compilation

Run the following command from the project directory:

```bash
gcc homework7.c
```

## Execution

Run the compiled program:

```bash
./a.out
```

Enter the number of processes, followed by the arrival time and burst time of each process.

## Scheduling Algorithms

### FCFS
Processes are executed in arrival order. When arrival times are equal, the original input order is preserved.

### SJF
The process with the shortest burst time is selected from the processes that have already arrived. Ties are resolved by arrival time, then PID. The algorithm is non-preemptive.

## Time Metrics

- Waiting Time = Start Time - Arrival Time
- Turnaround Time = Completion Time - Arrival Time
- Response Time = Start Time - Arrival Time
