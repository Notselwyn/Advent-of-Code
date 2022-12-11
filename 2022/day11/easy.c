// Monkeys operate on how **worried** you are about the **item**
// Monkey properties:
// - Start items: pre- worry level for each item
// - Operation: operator for chaning worry level after inspection 
// - Test: expression to decide next monkey
// Items: FIFO
// Each turn:
// - FOR ITEM {
//  1. monkey.operation
//  2. worry //= 3
//  3. monkey tests and throws item
// }
// OUTPUT: monkey.max1st * monkey.max2nd after 20 rounds

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"  // stop gcc from whining about polymorphism xd

// dynamic array
typedef struct {
    void** content;
    unsigned int nmemb;
    unsigned int typesize;
    unsigned int capacity;
} dynarr;


// all test cases are divisable
typedef struct {
    int arg;
    unsigned int success;
    unsigned int failure;
} testcase;


// max positive integer
int ARG_OLD_VALUE = 0x7fffffff;

// e.g. new = old * 19, where * is op. and 19 is op. arg2
typedef struct {
    char operator;
    int arg1;
    int arg2;
} operation;


typedef struct {
    dynarr items;  // integers. (dynarr for polymorphism)
    unsigned int item_counter;
    unsigned int index;
    operation op;
    testcase test;
} monkey; 


dynarr* dynarr_init(unsigned int typesize)
{
    dynarr* dnar = calloc(sizeof(dynarr), 1);
    dnar->typesize = typesize;
    
    return dnar;
}


monkey* monkey_init()
{
    monkey* mnk = calloc(sizeof(monkey), 1);
    mnk->items.typesize = sizeof(int);

    return mnk;
}


dynarr* dynarr_append(dynarr* dnar, void* entry)
{
    //printf("adding %p to monkey %p\n", entry, dnar);
    if (dnar->nmemb == dnar->capacity)
    {
        dnar->capacity = dnar->capacity ? dnar->capacity * 2 : 8;
        dnar->content = realloc(dnar->content, dnar->capacity * dnar->typesize);
        if (!dnar->content)
        {
            puts("ERR: realloc failed");
            exit(1);
        }
    }

    //void* content_entry = dnar->content + dnar->nmemb * dnar->typesize;
    //printf("copying monke to %p (size: %u)\n", content_entry, dnar->typesize);
    //memcpy(content_entry, entry, dnar->typesize);
    

    dnar->content[dnar->nmemb++] = entry;
    return dnar;
}


#define DYNARR_INDEX(dnar, index) ((dynarr*)dnar)->content[index]
#define DYNARR_LEN(dnar) ((dynarr*)dnar)->nmemb


void monkey_inspect(monkey*** tribe, monkey* mnk, int item, int round)
{
    printf("%d->", item);
    mnk->item_counter++;
    
    int arg1 = mnk->op.arg1 == ARG_OLD_VALUE ? item : mnk->op.arg1;
    int arg2 = mnk->op.arg2 == ARG_OLD_VALUE ? item : mnk->op.arg2;
    int worry;
    switch (mnk->op.operator) {
        case '+':
            worry = arg1 + arg2;
            break;
        case '-':
            worry = arg1 - arg2;
            break;
        case '*':
            worry = arg1 * arg2;
            break;
        case '/':
            worry = arg1 / arg2;  // this should get floored to int
            break;
        default:
            puts("ERR: invalid operator");
            exit(1);
    }
        
    worry /= 3;  // this should get floored to int as well
    

    monkey* mnk_next;
    if (worry % mnk->test.arg == 0)  // successfull test result
        mnk_next = (*tribe)[mnk->test.success];
    else
        mnk_next = (*tribe)[mnk->test.failure];

    // only increment round when all monkeys passed
    if (mnk_next->index < mnk->index)
        round++;

    if (round < 20)
        monkey_inspect(tribe, mnk_next, worry, round);
}


