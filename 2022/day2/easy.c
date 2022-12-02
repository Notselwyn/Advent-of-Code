// Rock Paper Scissors
// 
// A/X, B/Y, C/Z : Rock, Paper, Scissors
// total score = sum([round.shape + round.outcome for round in rounds])
// - shape score: 1, 2, 3 : Rock, Paper Scissors
// - outcome score: 0, 3, 6 : lost, draw, won
// input are rounds
// - s = {"A X": 4, "A Y": 8, "A Z": 3, "B X": 1, "B Y": 5, "B Z": 9, "C X": 7, "C Y": 2, "C Z": 6}
// - print(sum(s[l.strip()] for l in f.readlines()))

#include <stdio.h>


int main()
{
    FILE* file = fopen("input.txt", "r");
    char line[5];

    int score = 0;
    while (fgets(line, 5, file)) 
    {
        int oscore = score;

        // line[3] is a newline. prefer using size 5 over 4 because 
        // it saves an if statement and twice the fgets() calls
        line[3] = '\0';  
        
        int p1 = line[0] - 64;
        int p2 = line[2] - 87;
        int outcome = (3 + p1 - p2) % 3;
        
        // round score + move score
        score += ((outcome*2)%3+1)%3*3 + p2; 

        printf("%s = %d (%d)\n", line, score-oscore, score);
    }

    printf("%d\n", score);
}
