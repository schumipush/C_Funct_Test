#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    FILE *fwb = fopen("_test_wb.txt","wb");
    fprintf(fwb,"12345\n");
    fprintf(fwb,"abdef\n");
    fprintf(fwb,"一二三四五\n");
    FILE *fw = fopen("_test_w.txt","w");
    fprintf(fw,"12345\n");
    fprintf(fw,"abdef\n");
    fprintf(fw,"一二三四五\n");
    return 0;
}

/* 测试说明：

测试了 fopen中，wb模式和w模式的区别：
- w是以文本方式打开文件，wb是二进制方式打开文件，以文本方式打开文件时，fwrite函数每碰到一个0x0A时，就在它的前面加入0x0D.其它内容不做添加操作（https://blog.csdn.net/guyue6670/article/details/6681037）
- 也就是说：写入文件时，wb的换行是 "OA", w 的换行是 "OD OA"；其它写入内容没有区别；

*/