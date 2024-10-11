#include "mymath.h"
#include "mystdio.h"
#include <stdio.h>

int main()
{
    int res = myAdd(10, 20);

    printf("%d+%d=%d\n", 10, 20, res);
    
    myFILE *fp = my_fopen("log.txt", "w");
    if(fp == NULL) return 1;
    return 0;
}
