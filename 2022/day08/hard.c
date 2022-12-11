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


unsigned long forest_scenic_score(forest* frst, int index)
{
    int width = sqrt(frst->len);
    int x = index % width;
    int y = index / width;
    int height = frst->heights[index];

    unsigned long scenic_score = 1; // 1 so we can multiply
    for (int i=x-1; i >= 0; i--)
    {
        if (height <= frst->heights[y * width + i] || i == 0)
        {
            scenic_score *= x-i;
            break;
        }
    }

    for (int i=x+1; i < width; i++)
    {
        if (height <= frst->heights[y * width + i] || i == width-1)
        {
            scenic_score *= i-x;
            break;
        }
    }
    
    for (int i=y-1; i >= 0; i--)
    {
        if (height <= frst->heights[i * width + x] || i == 0)
        {
            scenic_score *= y-i;
            break;
        }
    }


    for (int i=y+1; i < width; i++)
    {
        if (height <= frst->heights[i * width + x] || i == width-1)
        {
            scenic_score *= i-y;
            break;
        }
    }

    return scenic_score;
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

    const int width = sqrt(frst->len);
    unsigned long best_score = 0;
    
    printf("forest size: %d, width: %d\n", frst->len, width); 
    for (int y=1; y < width-1; y++)
    {
        for (int x=1; x < width-1; x++)
        {
            const int index = y * width + x;
            unsigned char height = frst->heights[index];
            unsigned long scenic_score = forest_scenic_score(frst, index);
            
            printf("scenic score: %d\n", scenic_score); 
            if (scenic_score > best_score)
                best_score = scenic_score;
        }
    }

    printf("---------\nbest: %lu\n", best_score);
}
