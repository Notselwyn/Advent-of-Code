// Setup **communication system**
// Lock on their signal
// Add subroutine to detect **start-of-packet marker** (4 unique chars)
// Find where the 4th character sequence is unique 

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int has_duplicate(char* charr)
{
    for (int i=0; i < 4; i++)
    {
        for (int k=i+1; k < 4; k++)
        {
            if (charr[i] == charr[k])
                return 1;
        }
    }

    return 0;
}


int main()
{
    char buf[128];
    int mask = 0;
    int sum = 0;
    FILE* file = fopen("input.txt", "r");
    if (!file)
    {
        puts("ERR: failed to fopen()");
        exit(1);
    }

    while (fgets(buf, sizeof(buf), file))
    {
        for (int i=0; i < strlen(buf); i++)
        {
            mask <<= 8;
            mask |= buf[i];
            
            printf("%x (%d)\n", mask, sum+i+1);
            if (i >= 3 && !has_duplicate((char*)&mask))
            {
                // wrong: 64 63
                printf("marker: %d\n", sum+i+1);
                exit(1);
            }

        }
        sum += 127;  // sizeof(): 128 includes nullbyte
        puts("--------------");
    }
}
