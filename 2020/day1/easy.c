#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


typedef struct {
    int* numbers;
    int capacity;
    int len;
} list;

void list_push(list* lst, int value)
{
    if (lst->capacity == lst->len)
    {
        lst->capacity = lst->capacity ? lst->capacity * 2 : 8;
        lst->numbers = realloc(lst->numbers, lst->capacity*sizeof(int));
        if (lst->numbers == NULL)
            exit(1); 
    }

    lst->numbers[lst->len++] = value;
}


int is_smaller(const void* x, const void* y)
{
    return *(int*)x < *(int*)y;
}

int main()
{
    char line[5] = { 0 };
    
    FILE* file = fopen("input.txt", "r");
    if (errno != 0)  // use errno.h
        exit(1);

    list* lst = calloc(sizeof(lst), 1);

    while (fgets(line, sizeof(line), file))
    {
        int n = strtol(line, NULL, 10);
        if (n == 0) continue;

        list_push(lst, n);
        memset(line, 0, 5);
    }

    qsort(lst->numbers, lst->len, sizeof(int), is_smaller);
    
    int min = 0;
    int max = lst->len-1;
    int sum = 0;
    while (sum != 2020)
    {
        sum = lst->numbers[min] + lst->numbers[max];
        if (sum > 2020 && min++ == lst->len-1) exit(1);
        else if (sum < 2020 && max-- == 1) exit(1);

        printf("%d + %d = %d (mult %d)\n", lst->numbers[min], lst->numbers[max], sum, lst->numbers[min] * lst->numbers[max]);
    }
}
