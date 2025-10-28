/* if_01.c: Deixnei tin xrisi tis if */
#include<stdio.h>

int main()
{
    int x;
    int y;

    printf("Dwste ton akeraio arithmo x: ");
    scanf("%d", &x);
    printf("Dwste ton akeraio arithmo y: ");
    scanf("%d", &y);

    if(x<y)
        printf("\n%d < %d",x,y);
    
    if(x<=y)
        printf("\n%d <= %d",x,y);
    
    if(x==y)
        printf("\n%d == %d",x,y);

    if(x>y)
        printf("\n%d > %d",x,y);
    
    if(x>=y)
        printf("\n%d >= %d",x,y);

    return 0;

}