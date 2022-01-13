#include <stdio.h>
#include <stdlib.h>

extern double str2num(const char *s, int i, int n)
{
    double value;
    char str[256],*p=str;
    
    // 如果输入字符串s的长度，小于给定的读取位置i，则返回0；
    // 如果要读取的长度大于255，也返回0；
    if (i<0||(int)strlen(s)<i||(int)sizeof(str)-1<n) return 0.0;
    // d,D作用同E，后接指数
    for (s+=i;*s&&--n>=0;s++) *p++=*s=='d'||*s=='D'?'E':*s;
    *p='\0';
    return sscanf(str,"%lf",&value)==1?value:0.0;
}

int main(){
    double a = str2num("123d345",0,4);
    printf("%f",a);
}