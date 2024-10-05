#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define SIZE 1024
#define MAX_ARGC 64
#define SEP " "
#define STREND '\0'

// 下面的都和重定向有关
#define NoneRedir  -1
#define StdinRedir  0
#define StdoutRedir 1
#define AppendRedir 2

#define IgnSpace(buf,pos) do{ while(isspace(buf[pos])) pos++; }while(0)

int redir_type = NoneRedir;
char *filename = NULL;

char *argv[MAX_ARGC];
char pwd[SIZE];
char env[SIZE]; // for test
int lastcode = 0;


// ls -a -l
// ls -a -l > log.txt


const char* HostName()
{
    char *hostname = getenv("HOSTNAME");
    if(hostname) return hostname;
    else return "None";
}

const char* UserName()
{
    char *hostname = getenv("USER");
    if(hostname) return hostname;
    else return "None";
}

const char *CurrentWorkDir()
{
    char *hostname = getenv("PWD");
    if(hostname) return hostname;
    else return "None";
}

char *Home()
{
    return getenv("HOME");
}

int Interactive(char out[], int size)
{
    // 输出提示符并获取用户输入的命令字符串"ls -a -l"
    printf("[%s@%s %s]$ ", UserName(), HostName(), CurrentWorkDir());
    fgets(out, size, stdin);
    out[strlen(out)-1] = 0; //'\0', commandline是空串的情况?
    return strlen(out);
}

void CheckRedir(char in[])
{
    // ls -a -l
    // ls -a -l > log.txt
    // ls -a -l >> log.txt
    // cat < log.txt
    redir_type = NoneRedir;
    filename = NULL;
    int pos = strlen(in) - 1;
    while( pos >= 0 )
    {
        if(in[pos] == '>')
        {
            if(in[pos-1] == '>')
            {
                redir_type = AppendRedir;
                in[pos-1] = STREND;
                pos++;
                IgnSpace(in, pos);
                filename = in+pos;
                break;
            }
            else
            {
                redir_type = StdoutRedir;
                in[pos++] = STREND;
                IgnSpace(in, pos);
                filename = in+pos;
                //printf("debug: %s, %d\n", filename, redir_type);
                break;
            }
        }
        else if(in[pos] == '<')
        {
            redir_type = StdinRedir;
            in[pos++] = STREND;
            IgnSpace(in, pos);
            filename = in+pos;
            //printf("debug: %s, %d\n", filename, redir_type);
            break;
        }
        else
        {
            pos--;
        }
    }
}

void Split(char in[])
{
    CheckRedir(in);
    int i = 0;
    argv[i++] = strtok(in, SEP); // "ls -a -l"
    while(argv[i++] = strtok(NULL, SEP)); // 故意将== 写成 =
    if(strcmp(argv[0], "ls") ==0)
    {
        argv[i-1] = (char*)"--color";
        argv[i] = NULL;
    }
}

void Execute()
{
    pid_t id = fork();
    if(id == 0)
    {
        int fd = -1;
        if(redir_type == StdinRedir)
        {
            fd = open(filename, O_RDONLY);
            dup2(fd, 0);
        }
        else if(redir_type == StdoutRedir)
        {
            fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC);
            dup2(fd, 1);
        }
        else if(redir_type == AppendRedir)
        {
            fd = open(filename, O_CREAT | O_WRONLY | O_APPEND);
            dup2(fd, 1);
        }
        else
        {
            // do nothing
        }

        // 让子进程执行命名
        execvp(argv[0], argv);
        exit(1);
    }
    int status = 0;
    pid_t rid = waitpid(id, &status, 0);
    if(rid == id) lastcode = WEXITSTATUS(status); 
    //printf("run done, rid: %d\n", rid);
}

int BuildinCmd()
{
    int ret = 0;
    // 1. 检测是否是内建命令, 是 1, 否 0
    if(strcmp("cd", argv[0]) == 0)
    {
        // 2. 执行
        ret = 1;
        char *target = argv[1]; //cd XXX or cd
        if(!target) target = Home();
        chdir(target);
        char temp[1024];
        getcwd(temp, 1024);
        snprintf(pwd, SIZE, "PWD=%s", temp);
        putenv(pwd);
    }
    else if(strcmp("export", argv[0]) == 0)
    {
        ret = 1;
        if(argv[1])
        {
            strcpy(env, argv[1]);
            putenv(env);
        }
    }
    else if(strcmp("echo", argv[0]) == 0)
    {
        ret = 1;
        if(argv[1] == NULL) {
            printf("\n");
        }
        else{
            if(argv[1][0] == '$')
            {
                if(argv[1][1] == '?')
                {
                    printf("%d\n", lastcode);
                    lastcode = 0;
                }
                else{
                    char *e = getenv(argv[1]+1);
                    if(e) printf("%s\n", e);
                }
            }
            else{
                printf("%s\n", argv[1]);
            }
        }
    }
    return ret;
}

int main()
{
    while(1)
    {
        char commandline[SIZE];
        // 1. 打印命令行提示符，获取用户输入的命令字符串
        int n = Interactive(commandline, SIZE);
        if(n == 0) continue;
        // 2. 对命令行字符串进行切割
        Split(commandline);
        // 3. 处理内建命令
        n = BuildinCmd();
        if(n) continue;
        // 4. 执行这个命令
        Execute();
    }
   // for(int i=0; argv[i]; i++)
   // {
   //     printf("argv[%d]: %s\n", i, argv[i]);
   // }
    return 0;
}




















