/* sxesiakoi.c: Anadikniei tin leitourgia twn sxesiakwn telestwn */
#include <stdio.h>

int main()
{
    int x = 1;
    int y = 2;
    int z;
    printf("\nx=%d,y=%d", x, y);
    printf("\n=========");
    z = (x > y);
    printf("\nx>y:%d", z);

    z = (x >= y);
    printf("\nx>=y:%d", z);

    z = (x == y);
    printf("\nx==y:%d", z);

    z = (x <= y);
    printf("\nx<=y:%d", z);

    z = (x < y);
    printf("\nx<y:%d", z);

    return 0;
}