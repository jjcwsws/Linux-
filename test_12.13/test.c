#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:6031)
//����������ͼ��
//�汾1
//#include <stdio.h>
//int main() {
//    int a = 0;
//    while (~scanf("%d", &a))
//    {
//        int i = 0;
//        printf("*\n");//��ӡ��һ��*������
//        for (i = 1; i < a - 1; i++)
//        {
//            printf("*");//�м䲿�֣�ÿ��ѭ����ӡ��һ��*
//            int j = 0;
//            for (j = 0; j < 2 * i - 1; j++)
//            {
//                printf(" ");
//            }
//            printf("*\n");//�м䲿�֣�ÿ��ѭ����ӡ���һ��*
//        }
//        for (i = 0; i < a; i++)//��ӡ���һ�е�*
//        {
//            printf("* ");
//        }
//        printf("\n");
//    }
//    return 0;
//}
//�汾2
//#include <stdio.h>
//int main()
//{
//    int a = 0;
//    while (~scanf("%d", &a))
//    {
//        int i = 0;
//        for (i = 0; i < a; i++)
//        {
//            int j = 0;
//            for (j = 0; j <= i; j++)//������
//            {
//                if (i == j)//���ƴ�ӡ��һ��*
//                {
//                    printf("*\n");
//                }
//                else if (i != 0 && j == 0 || i == a - 1)//���ƴ�ӡ��һ�е�*�Լ�����һ�е�*
//                {
//                    printf("* ");
//                }
//                else {
//                    printf("  ");
//                }
//            }
//        }
//    }
//    return 0;
//}
//#include <stdio.h>
//int check_sys()
//{
//	int i = 1;
//	return *(char*) &i;//ȡ��i�ĵ�ַ��ת����char*��������ʱֻ�ܷ��ʵ�һ���ֽڵ�����
//}
//int main()
//{
//	int ret = check_sys();
//	if (ret == 1)
//	{
//		printf("С��\n");
//	}
//	else
//	{
//		printf("���\n");
//	}
//	return 0;
//}
//#include <stdio.h>
//int main()
//{
//	char a = -1;
//	signed char b = -1;
//	unsigned char c = -1;
//	printf("a=%d,b=%d,c=%d", a, b, c);
//	return 0;
//}
//#include <stdio.h>
//#include <windows.h>
//int main()
//{
//	unsigned int i;
//	for (i = 9; i >= 0; i--)
//	{
//		Sleep(1000);
//		printf("%u\n", i);
//	}
//
//	return 0;
//}
//#include <stdio.h>
//unsigned char i = 0;
//int main()
//{
//	for (i = 0; i <= 255; i++)
//	{
//		printf("hello world\n");
//	}
//	return 0;
//}
//#include <stdio.h>
//int main()
//{
//	char a[1000];
//	int i;
//	for (i = 0; i < 1000; i++)
//	{
//		a[i] = -1 - i;
//	}
//	printf("%d", strlen(a));
//	return 0;
//}
#include <stdio.h>
int main()
{
	int n = 9;
	float* pFloat = (float*)&n;
	printf("n��ֵΪ��%d\n", n);
	printf("*pFloat��ֵΪ��%f\n", *pFloat);
	//00000000000000000000000000001001  --9
	// 1001.0==(-1)^0*1.001*2^3
    //S=0
	//M=1.001
	//E=3  
	// 3+127==130  10000010
	//0 10000010 00100000000000000000000

	*pFloat = 9.0;
	printf("num��ֵΪ��%d\n", n);
	printf("*pFloat��ֵΪ��%f\n", *pFloat);
	return 0;
}