#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read(FILE* fp) {
    static long int len;
    printf("ftell=%ld\n", ftell(fp));
    
    //fseek(fp, len, SEEK_SET);
    printf("ftell=%ld\n", ftell(fp));

    char buff[100]={0};
    int MAXRNXLEN = 100;
    printf("ftell=%ld\n", strlen(buff));
    fgets(buff, MAXRNXLEN, fp);
    printf("ftell=%ld\n", strlen(buff));
    printf("\t%s", buff);
    printf("ftell=%ld\n", ftell(fp));
    len = ftell(fp);
}
int read2(FILE *fp){
    fseek(fp,3,SEEK_SET);
    char buff[100]={0};
    fgets(buff,100,fp);
    printf("%s",buff);
    printf("%ld",ftell(fp));
}
//在read基础上修改，使用strlen代替了ftell的功能，计算文件读取位置
// 现在可以成功读出每一行
int read3(FILE* fp) {
    static long int len;
    printf("ftell=%ld\n", ftell(fp));
    
    fseek(fp, len, SEEK_SET);
    printf("ftell=%ld\n", ftell(fp));

    char buff[100]={0};
    int MAXRNXLEN = 100;
    //printf("ftell=%ld\n", strlen(buff));
    fgets(buff, MAXRNXLEN, fp);
    printf("strlen=%ld\n", strlen(buff));
    printf("\t%s", buff);
    printf("ftell=%ld\n", ftell(fp));
    len += strlen(buff);
}

// 整理read3，思路一样
int read4(FILE* fp) {
    static long int len;
    fseek(fp, len, SEEK_SET);
    char buff[100]={0};
    fgets(buff, 100, fp);
    printf("strlen=%ld\n", strlen(buff));
    printf("%s", buff);
    len += strlen(buff);
}


int open(char* infile) {
    FILE* fp = fopen(infile, "r");
    //read(fp);
    //read2(fp);
    //read3(fp);
    read4(fp);
    //连续调用两次也可以
    read4(fp);
    fclose(fp);
}



int main(int argc, char* argv[])
{
    char infile[] = "README.md";
    //"README_ANSI.md";
    //"README copy 2.md";
    //"README copy - 副本.md";
    for (int i = 0; i < 10; i++)
        open(infile);


    return 0;
}