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
#include <limits.h>


// dynamic array
typedef struct dynarr_t {
    void** content;
    size_t nmemb;
    size_t typesize;
    size_t capacity;
} dynarr_t;


// all test cases are divisable
struct testcase {
    int arg;
    unsigned int success;
    unsigned int failure;
};


// max positive integer
int ARG_OLD_VALUE = INT_MAX;

// e.g. new = old * 19, where * is op. and 19 is op. arg2
struct operation {
    char operator;
    int arg1;
    int arg2;
};


typedef struct monkey_t {
    dynarr_t items;  // integers. (dynarr for polymorphism)
    unsigned int item_counter;
    unsigned int index;
    struct operation op;
    struct testcase test;
} monkey_t; 


dynarr_t* dynarr_init(unsigned int typesize)
{
    dynarr_t* dnar;
   
    dnar = malloc(sizeof(dynarr_t));
    memset(dnar, '\0', typesize);

    dnar->typesize = typesize;
    
    return dnar;
}


dynarr_t* dynarr_append(dynarr_t* dnar, void* entry)
{
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

    dnar->content[dnar->nmemb++] = entry;
    return dnar;
}


#define DYNARR_INDEX(dnar, index) (((dynarr_t*)dnar)->content[index])
#define DYNARR_LEN(dnar) (((dynarr_t*)dnar)->nmemb)


monkey_t* monkey_init()
{
    monkey_t* mnk;
   
    mnk = malloc(sizeof(monkey_t));
    memset(mnk, '\0', sizeof(monkey_t));

    mnk->items.typesize = sizeof(int*);

    return mnk;
}


void monkey_inspect(monkey_t*** tribe, monkey_t* mnk, int item, int round)
{
    while (round < 20)
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
        
        monkey_t* mnk_next;
        if (worry % mnk->test.arg == 0)  // successfull test result
            mnk_next = (*tribe)[mnk->test.success];
        else
            mnk_next = (*tribe)[mnk->test.failure];

        // only increment round when all monkeys passed
        if (mnk_next->index < mnk->index)
            round++;

        mnk = mnk_next;
        item = worry;
    }
}


monkey_t*** file_parse(FILE* file)
{
    char buf[128];
    monkey_t*** tribe = (monkey_t***)dynarr_init(sizeof(monkey_t*)); // lmao
    
    int i=0;
    monkey_t* mnk = NULL;
    while (fgets(buf, sizeof(buf), file))
    {
        switch (i % 7)
        {
        case 0:
            if (i != 0)  // i % 7 == 0 && i != 0
                dynarr_append((dynarr_t*)tribe, mnk);

            printf("tribe size: %ld\n", ((dynarr_t*)tribe)->nmemb);
            mnk = monkey_init();
            mnk->index = i / 7;
            break;
        case 1:  // starting items
            for (int i=16; i < strlen(buf); i++)
            {
                if (buf[i] == ' ') {
                    // using malloc for 4 bytes is inefficient
                    // since the minimal malloc size is 24
                    int* value = malloc(sizeof(int));
                    *value = strtol(buf+i, NULL, 10);
                    dynarr_append((dynarr_t*)mnk, value);
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
    dynarr_append((dynarr_t*)tribe, mnk);
    return tribe;
}


int main()
{
    FILE* file = fopen("input3.txt", "r");
    if (!file)
    {
        puts("ERR: fopen()");
        exit(1);
    }

    monkey_t*** tribe = file_parse(file);
    for (int i=0; i < DYNARR_LEN(tribe); i++)
    {
        monkey_t* mnk = (*tribe)[i];
       
        printf("===== MONKEY %d (%ld items) =====\n", i, DYNARR_LEN(mnk));
        for (int j=0; j < DYNARR_LEN(mnk); j++)
        {
            printf("----- ITEM %d (%d value) -----\n", j, *(int*)DYNARR_INDEX(mnk, j));
            monkey_inspect(tribe, mnk, *(int*)DYNARR_INDEX(mnk, j), 0);
            puts("");
        }
    }

    unsigned int best = 0;
    unsigned int best2nd = 0;
    for (int i=0; i < DYNARR_LEN(tribe); i++)
    {
        int items = (*tribe)[i]->item_counter;
        if (items > best) {
            best2nd = best;
            best = items;
        } else if (items > best2nd) {
            best2nd = items;
        }
        printf("monkey business: %d * %d = %d\n", best, best2nd, best * best2nd);
    }
}


