#include "funct_test.h"

int funct()
{
    return 99;
}

int main()
{
    double a;

    a = funct();

    printf("Hello Funct Test: %4.1f \n", a);

    system("pause"); 
    return 0;
}
