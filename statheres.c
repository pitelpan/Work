/* statheres.c: Programma pou deixnei tin xrisi statherwn */

#include <stdio.h>

#define N 100

int main()
{
    int i, sum;
    const int number = 10;

    sum = 0;
    for (i = number; i <= N; i++)
        sum =sum + i;

    printf("To athroisma twn arithmwn [%d..%d] einai %d", number, N, sum);

    return 0;
}