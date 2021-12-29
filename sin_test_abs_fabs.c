#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[]){
    float diff = -0.97500000000004;
    // abs 的参数类型是整形，如果送入一个浮点数，则会先舍弃小数部分、再取绝对值；
    printf("abs(diff) = %lf\n", abs(diff));
    printf("abs(-0.8) = %lf\n", abs(diff));
    // fabs 对小数取绝对值；
    printf("fabs(diff)= %lf\n", fabs(diff));
    
    
}