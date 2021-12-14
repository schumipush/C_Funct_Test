#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    struct{int a; int b;}x={1,2},y={0};

    printf("原始值：\t%d,%d,%d,%d\n",x.a,x.b,y.a,y.b);

    y=x; // 证明可以直接把一个结构体赋值给另一个结构体
    printf("y=x:\t\t%d,%d,%d,%d\n",x.a,x.b,y.a,y.b);

    y.a=y.b=6;
    printf("y.a=y.b=6:\t%d,%d,%d,%d\n",x.a,x.b,y.a,y.b);


    return 0;
}