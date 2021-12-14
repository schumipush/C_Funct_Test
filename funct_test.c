#include "funct_test.h"


int main()
{
    int a;

    a = funct();

    printf("Hello Funct Test: %4.1f \n", a);

    a=1;
    switch (a){
        case 1:{
            int * p = &a;
            printf("%d\n",p);
            printf("%d\n",p[0]);
            printf("%d\n",p[3]);
        }
    }
    int a1[] = {1,2,3,4,5,};
    int *p = a1;
    int *q = &a1[5];
    printf("%d", q-p);



    

    system("pause"); 
    return 0;
}
