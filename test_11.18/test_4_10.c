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
    ////int d = ��int1��0 / c + 1;//int dȥ����һ��float,����ʽת��������
    //���磺int a=3.14;a�ͻ����3

    printf("%d", c );

    return 0;
}


//ţţ�սӴ��Ȳ����У� ��֪���Ȳ�������ÿ����ǰһ��Ĳ�ֵ����ȵ����У���֪��һ�� a �͵ڶ��� b ��ֵ������֪���������Ƕ��١�
#include <stdio.h>

int main()
{

    int a, b, c;
    scanf("%d %d", &a, &b);
    c = b + b - a;//�Ȳʽ
    printf("%d", c);
    return 0;
}

//ţţ��һ���뾶Ϊ r ��������֪�������������

int main()
{
    int r = 0;
    float a = 3.14F;
    scanf("%d", &r);
    a = (float)4 / 3 * a * (r * r * r);//�����ʽ
    printf("%f", a);
    return 0;
}