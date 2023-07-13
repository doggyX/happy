#include <iostream>
#include <assert.h>
#include <string>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

// 为什么不定义全局buffer来进行通信呢？？ 因为有写时拷贝的存在，无法更改通信！

int main()
{
    // 1. 创建管道
    int pipfd[2] = {0};  // pipfd[0(嘴巴)] 为 读端  pipfd[1(钢笔)] 为 写端
    int n = pipe(pipfd); // 读写方式打开 ？我想这并非磁盘文件 而在内存
    assert(n != -1);     // debug assert 有效 release 被删掉
    (void)n;             // 防止release 模式 告警 n只定义未使用

#ifdef DEBUG
    cout << "pipfd[0]:" << pipfd[0] << endl; // 3
    cout << "pipfd[1]:" << pipfd[1] << endl; // 4
#endif

    // 2. 创建子进程
    pid_t id = fork();
    assert(id != -1);

    if (id == 0)
    {
        // 子进程
        // 3. 构建单向通信的信道 设父进程写入 子进程读取
        // 3. 关闭子进程不需要的fd
        close(pipfd[1]);
        int count = 0;
        char buffer[1024];
        while (true)
        {
            // 写入的一方文件描述符没有关闭 有数据读入的一方就读 没有就等
            // 写入的一方文件描述符关闭     read函数返回0 表示读到了文件的结尾
            ssize_t s = read(pipfd[0], buffer, sizeof(buffer) - 1);
            if (s > 0)
            {
                buffer[s] = 0;
                cout << "child get a message[" << getpid() << "]   Father #" << buffer << endl;
            }
            // else if (s == 0) // 写端的fd已关闭
            // {
            //     cout << "writer quit(father), me quit!!" << endl;
            //     break;
            // }
            count++;
            if (count == 5)
                close(pipfd[0]); // 关闭读端 写端被OS自动关闭
        }

        exit(0);
    }

    // 父进程
    // 3. 构建单向通信的信道
    // 3. 关闭父进程进程不需要的fd
    close(pipfd[0]);
    string message = "我是父进程，我在给你发信息";
    int count = 0;
    char send_buffer[1024];
    while (true)
    {
        // 3.2 构建一个变化的字符串
        snprintf(send_buffer, sizeof(send_buffer), "%s : %d", message.c_str(), count);
        // 写入send_buffer ---  我是父进程，我在给你发信息 ： 0/1/2...
        // 3.3 写入
        write(pipfd[1], send_buffer, strlen(send_buffer));
        printf("father send:%d\n", count++);
        // 3.4 故意sleep
        sleep(1);
        // if (count == 5)
        // {
        //     cout << "writer quite!\n";
        //     break;
        // }
    }

    close(pipfd[1]);
    pid_t ret = waitpid(id, nullptr, 0);
    assert(ret > 0);
    (void)ret;
}
