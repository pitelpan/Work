/* if_02.c: Deixnei tin xrisi tis if..else */
#include <stdio.h>

int main()
{
    int x;

    printf("Dwse enan akeraio: ");
    scanf("%d", &x);

    if(x%2==0)
        printf("O arithmos einai artios!");
    else
        printf("O arithmos einai perittos!");

    return 0;
}