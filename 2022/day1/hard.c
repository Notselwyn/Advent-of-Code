#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int* calories;
    int capacity;
    int len;
} list;

void list_push(list* lst, int value)
{
    if (lst->capacity == lst->len)
    {
        lst->capacity = lst->capacity == 0 ? 8 : lst->capacity * 2;
        lst->calories = realloc(lst->calories, lst->capacity*sizeof(int));
        if (lst->calories == NULL)
            exit(1);
    }

    lst->calories[lst->len++] = value; 
}


//int is_bigger(int* x, int* y)
int is_bigger(const void* x, const void* y)
{
    return *(int*)x < *(int*)y; 
}


int main()
{
    FILE* file = fopen("input.txt", "r");
    char calories[8];
    
    list lst = { 0 };

    list_push(&lst, 0);
    while (fgets(calories, sizeof(calories), file)) 
    {
        if (calories[0] == '\n')
        {
            printf("%d\n", lst.calories[lst.len-1]);
            list_push(&lst, 0);
        }
        
        lst.calories[lst.len-1] += strtol(calories, NULL, 10);
    }

    qsort(lst.calories, lst.len, sizeof(int), is_bigger);

    int sum = 0;
    for (int i=0; i < 3; i++)
        sum += lst.calories[i];

    printf("------\nsum: %d\n", sum);
}
