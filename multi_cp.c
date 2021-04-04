// 多进程拷贝文件， 利用父子进程共享fd，mmap进行进程间通信
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        perror("./a.out src dst");
        exit(1);
    }
    int n = 5; //指定了默认进程个数
    int srcfd = open(argv[1], O_RDONLY);
    if (srcfd < 0)
    {
        perror("open err.");
        exit(1);
    }
    int dstfd = open(argv[2], O_RDWR);
    if (dstfd < 0)
    {
        perror("open dst err.");
        exit(1);
    }
    // 从源文件获取文件大小
    struct stat sb;
    stat(argv[1], &sb);
    int len = sb.st_size;
    // 设置目标文件大小和源文件大小一致
    truncate(argv[2], len);
    //将源文件映射到缓冲区
    char *psrc = mmap(NULL, len, PROT_READ, MAP_SHARED, srcfd, 0);
    if (psrc == MAP_FAILED)
    {
        perror("mmap src failed.");
        exit(1);
    }
    //将目标文件映射到缓冲区
    char *pdst = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, dstfd, 0);
    if (pdst == MAP_FAILED)
    {
        perror("mmap dst failed.");
        exit(1);
    }
    //创建多个子进程
    int i;
    for (i = 0; i < n; ++i)
    {
        if (fork() == 0)
        {
            break;
        }
    }
    //拷贝数据
    if (i < n)
    {
        int cplen;
        int dlen = len / n;
        if (i == n - 1)
        {
            cplen = dlen + len % n;
        }
        else
        {
            cplen = dlen;
        }
        memcpy(pdst + i * dlen, psrc + i * dlen, cplen);
    }
    else
    {
        for (int j = 0; j < n; ++j)
        {
            wait(NULL);
        }
    }
    if (munmap(psrc, len) < 0)
    {
        perror("munmap err.");
        exit(1);
    }
    if (munmap(pdst, len) < 0)
    {
        perror("munmap dst err.");
        exit(1);
    }
    close(srcfd);
    close(dstfd);
    return 0;
}