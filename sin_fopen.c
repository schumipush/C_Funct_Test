#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
    FILE* fp = fopen("D:\\abc\\test.txt","wb");
    // 打开文件前，必须确保路径已经存在或创建；
    fprintf(fp,"test");
    return 0;
}