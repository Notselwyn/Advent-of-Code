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



#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int cycle_check(unsigned int cycle, int cycle_incr, int reg_new, int reg_old)
{
    for (int i=1; i <= cycle_incr; i++)
        if ((cycle + i + 20) % 40 == 0)
            return (i == cycle_incr ? reg_new : reg_old) * (cycle+i);

    return 0;
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
    unsigned int cycle = 1;
    unsigned int cycle_sum = 0;
    while (fgets(buf, sizeof(buf), file))
    {
        int cycle_incr = 0;
        if (!memcmp(buf, "addx ", 5))
        {
            int value = 0;
            int reg_x_old = reg_x;
            
            sscanf(buf, "addx %d", &value);
            reg_x += value;

            cycle_incr = 2;
            cycle_sum += cycle_check(cycle, cycle_incr, reg_x, reg_x_old);
        } else {
            cycle_incr = 1;
            cycle_sum += cycle_check(cycle, cycle_incr, reg_x, reg_x);
        }

        cycle += cycle_incr;
        printf("cycle %d: %d\n", cycle, reg_x);
    }

    printf("cycle sum: %d\n", cycle_sum);
}
