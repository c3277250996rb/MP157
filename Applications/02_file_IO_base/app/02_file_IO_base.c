#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
	char buffer[3];
	int identifier_1;
	int identifier_2;
	int result;

	/* 打开 src_file 文件 */
	identifier_1 = open("../src_file", O_RDONLY);
	if(identifier_1 == -1){
		printf("Error: 01\n");
		return -1;
	}

    /* 新建 dest_file 文件并打开 */
    identifier_2 = open("../dest_file",	O_WRONLY | 
    									O_CREAT | 
    									O_EXCL,		//exlusive access 独占访问, 通常与 O_CREAT 一起使用，O_CREAT 用于创建文件，而 O_EXCL 用于确保文件的独占性，即保证文件在被创建时不存在，避免竞争条件。
    									S_IRWXU | 	//U 7
    									S_IRGRP |	//G 4
    									S_IROTH);	//O 4
    if(identifier_2 == -1){
    	printf("Error: 02\n");
    	result -1;
    	goto Error_1;
    }

    /* 将 src_file 文件读写位置移动到偏移文件头 10 个字节处 */
    result = lseek(identifier_1, 10, SEEK_SET);
    if(result == -1){
    	printf("Error: 03\n");
    	goto Error_2;
    }

    /* 读取 src_file 文件数据，大小 3Byte */
    result = read(identifier_1, buffer, sizeof(buffer));
    if(result == -1){
    	printf("Error: 04\n");
    	goto Error_2;
    }

    /* 将 dest_file 文件读写位置移动到文件头 */
    result = lseek(identifier_2, 0, SEEK_SET);
    if(result == -1){
    	printf("Error: 05\n");
    	goto Error_2;
    }

    /* 将 buffer 中的数据写入 dest_file 文件，大小 1KByte */
    result = write(identifier_2, buffer, sizeof(buffer));
    if(result == -1){
    	printf("Error: 06\n");
    }

    printf("OK: test successful\n");
    result = 0;

Error_2:
	close(identifier_2);
Error_1:
	close(identifier_1);

	return result;
}