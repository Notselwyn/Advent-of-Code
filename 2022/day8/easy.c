// INPUT: tree map of **tree height**
// Algorithm:
// 1. is there enough tree cover to keep it hidden
// - count trees outside of grid.
// 2. height
// - visible if:
// -- all trees to grid are shorter
// -- up, right, down, lef
// OUTPUT: how many trees are visible from outside the grid?
//
// ALGO:
// - height = width (square)
// - for i, height in enumerate(line): height-width, height-0, et cetera. 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    unsigned char* heights;
    int len;
    int capacity;
} forest;


forest* forest_init()
{
    forest* frst = calloc(sizeof(forest), 1);
    return frst;
}


forest* forest_push(forest* frst, unsigned char height)
{
    if (frst->len == frst->capacity)
    {
        frst->capacity = frst->capacity ? frst->capacity * 2 : 8;
        frst->heights = realloc(frst->heights, frst->capacity);
        if (frst->heights == NULL)
        {
            puts("ERR: realloc failed");
            exit(1);
        }
    }

    frst->heights[frst->len++] = height;
    return frst;
}


int forest_is_visible(forest* frst, int index)
{
    int width = sqrt(frst->len);
    int x = index % width;
    int y = index / width;
    int height = frst->heights[index];

    int is_visible = 1; // sides on which 
    for (int i=x-1; i >= 0; i--)
        if (height <= frst->heights[y * width + i])
            is_visible = 0;
    
    if (is_visible)
        return 1;

    is_visible = 1;
    for (int i=x+1; i < width; i++)
        if (height <= frst->heights[y * width + i])
            is_visible = 0;

    if (is_visible)
        return 1;

    is_visible = 1;
    for (int i=y-1; i >= 0; i--)
        if (height <= frst->heights[i * width + x])
            is_visible = 0;

    if (is_visible)
        return 1;

    is_visible = 1; 
    for (int i=y+1; i < width; i++)
        if (height <= frst->heights[i * width + x])
            is_visible = 0;

    if (is_visible)
        return 1;

    return 0;
}




int main()
{
    FILE* file = fopen("input.txt", "r");
    if (!file)
    {
        puts("ERR: fopen()");
        exit(1);
    }

    forest* frst = forest_init();

    // y, x
    int index[2] = { 0 };
    char line[128];
    while (fgets(line, sizeof(line), file))
    {
        int line_len = strlen(line)-1; // exclude newline
        for (index[1] = 0; index[1] < line_len; index[1]++)
        {
            unsigned char height = line[index[1]]-48; // convert ascii to int
            forest_push(frst, height); 
        }
        
        index[0]++;
    }


    int width = sqrt(frst->len);
    printf("forest size: %d, width: %d\n", frst->len, width); 
    int count_visible = 4*width-4;
    
    for (int y=1; y < width-1; y++)
    {
        for (int x=1; x < width-1; x++)
        {
            int index = y * width + x;
            unsigned char height = frst->heights[index];
            if (forest_is_visible(frst, index))
                count_visible++;
        }
    }

    printf("visible: %d\n", count_visible); 
}
