#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    char content[256];
    unsigned int len;
    unsigned long bits;
} rucksack;


typedef struct {
    rucksack** rs;
    unsigned int len;
} group;


group* group_init(int size)
{
    group* grp = malloc(sizeof(group));
    grp->len = size;
    grp->rs = malloc(grp->len * 8);

    // allocate zeroed rucksack's
    for (int i=0; i < grp->len; i++)
        grp->rs[i] = calloc(sizeof(rucksack), 1);

    return grp;
}


group* group_read(group* grp, FILE* file)
{
    for (int i=0; i < grp->len; i++)
    {
        rucksack* rs = grp->rs[i];
        if (!fgets(rs->content, sizeof(rs->content), file))
            return NULL;
        
        // remove \n byte
        rs->len = strlen(rs->content)-1;
        rs->content[rs->len] = '\0';
    }

    return grp; 
}


group* group_init_bits(group* grp)
{
    for (int i=0; i < grp->len; i++)
    {
        rucksack* rs = grp->rs[i];
        rs->bits = 0;

        for (int i=0; i < rs->len; i++)
        {
            // each character get a bit
            rs->bits |= (unsigned long)pow(2, rs->content[i]-65);
        }
    }

    return grp;
}


char group_find_dup(group* grp)
{
    unsigned long comp = 0xffffffffffffffff;  // 2**64-1
    
    // strike out every character that's not in the bitfields
    for (int i=0; i < grp->len; i++)
        comp &= grp->rs[i]->bits;

    return 65 + log2(comp) / log2(2); // 65 + logbase(2, comp)
}


int main()
{
    FILE* file = fopen("input.txt", "r");
    group* grp = group_init(3);
    int sum = 0;

    while (group_read(grp, file))
    {
        group_init_bits(grp);
   
        char dup = group_find_dup(grp);
        int incr = dup&31;  // get priority
        if (!(dup & 32)) // is upper
            incr += 26;

        sum += incr;
        printf("%c (%d)\n", dup, incr);
    }

    printf("sum: %d\n", sum);
}
