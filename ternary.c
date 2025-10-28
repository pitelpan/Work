/* ternary.c: Ekseruna to telesti ?: */
#include <stdio.h>

int main()
{
    int x,y,z;

    /* kai i if epistrefei mia timi */
    x=0; y=1;
    if(x<y) z=0;
    else z=1;
    printf("%d",z);

    /* Isodynamosd tropos me ton telesti ?: */
    x=1; y=0;
    z = x<y ? 0 : 1;
    printf("%d",z);

    /* kai mporoume na to xrisimopoiisoume ws orisma. */
    printf("%d",x<y?0:1);

    return 0;
}