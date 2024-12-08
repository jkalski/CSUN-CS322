#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* declare global variables including a table structure to hold scheduling information */
typedef struct { 
    int id;
    int arrival;
    int total_cpu;
    int total_remaining;
    int done;
    int start_time;
    int already_started;
    int end_time;
    int turnaround_time;
} Process;

Process *processes;
int num_processes;

/***************************************************************/
void print_table() {
    printf("%-5s %-7s %-6s %-6s %-6s %-10s\n", "ID", "Arrival", "Total", "Start", "End", "Turnaround");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < num_processes; i++) {
        printf("%-5d %-7d %-6d", processes[i].id, processes[i].arrival, processes[i].total_cpu);
        if (processes[i].done == 1) {
            printf(" %-6d %-6d %-10d\n", processes[i].start_time, processes[i].end_time, processes[i].turnaround_time);
        } else {
            printf("\n"); 
        }
    }
}
/***************************************************************/
void option_1() {
    printf("Enter total number of processes: ");
    scanf("%d", &num_processes);
    processes = (Process *)malloc(num_processes * sizeof(Process));
    
    for (int i = 0; i < num_processes; i++) {
        printf("Enter process id: ");
        scanf("%d", &processes[i].id);
        printf("Enter arrival cycle for process P[%d]: ", processes[i].id);
        scanf("%d", &processes[i].arrival);
        printf("Enter total cycles for process P[%d]: ", processes[i].id);
        scanf("%d", &processes[i].total_cpu);
        
        processes[i].total_remaining = processes[i].total_cpu;
        processes[i].done = 0;
        processes[i].start_time = -1;
        processes[i].already_started = 0;
        processes[i].end_time = -1;
        processes[i].turnaround_time = -1;
    }

    print_table();
}
/***************************************************************/
void option_2() {
    int current_time = 0;
    int processes_scheduled = 0;

    for (int i = 0; i < num_processes; i++) {
        processes[i].done = 0; 
    }

    while (processes_scheduled < num_processes) {
        int earliest_arrival = INT_MAX;
        int index = -1;
        
        for (int i = 0; i < num_processes; i++) {
            if (!processes[i].done && processes[i].arrival < earliest_arrival) {
                earliest_arrival = processes[i].arrival;
                index = i;
            }
        }
        
        if (index != -1) {
            processes[index].start_time = current_time;
            current_time += processes[index].total_cpu;
            processes[index].end_time = current_time;
            processes[index].turnaround_time = processes[index].end_time - processes[index].arrival;
            processes[index].done = 1;
            processes_scheduled++;
        }
    }

    print_table();
}
/***************************************************************/
void option_3() {
    int current_time = 0;
    int processes_scheduled = 0;

    for (int i = 0; i < num_processes; i++) {
        processes[i].done = 0; 
    }

    while (processes_scheduled < num_processes) {
        int shortest_job = INT_MAX;
        int index = -1;
        
        for (int i = 0; i < num_processes; i++) {
            if (!processes[i].done && processes[i].total_cpu < shortest_job && processes[i].arrival <= current_time) {
                shortest_job = processes[i].total_cpu;
                index = i;
            }
        }
        
        if (index != -1) {
            processes[index].start_time = current_time;
            current_time += processes[index].total_cpu;
            processes[index].end_time = current_time;
            processes[index].turnaround_time = processes[index].end_time - processes[index].arrival;
            processes[index].done = 1;
            processes_scheduled++;
        } else {
            current_time++; 
        }
    }

    print_table();
}
/***************************************************************/
void option_4() {
    int current_time = 0;
    int processes_scheduled = 0;

    for (int i = 0; i < num_processes; i++) {
        processes[i].done = 0; //  done flag
        processes[i].total_remaining = processes[i].total_cpu; //  remaining time
        processes[i].already_started = 0; // already started flag
    }

    while (processes_scheduled < num_processes) {
        int shortest_remaining = INT_MAX;
        int index = -1;
        
        for (int i = 0; i < num_processes; i++) {
            if (!processes[i].done && processes[i].total_remaining < shortest_remaining && processes[i].arrival <= current_time) {
                shortest_remaining = processes[i].total_remaining;
                index = i;
            }
        }
        
        if (index != -1) {
            if (!processes[index].already_started) {
                processes[index].start_time = current_time;
                processes[index].already_started = 1;
            }
            
            current_time++;
            processes[index].total_remaining--;
            
            if (processes[index].total_remaining == 0) {
                processes[index].end_time = current_time;
                processes[index].turnaround_time = processes[index].end_time - processes[index].arrival;
                processes[index].done = 1;
                processes_scheduled++;
            }
        } else {
            current_time++; //  no process is ready advance time
        }
    }

    print_table();
}
/***************************************************************/
void option_5() {
    if (processes != NULL) {
        free(processes);
    }
    printf("Memory freed, quitting...\n");
}
/***************************************************************/
int main() {
    int choice;

    do {
        printf("Batch scheduling\n");
        printf("----------------\n");
        printf("1) Enter parameters\n");
        printf("2) Schedule processes with FIFO algorithm\n");
        printf("3) Schedule processes with SJF algorithm\n");
        printf("4) Schedule processes with SRT algorithm\n");
        printf("5) Quit and free memory\n");
        printf("Enter selection: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                option_1();
                break;
            case 2:
                option_2();
                break;
            case 3:
                option_3();
                break;
            case 4:
                option_4();
                break;
            case 5:
                option_5();
                break;
            default:
                printf("Invalid selection, please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
