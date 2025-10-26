/* prakseis.c: Anadyknyei tin xrisi twn dilwsewn mathimatikwn praksewn */

#include <stdio.h>

int main()
{
    int x,y,z;
    int telestis;

    printf("\nDwse ton 1o akeraio arithmo: ");
    scanf("%d", &x);
    printf("\nDwse ton 2o akeraio arithmo: ");
    scanf("%d", &y);
    printf("\n Dwse ton telesti\n 0 gia +\n 1 gia -\n 2 gia *\n 3 gia /\n 4 gia %%: ");
    printf("\nEpilogi? ");
    scanf("%d", &telestis);

    if (telestis==0)
    {
        z=x+y;
        printf("%d + %d = %d", x, y, z);
    }
    else if (telestis==1)
    {
        z=x-y;
        printf("%d - %d = %d", x, y, z);
    }
    else if (telestis==2)
    {
        z=x*y;
        printf("%d * %d = %d", x, y, z);
    }
    else if (telestis==3)
    {
        z=x/y;
        printf("%d / %d = %d", x, y, z);
    }
    else if (telestis==4)
    {
        z=x%y;
        printf("%d %% %d = %d", x, y, z);
    }
    return 0;
}