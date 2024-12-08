#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int childIndex;
    struct Node* next;
} Node;

typedef struct PCB {
    int parent;
    Node* children;
} PCB;

PCB* pcbs;
int maxProcesses = 0;
int currentProcesses = 0;

void printChildren(int parentIndex) {
    // PROCEDURE TO PRINT HIERARCHY OF PROCESSES
    Node* current = pcbs[parentIndex].children;
    
    if (current == NULL) {
        return;
    }

    printf("PCB[%d] is the parent of: ", parentIndex);
    while (current != NULL) {
        printf("PCB[%d] ", current->childIndex);
        current = current->next;
    }
    printf("\n");
}

void create() {
    // PROCEDURE FOR OPTION #1
    int parentIndex;
    printf("Enter the parent process index: ");
    scanf("%d", &parentIndex);
    
    if (parentIndex < 0 || parentIndex >= currentProcesses) {
        printf("Invalid parent process index.\n");
        return;
    }
    
    if (currentProcesses >= maxProcesses) {
        printf("Cannot create new process. Maximum limit reached.\n");
        return;
    }
    
    int childIndex = currentProcesses;
    pcbs[childIndex].parent = parentIndex;
    pcbs[childIndex].children = NULL;

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->childIndex = childIndex;
    newNode->next = NULL;

    if (pcbs[parentIndex].children == NULL) {
        pcbs[parentIndex].children = newNode;
    } else {
        Node* temp = pcbs[parentIndex].children;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    currentProcesses++;
    
    for (int i = 0; i < currentProcesses; i++) {
        if (pcbs[i].children != NULL) {
            printChildren(i);
        }
    }
}

void destroy(int parentIndex) {
    // RECURSIVE PROCEDURE TO DESTROY CHILDREN PROCESSES
    Node* current = pcbs[parentIndex].children;
    
    while (current != NULL) {
        int childIndex = current->childIndex;
        destroy(childIndex);
        current = current->next;
    }

    current = pcbs[parentIndex].children;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    pcbs[parentIndex].children = NULL;
}

void enterParameters() {
    // PROCEDURE FOR OPTION #2
    printf("Enter maximum number of processes: ");
    scanf("%d", &maxProcesses);
    
    pcbs = (PCB*)malloc(maxProcesses * sizeof(PCB));
    pcbs[0].parent = -1;
    pcbs[0].children = NULL;
    currentProcesses = 1;
    printf("Process PCB[0] created as the root process.\n");
}

void freeMemory() {
    // PROCEDURE FOR OPTION #4
    for (int i = 0; i < currentProcesses; i++) {
        Node* current = pcbs[i].children;
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pcbs);
    printf("Memory freed. Quitting program...\n");
}

int main() {
    // MAIN PROGRAM
    int choice;
    
    do {
        printf("Process creation and destruction\n");
        printf("--------------------------------\n");
        printf("1) Enter parameters\n");
        printf("2) Create a new child process\n");
        printf("3) Destroy all descendants of a process\n");
        printf("4) Quit program and free memory\n");
        printf("Enter selection: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                enterParameters();
                break;
            case 2:
                create();
                break;
            case 3:
                printf("Enter the parent process index to destroy all descendants: ");
                int parentToDestroy;
                scanf("%d", &parentToDestroy);
                if (parentToDestroy >= 0 && parentToDestroy < currentProcesses) {
                    destroy(parentToDestroy);
                    printf("All descendants of PCB[%d] have been destroyed.\n", parentToDestroy);
                } else {
                    printf("Invalid parent process index.\n");
                }
                break;
            case 4:
                freeMemory();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    return 0; // indicates success
}
