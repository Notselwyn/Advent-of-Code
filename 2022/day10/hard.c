// **clock circuit** runs on **cycles**
// CPU register X
// Signal strength = cycle * Vx (range(20, 220, 40))
// Instruction set:
// - addx V     ; add V to X
// - noop       ; nop
// X is center of 3-pixel sprite
// draw pixel each cycle
// CRT width starts at 0 and ends in 39. 6 rows
// If sprite atleast 1 pixel drawn, use '#' if not drawn use '.'
// if (cycle == reg_x-1 || cycle == reg_x || cycle == reg_x+1)
// - visible = true



#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int cycle_check(unsigned int cycle, int reg)
{
    if (cycle == reg-1 || cycle == reg || cycle == reg+1)
        return '#';

    return '.';
}

        
int main()
{
    FILE* file = fopen("input.txt", "r");
    if (!file)
    {
        puts("ERR: fopen()");
        exit(1);
    }


    char buf[128];
    int reg_x = 1;
    unsigned int cycle = 0;
    char line[40];
    while (fgets(buf, sizeof(buf), file))
    {
        if (!memcmp(buf, "addx ", 5))
        {
            int value = 0;
            
            sscanf(buf, "addx %d", &value);
            for (int i=0; i < 2; i++)
            {
                line[cycle] = cycle_check(cycle, reg_x);
                cycle = (cycle+1) % 40;
            }

            reg_x += value;
        } else {
            line[cycle] = cycle_check(cycle, reg_x);
            cycle = (cycle+1) % 40;
        }

        if (line[39] != 0)
        {
            puts(line);
            line[39] = 0;
        }
    }
}