monkey* monkey_inspect_all(monkey*** tribe, monkey* mnk)
{
    // iterate over all items
    printf("[ ");
    for (int k=0; k < DYNARR_LEN(mnk); k++)
    {
        int old_value = *(int*)DYNARR_INDEX(mnk, k);
        printf("%d:", old_value);
        
        int arg1 = mnk->op.arg1 == ARG_OLD_VALUE ? old_value : mnk->op.arg1;
        int arg2 = mnk->op.arg2 == ARG_OLD_VALUE ? old_value : mnk->op.arg2;
        int* worry = malloc(sizeof(int));
        switch (mnk->op.operator) {
            case '+':
                *worry = arg1 + arg2;
                break;
            case '-':
                *worry = arg1 - arg2;
                break;
            case '*':
                *worry = arg1 * arg2;
                break;
            case '/':
                *worry = arg1 / arg2;  // this should get floored to int
                break;
            default:
                puts("ERR: invalid operator");
                exit(1);
        }
        
        *worry /= 3;  // this should get floored to int as well
        monkey* mnk_next;
        if (*worry % mnk->test.arg == 0) // successfull result
            mnk_next = (*tribe)[mnk->test.success];
        else
            mnk_next = (*tribe)[mnk->test.failure];
        dynarr_append(mnk_next, worry);
        printf("%d ", *worry);
    }
    puts("]");
    
    return mnk;
}

int main()
{
    FILE* file = fopen("input2.txt", "r");
    if (!file)
    {
        puts("ERR: fopen()");
        exit(1);
    }

    char buf[128];
    monkey*** tribe = dynarr_init(sizeof(monkey*)); // lmao
    int i=0;
    
    monkey* mnk = NULL;
    while (fgets(buf, sizeof(buf), file))
    {
        switch (i % 7)
        {
            case 0:
                if (i != 0)  // i % 7 == 0 && i != 0
                    dynarr_append(tribe, mnk);

                printf("dynarr size: %d\n", ((dynarr*)tribe)->nmemb);
                mnk = monkey_init();
                mnk->index = i / 7;
                break;
            case 1:  // starting items
                int buflen = strlen(buf);
                int start = 0;
                for (int i=16; i < buflen; i++)
                {
                    if (buf[i] == ' ') {
                        start = i+1;
                    } else if (buf[i] == ',' || buf[i] == '\n') {
                        // using malloc for 4 bytes is inefficient
                        // since the minimal malloc size is 24
                        int* value = malloc(sizeof(int));
                        buf[i] = 0;
                        sscanf(buf+start, "%d", value);
                        dynarr_append(mnk, value);
                    }
                }
                break;
            case 2:
                if (!memcmp(buf+25, "old", 3))
                {
                    sscanf(buf, "  Operation: new = old %c", &mnk->op.operator);
                    mnk->op.arg2 = ARG_OLD_VALUE;
                } else {
                    sscanf(buf, "  Operation: new = old %c %d", &mnk->op.operator, &mnk->op.arg2);
                }
                mnk->op.arg1 = ARG_OLD_VALUE;
                break;
            case 3:
                sscanf(buf, "  Test: divisible by %d", &mnk->test.arg);
                break;
            case 4:
                sscanf(buf, "    If true: throw to monkey %d", &mnk->test.success);
                break;
            case 5:
                sscanf(buf, "    If false: throw to monkey %d", &mnk->test.failure);
                break;
        }
        i++;
    }

    if (!mnk)
    {
        puts("ERR: failed to interpret monke");
        exit(1);
    }
   
    // for the last monke 
    dynarr_append(tribe, mnk);
    
    for (int j=0; j < DYNARR_LEN(tribe); j++)
    {

        monkey* mnk = (*tribe)[j];
       
        printf("===== MONKEY %d (%d) =====\n", j, DYNARR_LEN(mnk));
        for (int k=0; k < DYNARR_LEN(mnk); k++)
        {
            printf("----- ITEM %d -----\n", k);
            monkey_inspect(tribe, mnk, *(int*)DYNARR_INDEX(mnk, k), 0);
            puts("");
        }
    }

    for (int i=0; i < DYNARR_LEN(tribe); i++)
    {
        printf("monkey %d: %d items\n", i, (*tribe)[i]->item_counter);

    }
}


