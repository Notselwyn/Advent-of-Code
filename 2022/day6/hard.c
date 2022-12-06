// Setup **communication system**
// Lock on their signal
// Add subroutine to detect **start-of-packet marker** (4 unique chars)
// Find where the 4th character sequence is unique 

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int has_duplicate(char* charr, int size)
{
    for (int i=0; i < size; i++)
    {
        for (int k=i+1; k < size; k++)
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
    __int128 mask = 0; // 16 bytes (14 required)
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
            if (i >= 13 && !has_duplicate((char*)&mask, 14))
            {
                printf("marker: %d\n", sum+i+1);
                exit(1);
            }

        }
        sum += 127;  // sizeof(): 128 includes nullbyte
    }

    // more than 3400
}
