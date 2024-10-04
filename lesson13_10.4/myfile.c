#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{

    //int fd = open("log.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
    //int fd = open("log.txt", O_WRONLY|O_CREAT|O_APPEND, 0666);
    int fd = open("log.txt", O_RDONLY);
    dup2(fd, 0);
    char buffer[1024];
    while(1)
    {
        // 默认情况，stdin键盘中读取，默认是要阻塞
        char * s = fgets(buffer, sizeof(buffer), stdin); // stdin -> FILE * -> _fileno
        if(s == NULL) break;
        printf("file content: %s", buffer);
    }
    //printf("hello hahahaha\n");














    // 0 1 2
    //close(1);
    ////open("log.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
    //open("log.txt", O_WRONLY|O_CREAT|O_APPEND, 0666);
    //printf("hello printf..................\n");
    //fprintf(stdout, "hello fprintf...........\n");

    //open("log.txt", O_RDONLY); // 0
    //printf("fd: %d\n", fd);
   // int a = 0;
   // scanf("%d", &a); // stdin -> _fileno = 0
   // printf("%d\n", a+100);


//    int fd = open("log.txt", O_RDONLY);
//    if(fd == -1)
//    {
//        perror("open");
//        return 1;
//    }
//    char buffer[1024];
//    ssize_t s = read(fd, buffer, sizeof(buffer));
//    if(s>0)
//    {
//        buffer[s] = 0;
//        printf("%s", buffer);
//    }
//    close(fd);

    //printf("%d\n", stdin->_fileno);
    //printf("%d\n", stdout->_fileno);
    //printf("%d\n", stderr->_fileno);
    //umask(0);
    //int fd1 = open("log1.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
    //int fd2 = open("log2.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
    //int fd3 = open("log3.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
    //int fd4 = open("log4.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
    //int fd5 = open("log5.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
    ////int fd = open("log.txt", O_WRONLY|O_CREAT|O_APPEND, 0666);
    ////if(fd == -1)
    ////{
    ////    perror("open");
    ////    return 1;
    ////}

    //printf("fd: %d\n", fd1);
    //printf("fd: %d\n", fd2);
    //printf("fd: %d\n", fd3);
    //printf("fd: %d\n", fd4);
    //printf("fd: %d\n", fd5);

   // const char *str = "aaaa\n";
   // write(fd, str, strlen(str));

   // close(fd);
    return 0;
}



















///#define ONE 1
///#define TWO (1<<1)
///#define THREE (1<<2)
///#define FOUR (1<<3)
///#define FIVE (1<<4)
///
///void Print(int flag)
///{
///    if(flag & ONE) printf("1\n");
///    if(flag & TWO) printf("2\n");
///    if(flag & THREE) printf("3\n");
///    if(flag & FOUR) printf("4\n");
///    if(flag & FIVE) printf("5\n");
///}
///
///int main()
///{
///    Print(ONE);
///    printf("----------------------\n");
///    Print(TWO);
///    printf("----------------------\n");
///    Print(ONE|TWO);
///    printf("----------------------\n");
///    Print(THREE|FOUR|FIVE);
///    printf("----------------------\n");
///    Print(ONE|TWO|THREE|FOUR|FIVE);
///}































//#include <stdio.h>
//#include <string.h>
//#include <sys/types.h>
//#include <unistd.h>
//
//#define FILENAME "log.txt"
//
//int main()
//{
//    char buffer[64];
//    fscanf(stdin, "%s", buffer);
//
//    printf("%s", buffer);
//
////    printf("hello printf\n");
////    fputs("hello fputs\n", stdout);
////    const char *msg = "hello fwrite\n";
////    fwrite(msg, 1, strlen(msg), stdout);
////    fprintf(stdout, "hello fprintf\n");
//
//    //chdir("/home/whb/110");
//    //FILE *fp = fopen(FILENAME, "r");
//    //if(fp == NULL)
//    //{
//    //    perror("fopen");
//    //    return 1;
//    //}
//
//    //char buffer[64];
//    //while(1)
//    //{
//    //    char *r = fgets(buffer, sizeof(buffer), fp);
//    //    if(!r) break;
//
//    //    printf("%s", buffer);
//    //}
//
//   // const char *msg = "hello bit\n";
//   // int cnt = 5;
//   // while(cnt)
//   // {
//   //     int n = fwrite(msg, strlen(msg), 1, fp);
//   //     printf("write %d block, pid is : %d\n", n, getpid());
//   //     cnt--;
//   //     sleep(20);
//   // }
//
//    //fclose(fp);
//    return 0;
//}
