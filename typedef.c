/* typedef.c: Paradeigma xrisis tis entolis typedef */

#include <stdio.h>

typedef int akeraios;

int main()
{
    akeraios x,y,z;

    printf("Dwste enan akeraio: ");
    scanf("%d", &x);
    printf("Dwse akoma enan akeraio: ");
    scanf("%d", &y);
    z = x + y;
    printf("To athroisma einai: %d", z);

    return 0;
}