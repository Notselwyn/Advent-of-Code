#include <stdio.h>
#include <math.h>
#include <string.h>

int main()
{
    FILE* file = fopen("input.txt", "r");
    char buf[256];
    int sum = 0;
    unsigned long bits = 0xffffffffffffffff; // 2**64-1

    for (int i=1; fgets(buf, sizeof(buf), file); i++)
    {
        unsigned long rucksack = 0;
        for (int j=0; j < strlen(buf)-1; j++)
        {
            // each character get a bit
            rucksack |= (unsigned long)pow(2, buf[j]-65);
        }

        bits &= rucksack;
        if (i % 3 == 0)
        {
            char dup = 65 + log2(bits) / log2(2); // 65 + logbase(2, comp)
            int incr = dup&31;  // get priority
            if (!(dup & 32)) // is upper
                incr += 26;

            sum += incr;
            printf("%c (%d)\n", dup, incr);
        
            bits = 0xffffffffffffffff;
        }
    }

    printf("sum: %d\n", sum);
}
