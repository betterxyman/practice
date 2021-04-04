// 实现兄弟进程间通信
// 实现 ps aux | grep bash
#include<stdio.h>
#include<sys/types.h>  
#include<sys/wait.h>
#include<unistd.h>
int main()
{
	int fd[2];
	pipe(fd);
	pid_t id1=fork();
	if (id1==0){
		// 先重定向 ps aux命令的输出写入管道
		dup2(fd[1],STDOUT_FILENO);
		// 关闭读
		close(fd[0]);
		execlp("ps","ps","aux",NULL);
	}else{
		pid_t id2=fork();
		if (id2==0){
			// 先重定向输入为读管道
			dup2(fd[0],STDIN_FILENO);
			// 关闭写
			close(fd[1]);
			execlp("grep","grep","bash",NULL);

		}else{
			close(fd[0]);
			close(fd[1]);
			wait(NULL);
			wait(NULL);
		}
	}

	return 0;
}
