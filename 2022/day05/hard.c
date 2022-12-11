// stack moves retain their order (FIFO) 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char* content;
    int len;
    int _capacity;
} stack;

stack* stack_push(stack* stck, const char value)
{
    if (stck->len == stck->_capacity)
    {
        if (stck->_capacity * 2 < stck->len)  // prevent integer overflow
            return NULL;

        stck->_capacity = stck->_capacity ? stck->_capacity * 2 : 8;
        stck->content = realloc(stck->content, stck->_capacity);
        if (stck->content == NULL)
            return NULL;
    }

    stck->content[stck->len++] = value;
    printf("(push) stck->content[%d]: %c\n", stck->len-1, stck->content[stck->len-1]);
    return stck;
}


stack* stack_reverse(stack* stck)
{
    for (int i=0; i < stck->len / 2; i++)
    {
        char swp = stck->content[stck->len-i-1];
        stck->content[stck->len-i-1] = stck->content[i];
        stck->content[i] = swp;
    }

    return stck;
}

stack* stack_move(stack* origin, stack* new, const unsigned int amount)
{
    if (origin->len < amount)
        return NULL;
   
    for (int i=0; i < amount; i++)
    {
        char crate = origin->content[origin->len-amount+i];
        stack_push(new, crate);
    }

    origin->len -= amount;
    return new;
}


int main()
{
    FILE* file = fopen("input.txt", "r");
    if (file == NULL)
    {
        puts("ERR: fopen()");
        exit(1);
    }

    char buf[128];
    stack* stacks[9] = { 0 };
    unsigned char is_parsing = 1;

    while (fgets(buf, sizeof(buf), file))
    {
        if (buf[0] == '\n') {  // ends parsing input stack
            is_parsing = 0;

            // reverse the stack in order to maintain O(1) stack pushing/popping
            for (int i=0; i < 9; i++)
                stack_reverse(stacks[i]);
        } else if (buf[0] == 'm' && buf[1] == 'o') {
            unsigned short amount;
            unsigned short from;
            unsigned short to;

            sscanf(buf, "move %hu from %hu to %hu\n", &amount, &from, &to);
            printf("moving %hu stacks from %hu to %hu\n", amount, from, to);
            
            if (!stack_move(stacks[from-1], stacks[to-1], amount))
            {
                puts("ERR: stack_move()");
                exit(1);
            }

            // display stack contents to stdout
            for (int i=0; i < 9; i++)
            {
                printf("%d. [ ", i+1);
                for (int j=0; j < stacks[i]->len; j++)
                    printf("%c ", stacks[i]->content[j]);
                printf("] (%d)\n", stacks[i]->len);
            }
        } else if (is_parsing == 1) {  
            // parse stack input
            for (int j=0; j < 9; j++)
            {
                if (buf[1+j*4] == ' ')
                    continue;

                if (stacks[j] == NULL)
                    stacks[j] = calloc(sizeof(stack), 1);

                stack_push(stacks[j], buf[1+j*4]);
            }
        }
    }

    puts("------------");
    for (int i=0; i < 9; i++)
    {
        printf("%c", stacks[i]->content[stacks[i]->len-1]);
    }
    puts("");
}
