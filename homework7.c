
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
} Process;

void copy_processes(Process source[], Process destination[], int n) {
    for (int i = 0; i < n; i++) {
        destination[i] = source[i];
    }
}

void sort_by_arrival(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time >
                processes[j + 1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

void calculate_times(Process *p, int start_time) {
    p->waiting_time = start_time - p->arrival_time;
    p->turnaround_time = p->waiting_time + p->burst_time;
    p->response_time = p->waiting_time;
}

void print_results(Process processes[], int n,
                   const char *title, int order[]) {
    double total_waiting = 0;
    double total_turnaround = 0;
    double total_response = 0;

    printf("\n=== %s ===\n", title);
    printf("Gantt Chart:\n");

    int time = 0;
    printf("%d", time);

    for (int i = 0; i < n; i++) {
        Process *p = &processes[order[i]];

        if (time < p->arrival_time) {
            printf(" -- idle -- %d", p->arrival_time);
            time = p->arrival_time;
        }

        printf(" | P%d | %d", p->pid, time + p->burst_time);
        time += p->burst_time;
    }

    printf("\n\n");
    printf("%-8s %-8s %-8s %-8s %-8s %-8s\n",
           "PID", "AT", "BT", "WT", "TAT", "RT");

    for (int i = 0; i < n; i++) {
        Process *p = &processes[order[i]];

        printf("%-8d %-8d %-8d %-8d %-8d %-8d\n",
               p->pid, p->arrival_time, p->burst_time,
               p->waiting_time, p->turnaround_time,
               p->response_time);

        total_waiting += p->waiting_time;
        total_turnaround += p->turnaround_time;
        total_response += p->response_time;
    }

    printf("\nAverage Waiting Time: %.2f\n",
           total_waiting / n);
    printf("Average Turnaround Time: %.2f\n",
           total_turnaround / n);
    printf("Average Response Time: %.2f\n",
           total_response / n);
}

void fcfs(Process processes[], int n) {
    Process *sorted = malloc(n * sizeof(Process));
    int *order = malloc(n * sizeof(int));

    if (sorted == NULL || order == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(sorted);
        free(order);
        return;
    }

    copy_processes(processes, sorted, n);
    sort_by_arrival(sorted, n);

    int current_time = 0;

    for (int i = 0; i < n; i++) {
        if (current_time < sorted[i].arrival_time) {
            current_time = sorted[i].arrival_time;
        }

        calculate_times(&sorted[i], current_time);
        order[i] = i;
        current_time += sorted[i].burst_time;
    }

    print_results(sorted, n, "First Come First Served (FCFS)", order);

    free(sorted);
    free(order);
}

void sjf(Process processes[], int n) {
    Process *scheduled = malloc(n * sizeof(Process));
    int *order = malloc(n * sizeof(int));
    int *completed = calloc(n, sizeof(int));

    if (scheduled == NULL || order == NULL || completed == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(scheduled);
        free(order);
        free(completed);
        return;
    }

    copy_processes(processes, scheduled, n);

    int current_time = 0;

    for (int count = 0; count < n; count++) {
        int best = -1;

        for (int i = 0; i < n; i++) {
            if (completed[i] ||
                scheduled[i].arrival_time > current_time) {
                continue;
            }

            if (best == -1 ||
                scheduled[i].burst_time < scheduled[best].burst_time ||
                (scheduled[i].burst_time == scheduled[best].burst_time &&
                 (scheduled[i].arrival_time < scheduled[best].arrival_time ||
                  (scheduled[i].arrival_time == scheduled[best].arrival_time &&
                   scheduled[i].pid < scheduled[best].pid)))) {
                best = i;
            }
        }

        if (best == -1) {
            int next_arrival = -1;

            for (int i = 0; i < n; i++) {
                if (!completed[i] &&
                    (next_arrival == -1 ||
                     scheduled[i].arrival_time < next_arrival)) {
                    next_arrival = scheduled[i].arrival_time;
                }
            }

            current_time = next_arrival;
            count--;
            continue;
        }

        calculate_times(&scheduled[best], current_time);
        order[count] = best;
        current_time += scheduled[best].burst_time;
        completed[best] = 1;
    }

    print_results(scheduled, n, "Shortest Job First (SJF)", order);

    free(scheduled);
    free(order);
    free(completed);
}

int main(void) {
    int n;

    printf("Enter the number of processes: ");

    if (scanf("%d", &n) != 1 || n <= 0 || n > 10000) {
        fprintf(stderr, "Invalid number of processes. Enter 1-10000.\n");
        return 1;
    }

    Process *processes = malloc(n * sizeof(Process));

    if (processes == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;

        printf("Enter the arrival time and burst time for process %d: ",
               i + 1);

        if (scanf("%d %d", &processes[i].arrival_time,
                  &processes[i].burst_time) != 2 ||
            processes[i].arrival_time < 0 ||
            processes[i].burst_time <= 0) {
            fprintf(stderr,
                    "Invalid input. Arrival time must be non-negative "
                    "and burst time positive.\n");
            free(processes);
            return 1;
        }
    }

    fcfs(processes, n);
    sjf(processes, n);

    free(processes);
    return 0;
}
