// Each rucksack has 2 **compartments**
// - First half of characters is comp. 1, second half is comp. 2 
// Each **item** sorted into 1 compartment
// - Input failed to follow for **1 item** per **sucksack**
// Each **item type** by lower- and uppercase
// Give sum(alp.find(dchar)*(1+dchar.isupper()))

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


int main()
{
    FILE* file = fopen("input.txt", "r");
    char rucksack[256];
    int sum = 0;

    while (fgets(rucksack, sizeof(rucksack), file))
    {
        // we are given chars a-zA-Z, aka 2*26 = 52
        // we use a 64-bit long like 64 booleans
        
        // ord("A")-65 == 0
        // ord("z")-65 == 57

        unsigned long comp1 = 0;
        rucksack[strlen(rucksack)-1] = '\0';

        int size_half = strlen(rucksack)/2;
        bool found = false;
        for (int i=0; i<size_half*2 && !found; i++)
        {
            unsigned long encoded = pow(2, rucksack[i]-65);
            if (i < size_half)
                comp1 |= encoded;
            else if (comp1 & encoded) {
                found = true;
                int incr = rucksack[i]&31;  // get priority  
                if (!(rucksack[i] & 32)) // is upper
                    incr += 26;

                sum += incr;
                printf("%c (%d)\n", rucksack[i], incr);
            }
        }
    }
    // 6437 < N < 8385 
    printf("sum: %d\n", sum);
}
