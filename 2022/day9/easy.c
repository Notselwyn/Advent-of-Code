// Figure out where not to step
// If T too far from H then T moves to H
// INPUT: Series of motions (H)ead and (T)ail
// OUTPUT: AMOUNT of positions the tail has traveled
// Algorithm:
// - H and T must always be touching (9x9 area)
// - start: H same pos as T
// - if T is 2 straight steps away from H, T moves 1 step to H
// - if T is 2 diagional steps away from H, T moves 1 diagonal step to H
//
//
// Can't O(1) because R3 -> L2
// Create state machine: don't use actual head/tail objects
// - save calculated coordinates into linked list
// - save relative H side of T
// - R3: x+=2 (T = H)
// - R1: x+=1
// - L2: - (T->H : H<-T)
// - U1: - 
// - U1: x+=1, y+=1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned char HEAD_UP = 1;
const unsigned char HEAD_RIGHT = 2;
const unsigned char HEAD_DOWN = 4;
const unsigned char HEAD_LEFT = 8;
const unsigned char HEAD_DIAG = 16;
const unsigned char HEAD_OVERLAP = 32;


typedef struct {
    int** positions;
    int capacity;
    int len;
} history;


history* history_add(history* hst, int* position)
{
    for (int i=0; i < hst->len; i++)
        if (!memcmp(hst->positions[i], position, 8))
            return NULL;

    if (hst->len == hst->capacity)
    {
        hst->capacity = hst->capacity ? hst->capacity * 2 : 8;
        hst->positions = realloc(hst->positions, hst->capacity * sizeof(int*));
        if (!hst->positions)
        {
            puts("ERR @ history_add(): invalid realloc retv");
            exit(1);
        }
    }

    hst->positions[hst->len] = malloc(2 * sizeof(int));
    memcpy(hst->positions[hst->len], position, 8);
    
    hst->len++;
    return hst;
}


unsigned int* dist(int* tail, int* head)
{
    unsigned int* dist = malloc(2 * sizeof(int)); // y, x
    dist[0] = abs(tail[0]-head[0]);
    dist[1] = abs(tail[1]-head[1]);

    return dist;
}


void tick(int* tail, int* head, unsigned char steps_dir)
{
    if (steps_dir == HEAD_UP)
        head[0]++;
    else if (steps_dir == HEAD_RIGHT)
        head[1]++;
    else if (steps_dir == HEAD_DOWN)
        head[0]--;
    else if (steps_dir == HEAD_LEFT)
        head[1]--;
    else {
        puts("ERR @ tick(): steps_dir invalid");
        exit(1);
    }

    unsigned int* dist_th = dist(tail, head);
    if (dist_th[0] > 2 || dist_th[1] > 2)
    {
        printf("ERR @ tick(): dist too big (dy: %d, dx: %d)\n", dist_th[0], dist_th[1]);
        exit(1);
    }

    if ((steps_dir == HEAD_UP || steps_dir == HEAD_DOWN) && dist_th[0] == 2) 
    {
        tail[1] = head[1];
        if (steps_dir == HEAD_UP)
            tail[0] = head[0]-1;
        else
            tail[0] = head[0]+1;
    } else if ((steps_dir == HEAD_LEFT || steps_dir == HEAD_RIGHT) && dist_th[1] == 2) {
        tail[0] = head[0];
        if (steps_dir == HEAD_LEFT)
            tail[1] = head[1]+1;
        else
            tail[1] = head[1]-1;
    }
    
    printf("tail: (y: %d, x: %d), head: (y: %d, x: %d)\n", tail[0], tail[1], head[0], head[1]);
}

int main()
{
    FILE* file = fopen("input.txt", "r");
    if (!file)
    {
        puts("ERR: fopen()");
        exit(1);
    }

    char buf[128];
    int* head = calloc(sizeof(int), 2);
    int* tail = calloc(sizeof(int), 2);
    history* hst = calloc(sizeof(history), 1);

    while (fgets(buf, sizeof(buf), file))
    {
        char steps_dir = '\0';
        unsigned int steps_amount = 0;

        sscanf(buf, "%c %u", &steps_dir, &steps_amount);
        if (!steps_dir || !steps_amount)
        {
            puts("ERR: invalid input");
            exit(1);
        }

        if (steps_dir == 'U')
            steps_dir = HEAD_UP;
        else if (steps_dir == 'R')
            steps_dir = HEAD_RIGHT;
        else if (steps_dir == 'D')
            steps_dir = HEAD_DOWN;
        else if (steps_dir == 'L')
            steps_dir = HEAD_LEFT;

        for (int i=0; i < steps_amount; i++)
        {
            tick(tail, head, steps_dir);
            history_add(hst, tail);
        }
    }

    printf("positions: %d\n", hst->len);
}
