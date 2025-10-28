#include <stdio.h>

void main()
{
    int x,y,z;

    printf("Dwse ton 1o arithmo: ");
    scanf("%d", &x);
    printf("Dwse ton 2o arithmo: ");
    scanf("%d", &y);
    printf("Dwse ton 3o arithmo: ");
    scanf("%d", &z);

    if(x>y && x>z)
        printf("\nO 1os arithmos einai o megaliteros.");
    else if(y>x && y>z)
        printf("\nO 2os arithmos einai o megaliteros.");
    else
        printf("\nO 3os arithmos einai o megaliteros.");
}