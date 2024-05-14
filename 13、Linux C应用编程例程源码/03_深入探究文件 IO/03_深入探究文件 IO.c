#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

// 1. 示例代码 3.2.1 strerror 测试代码\n\

int test_strerror(void)
{
    int fd;
    
    /* 打开文件 */
    fd = open("./test_file", O_RDONLY);
    if (-1 == fd) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    
    close(fd);
    return 0;
}

// 2. 示例代码 3.2.2 perror 测试代码\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
#include <stdlib.h>

int test_perror(void)
{
    int fd;

    /* 打开文件 */
    fd = open("./test_file", O_RDONLY);
    if (-1 == fd) {
        perror("Custom Head");
        return -1;
    }

    close(fd);
    return 0;
}

// 3. 示例代码 3.3.1 exit()使用示例\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>

// exit()是一个标准 C 库函数，而_exit()和_Exit()是系统调用。
// 执行 exit()会执行一些清理工作，最后调用_exit()函数。
int test_exit(void)
{
    int fd;

    /* 打开文件 */
    fd = open("./test_file", O_RDONLY);
    if (-1 == fd) {
        perror("open error");
        exit(-1);
    }

    close(fd);
    exit(0);
}

// 4. 示例代码 3.4.1 空洞文件测试代码\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

/*
alientek@ubuntu18:~/ZAT/13、Linux C应用编程例程源码/03_深入探究文件 IO$ ls -lh hole_file 
-rw-r--r-- 1 alientek alientek 8.0K 4月  12 09:59 hole_file
alientek@ubuntu18:~/ZAT/13、Linux C应用编程例程源码/03_深入探究文件 IO$ du -h hole_file 
4.0K    hole_file
*/
int test_holefile(void)
{
    int fd;
    int ret;
    char buffer[1024];
    int i;

    /* 打开文件 */
    fd = open("./hole_file", O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (-1 == fd) {
        perror("open error");
        exit(-1);
    }

    /* 将文件读写位置移动到偏移文件头 4096 个字节(4K)处 */
    ret = lseek(fd, 4096, SEEK_SET);
    if (-1 == ret) {
        perror("lseek error");
        goto err;
    }

    /* 初始化 buffer 为 0xFF */
    memset(buffer, 0xFF, sizeof(buffer));

    /* 循环写入 4 次，每次写入 1K */
    for (i = 0; i < 4; i++) {
        ret = write(fd, buffer, sizeof(buffer));
        if (-1 == ret) {
            perror("write error");
            goto err;
        }
        ret = 0;
    }

err:
    /* 关闭文件 */
    close(fd);
    exit(ret);
}

// 5. 示例代码 3.5.1 O_TRUNC 标志测试\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>

// 调用 open 函数打开文件的时候会将文件
// 原本的内容全部丢弃，文件大小变为 0

// O_TRUNC的全称是"Open Truncate"，它是open()函数的一个选项，用于打开文件时清空文件内容。
int test_O_TRUNC(void)
{
    int fd;
    
    /* 打开文件 */
    fd = open("./test_file", O_WRONLY | O_TRUNC);
    if (-1 == fd) {
        perror("open error");
        exit(-1);
    }
    
    /* 关闭文件 */
    close(fd);
    exit(0);
}

// 6. 示例代码 3.5.2 O_APPEND 标志测试 1\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// 如果 open 函数携带了 O_APPEND 标志，调用 open 函数打开文件，
// 当每次使用 write()函数对文件进行写操作时，都会自动把文件当前位置偏移量移动到文件末尾，从文件末
// 尾开始写入数据，也就是意味着每次写入数据都是从文件末尾开始。
int test_O_APPEND(void)
{
    char buffer[16];
    int fd;
    int ret;
    
    /* 打开文件 */
    fd = open("./test_file", O_RDWR | O_APPEND);
    if (-1 == fd) {
        perror("open error");
        exit(-1);
    }

    /* 初始化 buffer 中的数据 */
    memset(buffer, 0x55, sizeof(buffer));

    /* 写入数据: 写入 4 个字节数据 */
    ret = write(fd, buffer, 4);
    if (-1 == ret) {
        perror("write error");
        goto err;
    }

    /* 将 buffer 缓冲区中的数据全部清 0 */
    memset(buffer, 0x00, sizeof(buffer));

    /* 将位置偏移量移动到距离文件末尾 4 个字节处 */
    ret = lseek(fd, -4, SEEK_END);
    if (-1 == ret) {
        perror("lseek error");
        goto err;
    }

    /* 读取数据 */
    ret = read(fd, buffer, 4);
    if (-1 == ret) {
        perror("read error");
        goto err;
    }

    printf("0x%x 0x%x 0x%x 0x%x\n", buffer[0], buffer[1], buffer[2], buffer[3]);

    ret = 0;

err:
    /* 关闭文件 */
    close(fd);
    exit(ret);
}

// 7. 示例代码 3.6.1 多次打开同一个文件测试代码 1\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>

int test_OpenTheSameFileMultipleTimes(void)
{
    int fd1, fd2, fd3;
    int ret;

    /* 第一次打开文件 */
    fd1 = open("./test_file", O_RDWR);
    if (-1 == fd1) {
        perror("open error");
        exit(-1);
    }

    /* 第二次打开文件 */
    fd2 = open("./test_file", O_RDWR);
    if (-1 == fd2) {
        perror("open error");
        ret = -1;
        goto err1;
    }

    /* 第三次打开文件 */
    fd3 = open("./test_file", O_RDWR);
    if (-1 == fd3) {
        perror("open error");
        ret = -1;
        goto err2;
    }

    /* 打印出 3 个文件描述符 */
    printf("%d %d %d\n", fd1, fd2, fd3);
    close(fd3);
    ret = 0;

err2:
    close(fd2);

err1:
    /* 关闭文件 */
    close(fd1);
    exit(ret);
}

// 8. 示例代码 3.6.2 多次打开同一个文件测试代码 2\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// 即使多次打开同一个文件，内存中也只有一份动态文件。
int test_OpenTheSameFileMultipleTimes_2(void)
{
    char buffer[4];
    int fd1, fd2;
    int ret;

    /* 创建新文件 test_file 并打开 */
    fd1 = open("./test_file", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (-1 == fd1) {
        perror("open error");
        exit(-1);
    }

    /* 再次打开 test_file 文件 */
    fd2 = open("./test_file", O_RDWR);
    if (-1 == fd2) {
        perror("open error");
        ret = -1;
        goto err1;
    }

    /* 通过 fd1 文件描述符写入 4 个字节数据 */
    buffer[0] = 0x11;
    buffer[1] = 0x22;
    buffer[2] = 0x33;
    buffer[3] = 0x44;
    ret = write(fd1, buffer, 4);
    if (-1 == ret) {
        perror("write error");
        goto err2;
    }

    /* 将读写位置偏移量移动到文件头 */
    ret = lseek(fd2, 0, SEEK_SET);
    if (-1 == ret) {
        perror("lseek error");
        goto err2;
    }

    /* 读取数据 */
    memset(buffer, 0x00, sizeof(buffer));
    ret = read(fd2, buffer, 4);
    if (-1 == ret) {
        perror("read error");
        goto err2;
    }
    printf("0x%x 0x%x 0x%x 0x%x\n", buffer[0], buffer[1], buffer[2], buffer[3]);

    ret = 0;

err2:
    close(fd2);

err1:
    /* 关闭文件 */
    close(fd1);
    exit(ret);
}

// 9. 示例代码 3.6.3 多次打开同一个文件测试代码 3\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// 一个进程内多次 open 打开同一个文件，不同文件描述符所对应的读写位置偏移量是相互独立的。

/* 示例代码 3.6.3 中，重复两次打开 test_file 文件，分别得到两个文件描述符 fd1、fd2；首先通过 fd1 写
入 4 个字节数据（0x11、0x22、0x33、0x44）到文件中，接着再通过 fd2 写入 4 个字节数据（0xaa、0xbb、
0xcc、0xdd）到文件中，循环写入 4 此；最后再将写入的数据读取出来，将其打印到终端。如果它们是分别
写，那么读取出来的数据就应该是 aabbccdd……，因为通过 fd1 写入的数据被 fd2 写入的数据给覆盖了；如
果它们是接续写，那么读取出来的数据应该是 11223344aabbccdd……，*/
int test_IndependentOffsetOfDifferentFilesDescriptors(void)
{
    unsigned char buffer1[4], buffer2[4];
    int fd1, fd2;
    int ret;
    int i;

    /* 创建新文件 test_file 并打开 */
    fd1 = open("./test_file", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (-1 == fd1) {
        perror("open error");
        exit(-1);
    }

    /* 再次打开 test_file 文件 */
    fd2 = open("./test_file", O_RDWR);
    if (-1 == fd2) {
        perror("open error");
        ret = -1;
        goto err1;
    }

    /* buffer 数据初始化 */
    buffer1[0] = 0x11;
    buffer1[1] = 0x22;
    buffer1[2] = 0x33;
    buffer1[3] = 0x44;
    buffer2[0] = 0xAA;
    buffer2[1] = 0xBB;
    buffer2[2] = 0xCC;
    buffer2[3] = 0xDD;

    /* 循环写入数据 */
    for (i = 0; i < 4; i++) {
        ret = write(fd1, buffer1, sizeof(buffer1));
        if (-1 == ret) {
            perror("write error");
            goto err2;
        }
        ret = write(fd2, buffer2, sizeof(buffer2));
        if (-1 == ret) {
            perror("write error");
            goto err2;
        }
    }

    /* 将读写位置偏移量移动到文件头 */
    ret = lseek(fd1, 0, SEEK_SET);
    if (-1 == ret) {
        perror("lseek error");
        goto err2;
    }

    /* 读取数据 */
    for (i = 0; i < 8; i++) {
        ret = read(fd1, buffer1, sizeof(buffer1));
        if (-1 == ret) {
            perror("read error");
            goto err2;
        }
        printf("%x%x%x%x", buffer1[0], buffer1[1], buffer1[2], buffer1[3]);
    }
    printf("\n");
    ret = 0;

err2:
    close(fd2);

err1:
    /* 关闭文件 */
    close(fd1);
    exit(ret);
}

// 10.示例代码 3.6.4 多次打开同一个文件测试代码 4\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// 当 open 函数使用 O_APPEND 标志，在使用 write 函数进行写入操作时，会自动将偏移量移动到文件末尾，也就是每次写入都是从文件末尾开始
int test_O_APPEND_IndependentOffsetOfDifferentFilesDescriptors(void)
{
    unsigned char buffer1[4], buffer2[4];
    int fd1, fd2;
    int ret;
    int i;

    /* 创建新文件 test_file 并打开 */
    fd1 = open("./test_file", O_RDWR | O_CREAT | O_EXCL | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (-1 == fd1) {
        perror("open error");
        exit(-1);
    }

    /* 再次打开 test_file 文件 */
    fd2 = open("./test_file", O_RDWR | O_APPEND);
    if (-1 == fd2) {
        perror("open error");
        ret = -1;
        goto err1;
    }

    /* buffer 数据初始化 */
    buffer1[0] = 0x11;
    buffer1[1] = 0x22;
    buffer1[2] = 0x33;
    buffer1[3] = 0x44;
    buffer2[0] = 0xAA;
    buffer2[1] = 0xBB;
    buffer2[2] = 0xCC;
    buffer2[3] = 0xDD;

    /* 循环写入数据 */
    for (i = 0; i < 4; i++) {
        ret = write(fd1, buffer1, sizeof(buffer1));
        if (-1 == ret) {
            perror("write error");
            goto err2;
        }
        ret = write(fd2, buffer2, sizeof(buffer2));
        if (-1 == ret) {
            perror("write error");
            goto err2;
        }
    }

    /* 将读写位置偏移量移动到文件头 */
    ret = lseek(fd1, 0, SEEK_SET);
    if (-1 == ret) {
        perror("lseek error");
        goto err2;
    }

    /* 读取数据 */
    for (i = 0; i < 8; i++) {
        ret = read(fd1, buffer1, sizeof(buffer1));
        if (-1 == ret) {
            perror("read error");
            goto err2;
        }
        printf("%x%x%x%x", buffer1[0], buffer1[1], buffer1[2], buffer1[3]);
    }
    printf("\n");
    ret = 0;

err2:
    close(fd2);

err1:
    /* 关闭文件 */
    close(fd1);
    exit(ret);
}

// 11.示例代码 3.7.1 dup 函数测试代码\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// 由打印信息可知，fd1 等于 6，复制得到的新的文件描述符为 7（遵循 fd 分配原则），打印出来的数据
// 显示为接续写，所以可知，通过复制文件描述符可以实现接续写
int test_dup(void)
{
    unsigned char buffer1[4], buffer2[4];
    int fd1, fd2;
    int ret;
    int i;

    /* 创建新文件 test_file 并打开 */
    fd1 = open("./test_file", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (-1 == fd1) {
        perror("open error");
        exit(-1);
    }

    /* 复制文件描述符 */
    fd2 = dup(fd1);
    if (-1 == fd2) {
        perror("dup error");
        ret = -1;
        goto err1;
    }
    printf("fd1: %d\nfd2: %d\n", fd1, fd2);

    /* buffer 数据初始化 */
    buffer1[0] = 0x11;
    buffer1[1] = 0x22;
    buffer1[2] = 0x33;
    buffer1[3] = 0x44;
    buffer2[0] = 0xAA;
    buffer2[1] = 0xBB;
    buffer2[2] = 0xCC;
    buffer2[3] = 0xDD;

    /* 循环写入数据 */
    for (i = 0; i < 4; i++) {
        ret = write(fd1, buffer1, sizeof(buffer1));
        if (-1 == ret) {
            perror("write error");
            goto err2;
        }
        ret = write(fd2, buffer2, sizeof(buffer2));
        if (-1 == ret) {
            perror("write error");
            goto err2;
        }
    }

    /* 将读写位置偏移量移动到文件头 */
    ret = lseek(fd1, 0, SEEK_SET);
    if (-1 == ret) {
        perror("lseek error");
        goto err2;
    }

    /* 读取数据 */
    for (i = 0; i < 8; i++) {
        ret = read(fd1, buffer1, sizeof(buffer1));
        if (-1 == ret) {
            perror("read error");
            goto err2;
        }
        printf("%x%x%x%x", buffer1[0], buffer1[1], buffer1[2], buffer1[3]);
    }
    printf("\n");
    ret = 0;

err2:
    close(fd2);

err1:
    /* 关闭文件 */
    close(fd1);
    exit(ret);
}

// 12.示例代码 3.7.2 dup2 函数测试代码\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>

// dup 系统调用分配的文件描述符是由系统分配的，遵循文件描述符分配原则，并不能自己指定一个文件
// 描述符，这是 dup 系统调用的一个缺陷；而 dup2 系统调用修复了这个缺陷，可以手动指定文件描述符，而
// 不需要遵循文件描述符分配原则，
int test_dup_2(void)
{
    int fd1, fd2;
    int ret;

    /* 创建新文件 test_file 并打开 */
    fd1 = open("./test_file", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (-1 == fd1) {
        perror("open error");
        exit(-1);
    }

    /* 复制文件描述符 */
    fd2 = dup2(fd1, 100);
    if (-1 == fd2) {
        perror("dup error");
        ret = -1;
        goto err1;
    }

    printf("fd1: %d\nfd2: %d\n", fd1, fd2);
    ret = 0;

    close(fd2);

err1:
    /* 关闭文件 */
    close(fd1);
    exit(ret);
}

// 13.示例代码 3.9.1 pread 函数测试\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>

// 在当前目录下存在一个文件 test_file，上述代码中会打开 test_file 文件，然后直接使用 pread 函数读取
// 1 个字节数据，从偏移文件头部 2 字节处，读取完成之后再使用 lseek 函数获取到文件当前位置偏移
// 量，并将其打印出来。假如 pread 函数会改变文件表中记录的当前位置偏移量，则打印出来的数据应该是
// 2 + 1 = 3；如果不会改变文件表中记录的当前位置偏移量，则打印出来的数据应该是 0

// pread 函数确实不会改变文件表中记录的
// 当前位置偏移量；同理，pwrite 函数也是如此，大家可以把 pread 换成 pwrite 函数再次进行测试，不出意外，
// 打印出来的数据依然是 0。
// 如果把 pread 函数换成 read（或 write）函数，那么打印出来的数据就是 100 了，因为读取了 100 个字
// 节数据，相应的当前位置偏移量会向后移动 100 个字节。
int test_pread(void)
{
    unsigned char buffer[1];
    int fd;
    int ret;

    /* 打开文件 test_file */
    fd = open("./test_file", O_RDWR);
    if (-1 == fd) {
        perror("open error");
        exit(-1);
    }

    /* 使用 pread 函数读取数据(从偏移文件头 2 字节处开始读取) */
    ret = pread(fd, buffer, sizeof(buffer), 2);
    if (-1 == ret) {
        perror("pread error");
        goto err;
    }

    /* 获取当前位置偏移量 */
    ret = lseek(fd, 0, SEEK_CUR);
    if (-1 == ret) {
        perror("lseek error");
        goto err;
    }

    printf("Current Offset: %d\n", ret);
    ret = 0;

err:
    /* 关闭文件 */
    close(fd);
    exit(ret);
}

// 14.示例代码 3.10.1 fcntl 复制文件描述符\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>

// fcntl函数的全称是"file control"，它是一个系统调用，用于对已打开的文件描述符进行各种操作，包括获取或设置文件状态标志、锁定文件、改变文件读写位置等。
// F_DUPFD是fcntl函数的一个操作参数，其全称是"File Duplicate for Descriptor"，用于复制一个文件描述符。
int test_fcontl_F_DUPFD(void)
{
    int fd1, fd2;
    int ret;

    /* 打开文件 test_file */
    fd1 = open("./test_file", O_RDONLY);
    if (-1 == fd1) {
        perror("open error");
        exit(-1);
    }

    /* 使用 fcntl 函数复制一个文件描述符 */
    fd2 = fcntl(fd1, F_DUPFD, 0);
    if (-1 == fd2) {
        perror("fcntl error");
        ret = -1;
        goto err;
    }

    printf("fd1: %d\nfd2: %d\n", fd1, fd2);
    ret = 0;

    close(fd2);

err:
    /* 关闭文件 */
    close(fd1);
    exit(ret);
}

// 15.示例代码 3.10.2 fcntl 读取/设置文件状态标志\n\

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>

int test_fcontl_F_GETFL_F_SETFL(void)
{
    int fd;
    int ret;
    int flag;

    /* 打开文件 test_file */
    fd = open("./test_file", O_RDWR);
    if (-1 == fd) {
        perror("open error");
        exit(-1);
    }

    /* 获取文件状态标志 */
    flag = fcntl(fd, F_GETFL);
    if (-1 == flag) {
        perror("fcntl F_GETFL error");
        ret = -1;
        goto err;
    }
    printf("flags: 0x%x\n", flag);

    /* 设置文件状态标志,添加 O_APPEND 标志 */
    ret = fcntl(fd, F_SETFL, flag | O_APPEND);
    if (-1 == ret) {
        perror("fcntl F_SETFL error");
        goto err;
    }

    ret = 0;

err:
    /* 关闭文件 */
    close(fd);
    return ret;
}

// 16.示例代码 3.11.1 文件截断操作\n\

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

/* 首先使用 open()函数打开文件 file1，得到文件描述符 fd，接着使用 ftruncate()系统调用将
文件截断为 0 长度，传入 file1 文件对应的文件描述符；接着调用 truncate()系统调用将文件 file2 截断为 1024
字节长度，传入 file2 文件的相对路径。*/

/* 在当前目录下准备两个文件 file1 和 file2  file1 和 file2 文件此时均为 592 字节大小
程序运行之后，file1 文件大小变成了 0，而 file2 文件大小变成了 1024 字节，*/

int test_ioctl_truncate_ftruncate(void)
{
    int fd;

    /* 打开 file1 文件 */
    if (0 > (fd = open("./file1", O_RDWR))) {
        perror("open error");
        exit(-1);
    }

    /* 使用 ftruncate 将 file1 文件截断为长度 0 字节 */
    if (0 > ftruncate(fd, 0)) {
        perror("ftruncate error");
        exit(-1);
    }

    /* 使用 truncate 将 file2 文件截断为长度 1024 字节 */
    if (0 > truncate("./file2", 1024)) {
        perror("truncate error");
        exit(-1);
    }

    /* 关闭 file1 退出程序 */
    close(fd);
    exit(0);
}







int main(void){
    int i, result = 0;
    
    printf("\
1. 示例代码 3.2.1 strerror 测试代码\n\
2. 示例代码 3.2.2 perror 测试代码\n\
3. 示例代码 3.3.1 exit()使用示例\n\
4. 示例代码 3.4.1 空洞文件测试代码\n\
5. 示例代码 3.5.1 O_TRUNC 标志测试\n\
6. 示例代码 3.5.2 O_APPEND 标志测试 1\n\
7. 示例代码 3.6.1 多次打开同一个文件测试代码 1\n\
8. 示例代码 3.6.2 多次打开同一个文件测试代码 2\n\
9. 示例代码 3.6.3 多次打开同一个文件测试代码 3\n\
10.示例代码 3.6.4 多次打开同一个文件测试代码 4\n\
11.示例代码 3.7.1 dup 函数测试代码\n\
12.示例代码 3.7.2 dup2 函数测试代码\n\
13.示例代码 3.9.1 pread 函数测试\n\
14.示例代码 3.10.1 fcntl 复制文件描述符\n\
15.示例代码 3.10.2 fcntl 读取/设置文件状态标志\n\
16.示例代码 3.11.1 文件截断操作\n\
\
");
    
    while(1){
        printf("switch: ");
        scanf("%d", &i);
        switch(i){
            case 1:
                result = test_strerror();
                break;
            case 2:
                result = test_perror();
                break;
            case 3:
                result = test_exit();
                break;
            case 4:
                result = test_holefile();
                break;
            case 5:
                result = test_O_TRUNC();
                break;
            case 6:
                result = test_O_APPEND();
                break;
            case 7:
                result = test_OpenTheSameFileMultipleTimes();
                break;
            case 8:
                result = test_OpenTheSameFileMultipleTimes_2();
                break;
            case 9:
                result = test_IndependentOffsetOfDifferentFilesDescriptors();
                break;
            case 10:
                result = test_O_APPEND_IndependentOffsetOfDifferentFilesDescriptors();
                break;
            case 11:
                result = test_dup();
                break;
            case 12:
                result = test_dup_2();
                break;
            case 13:
                result = test_pread();
                break;
            case 14:
                result = test_fcontl_F_DUPFD();
                break;
            case 15:
                result = test_fcontl_F_GETFL_F_SETFL();
                break;
            case 16:
                result = test_ioctl_truncate_ftruncate();
                break;
        }
    }
    
    return result;
}

