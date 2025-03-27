#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define N 100
int pid[N], at[N], bt[N], Size[N];
int num_pro = 0, i;
int startadd = 0;
int endadd = 31;

struct Node {
    int Size;
    int pid;
    int startAddr;
    int endAddr;
    struct Node* prev;
    struct Node* next;
};

struct Node* initial_node = NULL;

// Function to create a new node
struct Node* createNode(int size, int pid, int stadd, int edadd, struct Node* prev, struct Node* next) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newNode->Size = size;
    newNode->pid = pid;
    newNode->startAddr = stadd;
    newNode->endAddr = edadd;
    newNode->prev = prev;
    newNode->next = next;
    return newNode;
}

// Find best fit hole
struct Node* Find(int size) {
    int min_size = INT_MAX;
    struct Node* temp = initial_node;
    struct Node* bestFit = NULL;
    
    while (temp != NULL) {
        if (temp->pid == -1 && temp->Size >= size && temp->Size < min_size) {
            min_size = temp->Size;
            bestFit = temp;
        }
        temp = temp->next;
    }

    if (bestFit != NULL) {
        printf("Selected hole: StartAddr=%d, EndAddr=%d, Size=%d\n", bestFit->startAddr, bestFit->endAddr, bestFit->Size);
    } else {
        printf("No suitable hole found for size %d!\n", size);
    }

    return bestFit;
}

// Allocate process in selected hole
struct Node* Add(struct Node* tempF, int s, int p) {
    if (tempF == NULL) {
        printf("No suitable hole found for Process %d\n", p);
        return NULL;
    }

    printf("Allocating Process %d in hole at StartAddr=%d, EndAddr=%d\n", p, tempF->startAddr, tempF->endAddr);

    tempF->pid = p;
    int oldEnd = tempF->endAddr;
    tempF->endAddr = tempF->startAddr + s - 1;

    if (tempF->Size > s) {
        struct Node* newHole = createNode(tempF->Size - s, -1, tempF->endAddr + 1, oldEnd, tempF, tempF->next);
        tempF->Size = s;
        if (tempF->next) {
            tempF->next->prev = newHole;
        }
        tempF->next = newHole;
        printf("Created new hole at StartAddr=%d, EndAddr=%d, Size=%d\n", newHole->startAddr, newHole->endAddr, newHole->Size);
    }

    return tempF;
}

// Free memory and merge adjacent holes
void Free(int p) {
    struct Node* temp = initial_node;

    // Find the process
    while (temp != NULL) {
        if (temp->pid == p) {
            break;
        }
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Process %d not found in memory!\n", p);
        return;
    }

    printf("Freeing Process %d from StartAddr=%d, EndAddr=%d\n", p, temp->startAddr, temp->endAddr);
    
    temp->pid = -1;  // Mark as a hole

    // Merge with previous hole
    if (temp->prev != NULL && temp->prev->pid == -1) {
        printf("Merging with previous hole at StartAddr=%d, EndAddr=%d\n", temp->prev->startAddr, temp->prev->endAddr);
        temp->prev->Size += temp->Size;
        temp->prev->endAddr = temp->endAddr;
        temp->prev->next = temp->next;
        if (temp->next) {
            temp->next->prev = temp->prev;
        }
        free(temp);
        temp = temp->prev;  // Move back to merged hole
    }

    // Merge with next hole
    if (temp->next != NULL && temp->next->pid == -1) {
        printf("Merging with next hole at StartAddr=%d, EndAddr=%d\n", temp->next->startAddr, temp->next->endAddr);
        temp->Size += temp->next->Size;
        temp->endAddr = temp->next->endAddr;
        struct Node* toDelete = temp->next;
        temp->next = temp->next->next;
        if (temp->next) {
            temp->next->prev = temp;
        }
        free(toDelete);
    }
}

// Print memory state
void printMemory() {
    struct Node* temp = initial_node;
    printf("\nMemory Layout:\n");
    while (temp != NULL) {
        printf("[PID: %d | Size: %d | Start: %d | End: %d] -> ", temp->pid, temp->Size, temp->startAddr, temp->endAddr);
        temp = temp->next;
    }
    printf("NULL\n\n");
}

int main() {
    FILE *file = fopen("processes.txt", "r");
    if (file == NULL) {
        printf("Error opening the file!\n");
        return 1;
    }

    char buffer[100];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        num_pro++;
    }
    num_pro -= 1;

    rewind(file);
    int count = 0;
    fgets(buffer, sizeof(buffer), file);

    while (fgets(buffer, sizeof(buffer), file) != NULL && count < num_pro) {
        int num1, num2, num3, num4;
        if (sscanf(buffer, "%d %d %d %d", &num1, &num2, &num3, &num4) >= 4) {
            pid[count] = num1;
            at[count] = num2;
            bt[count] = num3;
            Size[count] = num4;
            count++;
        }
    }

    fclose(file);

    initial_node = createNode(32, -1, startadd, endadd, NULL, NULL);

    int current_time = 0, completed = 0;
    bool executing[num_pro];
    for (i = 0; i < num_pro; i++) executing[i] = false;

    while (completed != num_pro) {
        for (i = 0; i < num_pro; i++) {
            if (!executing[i] && current_time >= at[i]) {
                struct Node* f = Find(Size[i]);
                if (f) {
                    Add(f, Size[i], pid[i]);
                    executing[i] = true;
                }
                printMemory();
            }
        }

        for (i = 0; i < num_pro; i++) {
            if (executing[i] && current_time == at[i] + bt[i]) {
                Free(pid[i]);
                executing[i] = false;
                completed++;
                printMemory();
            }
        }

        current_time++;
    }

    return 0;
}
/*
processes.txt
p a b s
1 0 4 1
2 1 4 5
3 2 5 6
4 3 6 2
5 4 7 1
6 5 2 1
7 6 3 1
8 7 5 4
9 8 6 7
*/
