/* Multiply.c: programma gia ton ipologismo tou ginomenou dio arithmwn */

#include <stdio.h>

int a,b,c;

int ginomeno(int x, int y);

int main()
{
    /* Eisodos protou arithmou */
    printf("Eisagete arithmo metaksi 1 kai 100:");
    scanf("%d", &a);

    /* Eisodos deuterou arithmou */
    printf("Eisagete arithmo metaksi 1 kai 100:");
    scanf("%d", &b);

    c=ginomeno(a,b);
    printf("To ginomeno tou %d kai tou %d einai: %d\n",a,b,c);

    return 0;
}

/* H Synartisi  epistrefei to ginomeno twn dio orismatwn tis*/

int ginomeno(int x, int y)
{
    return (x*y);
}