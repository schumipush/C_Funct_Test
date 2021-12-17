#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]){
    char *ifile[4];
    for (int i=0;i<4;i++){
        ifile[i] = (char*)malloc(16);
    }
    //ifile[0]="abcdefg";
    //*ifile[1]="qwerty";
    strcpy(ifile[0],"abcdefg-strcpy");

    printf("🌳 - 取字符串（要的是字符串首地址）：\n");
    printf("ifile[0]    = \t%s\n",ifile[0]);   
    printf("*ifile      = \t%s\n",*ifile);

    printf("🌳 - 取字符（要的是字符变量、不是地址）：\n");
    printf("*ifile[0]   = \t%c\n",*ifile[0]);
    printf("**ifile     = \t%c\n",**ifile);
    printf("**&ifile[0] = \t%c\n",**&ifile[0]);

    printf("🌳 - 取字符串地址：\n");
    printf("ifile[0]    = \t%p\n",ifile[0]);
    printf("*ifile      = \t%p\n",*ifile);
    // 不同编译器分配的地址不同：
    // 试了gcc和MSVC应该都是把字符串的地址分配到堆中的；
    // 但4个字符串被malloc的地址不相邻（MSVC）
    // &(*ifile[0]), &(*ifile[1]), &(*ifile[2]), &(*ifile[3])

    printf("🌳 - 取数组地址（ifile[]存放字符串地址的数组）：\n");
    printf("ifile       = \t%p\n",ifile);
    printf("&ifile[0]   = \t%p\n",&ifile[0]);
    printf("&ifile      = \t%p\n",&ifile);

    int a = 1,b=2;
    printf("\n再放两个相邻int变量的地址作对比：\n%p,%p\n",&a,&b);

    return 0;
}

/* 本程序输出：
🌳 - 取字符串（要的是字符串首地址）：
ifile[0]    =   abcdefg-strcpy
*ifile      =   abcdefg-strcpy
🌳 - 取字符（要的是字符变量、不是地址）：
*ifile[0]   =   a
**ifile     =   a
**&ifile[0] =   a
🌳 - 取字符串地址：
ifile[0]    =   0000000000093BE0
*ifile      =   0000000000093BE0
🌳 - 取数组地址（ifile[]存放字符串地址的数组）：
ifile       =   000000000061FDF0
&ifile[0]   =   000000000061FDF0
&ifile      =   000000000061FDF0

再放两个相邻int变量的地址作对比：
000000000061FDEC,000000000061FDE8
*/