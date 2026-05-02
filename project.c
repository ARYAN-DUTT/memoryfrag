#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_SIZE 64   // Small size for clear terminal visualization
#define MIN_SPLIT 2    // Minimum size to split a block

typedef struct Block {
    int size;
    int is_free;
    struct Block* next;
} Block;

char heap[HEAP_SIZE]; 
Block* head;

// Initialize the "Heap"
void init_heap() {
    head = (Block*)malloc(sizeof(Block));
    head->size = HEAP_SIZE;
    head->is_free = 1;
    head->next = NULL;
}

// Visualization Logic
void visualize_heap() {
    printf("\n[");
    Block* curr = head;
    while (curr) {
        for (int i = 0; i < curr->size; i++) {
            if (curr->is_free) printf("."); // . = Free
            else printf("#");               // # = Allocated
        }
        if (curr->next) printf("|");        // | = Block boundary
        curr = curr->next;
    }
    printf("]\n");
}

// Analysis Logic
void analyze_fragmentation() {
    int total_free = 0;
    int max_contiguous = 0;
    int free_blocks = 0;

    Block* curr = head;
    while (curr) {
        if (curr->is_free) {
            total_free += curr->size;
            if (curr->size > max_contiguous) max_contiguous = curr->size;
            free_blocks++;
        }
        curr = curr->next;
    }

    float frag_ratio = (total_free == 0) ? 0 : (float)(total_free - max_contiguous) / total_free;
    
    printf("--- STATS ---\n");
    printf("Total Free: %d bytes | Max Contiguous: %d bytes\n", total_free, max_contiguous);
    printf("External Fragmentation Index: %.2f (0.00 is perfect)\n", frag_ratio);
}

// Allocation Logic (First Fit) based on Screenshot 2026-05-01 at 9.00.47 AM.jpg
void allocate(int request_size) {
    Block* curr = head;
    while (curr && (curr->is_free == 0 || curr->size < request_size)) {
        curr = curr->next;
    }

    if (curr) {
        int waste = curr->size - request_size;
        if (waste >= MIN_SPLIT) {
            Block* new_block = (Block*)malloc(sizeof(Block));
            new_block->size = waste;
            new_block->is_free = 1;
            new_block->next = curr->next;
            
            curr->size = request_size;
            curr->next = new_block;
        }
        curr->is_free = 0;
        printf("Allocated %d bytes.", request_size);
    } else {
        printf("Error: Memory allocation of %d bytes failed!", request_size);
    }
}

// Simple Free Logic
void deallocate(int target_size) {
    Block* curr = head;
    while (curr) {
        if (!curr->is_free && curr->size == target_size) {
            curr->is_free = 1;
            printf("Freed block of size %d.", target_size);
            return;
        }
        curr = curr->next;
    }
}

int main() {
    init_heap();
    int choice, val;

    while (1) {
        visualize_heap();
        analyze_fragmentation();
        printf("\n1. Allocate  2. Free  3. Exit\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Size to allocate: ");
            scanf("%d", &val);
            allocate(val);
        } else if (choice == 2) {
            printf("Size of block to free: ");
            scanf("%d", &val);
            deallocate(val);
        } else break;
    }
    return 0;
}