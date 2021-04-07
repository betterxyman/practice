// 捕获sigchild信号回收子进程
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

void cat_sigchild(int sig)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(0, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("child exit with %d, pid=%d\n", WEXITSTATUS(status), pid);
        }
        else if (WIFSIGNALED(status))
        {
            printf("child kill by sig %d, pid=%d\n", WTERMSIG(status), pid);
        }
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    int i;
    //如果在信号捕捉函数设置之前，子进程先结束，那么如果提前不阻塞SIGCHLD信号的话，
    //就会导致先结束的子进程无法回收，所以在创建子进程之前应该先阻塞SIGCHLD信号。
    sigset_t newset, oldset;
    sigemptyset(&newset);
    sigaddset(&newset, SIGCHLD);
    sigprocmask(SIG_BLOCK, &newset, &oldset);
    for (i = 0; i < 10; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            break;
        }
        else if (pid < 0)
        {
            perror("fork:");
            exit(1);
        }
    }

    if (pid == 0)
    {
        printf("i am %d child pid is %d\n", i + 1, getpid());
        // 留时间差等着被kill
        sleep(3);
        return i + 1;
    }
    else if (pid > 0)
    {
        struct sigaction act, oldact;
        act.sa_handler = cat_sigchild;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD, &act, NULL);

        //挂上回调后解除阻塞
        sigprocmask(SIG_SETMASK, &oldset, NULL);
        // 等待最后一个子进程打印自己的信息再kill
        sleep(2);
        kill(pid, SIGALRM); // 子进程已经变成僵尸进程也可以kill
        while (1)
        {
            sleep(1);
        }
    }
    return 0;
}