#include <stdio.h>

int main()
{
    int x;
    int y;

    printf("Dwse ton 1o arithmo: ");
    scanf("%d", &x);
    printf("Dwse ton 2o arithmo:");
    scanf("%d", &y);

    if(x>y)
        printf("\nO 1os arithmos einai megaluteros tou 2ou");
    else
        printf("\nO 2os arithmos einai megaluteros tou 1ou");
    
    return 0;
}