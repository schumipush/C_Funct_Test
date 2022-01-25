// 测试 sscanf()函数；

#include <stdlib.h>
#include <stdio.h>
int main(int argc, char * argv[]){
    char str1[32],str2[32]="";
    // 这其实是一条 sat dcb数据；
    char *buff = "G01                           1.360       0.001";
    int num = sscanf(buff,"%s %s",str1,str2);
    printf("%d %s %s\n",num,str1,str2);
    return 0;
}