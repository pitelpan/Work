/* askisi_if_1.c */

#include <stdio.h>

int main()
{
    int age;

    printf("Dwste tin ilikia sas: ");
    scanf("%d", &age);

    if(age<18)
        printf("\nEisai anilikos.");
    else if(age<=65)
        printf("\nEisai enilikas.");
    else
        printf("\nEisai sintaxiouxos.");
    
    return 0;
}