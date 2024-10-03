#include <stdio.h>

int main()
{
    FILE *fp = fopen("./log.txt", "a");
    if(fp == NULL)
    {
        perror("fopen");
        return 1;
    }

    // 文件操作
    const char *str = "hello file\n";
    fputs(str, fp);

    fclose(fp);
    return 0;
}
