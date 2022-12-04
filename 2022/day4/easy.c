// **Section** has **unique Section ID**
// **Elf** has multiple **Section IDs**
// List of section ranges per **2 Elf pair**
// Find out where (A contains B) || (B contains A)
// - (As > Bs && Ae < Be) || (Bs > As && Be < Ae)


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE* file = fopen("input.txt", "r");
    char buf[13]; // 11 chars + \n + \x00
    int contain_counter = 0;

    while (fgets(buf, sizeof(buf), file))
    {
        unsigned char sections[4] = { 0 };
        unsigned char section_i = 0;
        unsigned char start = 0;
        const unsigned char buflen = strlen(buf);
        
        for (int i=0; i < buflen; i++)
        {
            if ((buf[i] == '-') || (buf[i] == ',') || i == buflen-1) {
                buf[i] = 0;
                sections[section_i++] = strtol(&buf[start], NULL, 10);
                start = i+1;
            }
        }

        printf("%d-%d,%d-%d\n", sections[0], sections[1], sections[2], sections[3]);
        if ((sections[0] >= sections[2]) && (sections[1] <= sections[3])) {
            puts("B contains A");
            contain_counter++;
        } else if ((sections[2] >= sections[0]) && (sections[3] <= sections[1])) {
            puts("A contains B");
            contain_counter++;
        } else {
            puts("doesn't contain");
        } 
    }

    printf("countain counter: %d", contain_counter);
}
