#include <stdio.h>
#include <stdlib.h>

int *resources, *available;
int **max_claim, **allocation, **need;
int num_processes, num_resources;

void print_resource_vector() {
    printf("Resource Vector:\n");
    for (int j = 0; j < num_resources; j++) {
        printf("Resource r%d: Total = %d, Available = %d\n", j, resources[j], available[j]);
    }
    printf("\n");
}

void print_matrix() {
    printf("Max claim                        Current                        Potential\n");
    printf("r0      r1      r2              r0      r1      r2              r0      r1      r2\n");
    printf("-----------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < num_processes; i++) {
        printf("p%d      ", i);
        for (int j = 0; j < num_resources; j++) {
            printf("%-8d", max_claim[i][j]);
        }
        printf("    ");
        for (int j = 0; j < num_resources; j++) {
            printf("%-8d", allocation[i][j]);
        }
        printf("    ");
        for (int j = 0; j < num_resources; j++) {
            printf("%-8d", need[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void enter_parameters() {
    printf("Enter number of processes: ");
    scanf("%d", &num_processes);
    printf("Enter number of resources: ");
    scanf("%d", &num_resources);

    // Allocate memory for vectors and arrays
    resources = (int *)malloc(num_resources * sizeof(int));
    available = (int *)malloc(num_resources * sizeof(int));
    max_claim = (int **)malloc(num_processes * sizeof(int *));
    allocation = (int **)malloc(num_processes * sizeof(int *));
    need = (int **)malloc(num_processes * sizeof(int *));
    for (int i = 0; i < num_processes; i++) {
        max_claim[i] = (int *)malloc(num_resources * sizeof(int));
        allocation[i] = (int *)malloc(num_resources * sizeof(int));
        need[i] = (int *)malloc(num_resources * sizeof(int));
    }

    // Enter the number of units for each resource
    printf("Enter number of units for resources (r0 to r%d): ", num_resources - 1);
    for (int j = 0; j < num_resources; j++) {
        scanf("%d", &resources[j]);
        available[j] = resources[j]; // Initially, all resources are available
    }

    // Enter maximum number of units each process may request
    for (int i = 0; i < num_processes; i++) {
        printf("Enter maximum number of units process p%d will request from each resource (r0 to r%d): ", i, num_resources - 1);
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &max_claim[i][j]);
            need[i][j] = max_claim[i][j]; // Initially, need = max_claim
        }
    }

    // Enter allocation of resources to each process
    for (int i = 0; i < num_processes; i++) {
        printf("Enter number of units of each resource (r0 to r%d) allocated to process p%d: ", num_resources - 1, i);
        for (int j = 0; j < num_resources; j++) {
            scanf("%d", &allocation[i][j]);
            need[i][j] -= allocation[i][j]; // Update need
            available[j] -= allocation[i][j]; // Update available
        }
    }

    // Print the resource vector and matrices
    print_resource_vector();
    print_matrix();
}

void check_safe_sequence() {
    int safe_sequence[num_processes];
    int safe_index = 0;
    int work[num_resources];
    int finish[num_processes];

    // Initialize work and finish
    for (int i = 0; i < num_resources; i++) {
        work[i] = available[i];
    }
    for (int i = 0; i < num_processes; i++) {
        finish[i] = 0; // 0 means not finished, 1 means finished
    }

    int process_checked = 0;

    // Find safe sequence
    while (process_checked < num_processes) {
        int sequenced = 0;
        for (int i = 0; i < num_processes; i++) {
            if (!finish[i]) {
                int can_sequence = 1;
                printf("Checking: < ");
                for (int j = 0; j < num_resources; j++) {
                    printf("%d ", need[i][j]);
                }
                printf("> <= < ");
                for (int j = 0; j < num_resources; j++) {
                    printf("%d ", work[j]);
                }

                for (int j = 0; j < num_resources; j++) {
                    if (need[i][j] > work[j]) {
                        can_sequence = 0;
                        break;
                    }
                }

                if (can_sequence) {
                    printf("> :p%d safely sequenced\n", i);
                    safe_sequence[safe_index++] = i;
                    finish[i] = 1;
                    for (int j = 0; j < num_resources; j++) {
                        work[j] += allocation[i][j];
                    }
                    sequenced = 1;
                    process_checked++;
                    break;
                } else {
                    printf("> :p%d could not be sequenced\n", i);
                }
            }
        }

        if (!sequenced) {
            break; // No process could be sequenced
        }
    }

    // Print safe sequence
    printf("Safe Sequence: ");
    for (int i = 0; i < safe_index; i++) {
        printf("p%d ", safe_sequence[i]);
    }
    printf("\n");
}

void quit_program() {
    // Free allocated memory
    free(resources);
    free(available);
    for (int i = 0; i < num_processes; i++) {
        free(max_claim[i]);
        free(allocation[i]);
        free(need[i]);
    }
    free(max_claim);
    free(allocation);
    free(need);
    printf("Quitting program...\n");
}

int main() {
    int choice;
    do {
        printf("Banker's Algorithm\n------------------\n");
        printf("1) Enter parameters\n");
        printf("2) Determine safe sequence\n");
        printf("3) Quit program\n");
        printf("Enter selection: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                enter_parameters();
                break;
            case 2:
                check_safe_sequence();
                break;
            case 3:
                quit_program();
                break;
            default:
                printf("Invalid selection. Try again.\n");
        }
    } while (choice != 3);

    return 0;
}
