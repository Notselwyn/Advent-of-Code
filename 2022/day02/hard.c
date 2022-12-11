// Rock Paper Scissors
#include <stdio.h>


int main()
{
    FILE* file = fopen("input.txt", "r");
    char line[5];  // line[3] == \x0a and line[4] == 0

    int score = 0;
    while (fgets(line, 5, file)) 
    {
        int p1 = line[0] - 64;
        int outcome = ((line[2]*2)%3+2)%3+1;  // X, Y, Z -> 0, 1, 2
        int p2 = (3 + p1 - outcome)%3+1;  // basically outcome = (3 + p1 - p2) % 3

        // round score + move score
        score += (line[2]-1)%3*3 + p2;
    }

    printf("score: %d\n", score);
}
