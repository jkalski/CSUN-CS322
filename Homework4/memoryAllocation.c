#include <stdio.h>
#include <stdlib.h>

// Structure for memory blocks
typedef struct Block {
    int id;
    int start;
    int end;
    struct Block *next;
} Block;

// Global variables
int pm_size = 0;          // Physical memory size
int hole_fitting = 0;     // 0 = First-fit, 1 = Best-fit
int remaining_memory = 0; // unalocated mem
Block *head = NULL;       // Head of linked list

void enter_parameters() {
    printf("Enter size of physical memory: ");
    scanf("%d", &pm_size);
    printf("Enter hole-fitting algorithm (0=first-fit, 1=best-fit): ");
    scanf("%d", &hole_fitting);

    remaining_memory = pm_size;
    head = malloc(sizeof(Block)); // Initialize dummy block
    head->id = -1;
    head->start = 0;
    head->end = 0;
    head->next = NULL;
}

void print_allocation_table() {
    printf("ID   Start   End\n");
    printf("-----------------\n");
    Block *current = head->next; // Skip dummy block
    while (current) {
        printf("%d    %d      %d\n", current->id, current->start, current->end);
        current = current->next;
    }
}

void allocate_memory() {
    int id, size;
    printf("Enter block id: ");
    scanf("%d", &id);
    printf("Enter block size: ");
    scanf("%d", &size);

    if (size > remaining_memory) {
        printf("Not enough memory to allocate block.\n");
        return;
    }

    Block *current = head;
    Block *best_block = NULL;
    int best_size = pm_size + 1; 

    while (current->next) {
        int hole_size = current->next->start - current->end;
        if (hole_size >= size) {
            if (hole_fitting == 0) { // First-fit
                best_block = current;
                break;
            } else if (hole_size < best_size) { // Best-fit
                best_size = hole_size;
                best_block = current;
            }
        }
        current = current->next;
    }

    if (!best_block && pm_size - current->end >= size) {
        best_block = current; // Allocate at the end
    }

    if (!best_block) {
        printf("No suitable hole found for allocation.\n");
        return;
    }

    Block *new_block = malloc(sizeof(Block));
    new_block->id = id;
    new_block->start = best_block->end;
    new_block->end = new_block->start + size;
    new_block->next = best_block->next;
    best_block->next = new_block;

    remaining_memory -= size;
    print_allocation_table();
}

void deallocate_memory() {
    int id;
    printf("Enter block id: ");
    scanf("%d", &id);

    Block *current = head;
    while (current->next && current->next->id != id) {
        current = current->next;
    }

    if (!current->next) {
        printf("Block id not found.\n");
        return;
    }

    Block *to_free = current->next;
    current->next = to_free->next;
    remaining_memory += (to_free->end - to_free->start);
    free(to_free);

    print_allocation_table();
}

void defragment_memory() {
    Block *current = head->next; // Skip dummy block
    int current_start = 0;

    while (current) {
        int block_size = current->end - current->start;
        current->start = current_start;
        current->end = current->start + block_size;
        current_start = current->end;
        current = current->next;
    }

    print_allocation_table();
}

void quit_program(Block *node) {
    if (!node) return;
    quit_program(node->next);
    free(node);
}

int main() {
    int choice;

    while (1) {
        printf("Memory allocation\n");
        printf("-----------------\n");
        printf("1) Enter parameters\n");
        printf("2) Allocate memory for block\n");
        printf("3) Deallocate memory for block\n");
        printf("4) Defragment memory\n");
        printf("5) Quit program\n");
        printf("Enter selection: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: enter_parameters(); break;
            case 2: allocate_memory(); break;
            case 3: deallocate_memory(); break;
            case 4: defragment_memory(); break;
            case 5: quit_program(head); return 0;
            default: printf("Invalid selection.\n");
        }
    }
}
