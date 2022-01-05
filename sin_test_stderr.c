// 例子来自：https://www.cnblogs.com/zhangyabin---acm/p/3203745.html
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    fprintf(stdout,"Hello ");
    fprintf(stderr,"World!");
    // 默认情况下，stdout是行缓冲的，他的输出会放在一个buffer里面，只有到换行的时候，才会输出到屏幕。
    // 而stderr是无缓冲的，会直接输出，举例来说就是printf(stdout, "xxxx") 和 printf(stdout, "xxxx\n")，
    // 前者会憋住，直到遇到新行才会一起输出。而printf(stderr, "xxxxx")，不管有么有\n，都输出。
    return 0;
}

/*
ubuntu子系统 gcc 编译结果：
schumipush@T560:/mnt.../C_Funct_Test$ gcc sin_test_stderr.c
schumipush@T560:/mnt/d../C_Funct_Test$ ./a.out
World!Hello schumipush@T560:/mnt.../C_Funct_Test$
 */