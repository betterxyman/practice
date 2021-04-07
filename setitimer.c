// 使用setitimer实现每隔一秒打印一次hello world！
#include<stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include<signal.h>

void catch_signal(int signum)
{
     // 必须有换行符号才会显示出来，否则会放到行缓冲中，当遇到行刷新标志或者该缓存已满的情况下，才会把缓存的数据显示到终端设备上
    printf("hello world!\n");
}
int main()
{
    struct sigaction a;
    a.sa_flags=0;
    a.sa_handler=catch_signal;
    sigemptyset(&a.sa_mask);
    sigaction(SIGALRM,&a,NULL);
    struct itimerval mytimer={{1,0},{1,0}};
    setitimer(ITIMER_REAL, &mytimer, NULL);
    while(1){}
    return 0;
}