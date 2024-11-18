#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:6031)
#include <stdio.h>

int main()
{
    int a, b;
    int c = 3.14;
    //float c = 3.14;//3.14F
    //scanf("%d %d", &a, &b);
    //c = c * a * (b * b) / 1000;
    ////int d = （int1）0 / c + 1;//int d去接收一个float,会隐式转换成整形
    //例如：int a=3.14;a就会等于3

    printf("%d", c );

    return 0;
}


//牛牛刚接触等差数列， 他知道等差数列是每项与前一项的差值都相等的数列，已知第一项 a 和第二项 b 的值，他想知道第三项是多少。
#include <stdio.h>

int main()
{

    int a, b, c;
    scanf("%d %d", &a, &b);
    c = b + b - a;//等差公式
    printf("%d", c);
    return 0;
}

//牛牛有一个半径为 r 的球，他想知道这个球的体积。

int main()
{
    int r = 0;
    float a = 3.14F;
    scanf("%d", &r);
    a = (float)4 / 3 * a * (r * r * r);//体积公式
    printf("%f", a);
    return 0;
}