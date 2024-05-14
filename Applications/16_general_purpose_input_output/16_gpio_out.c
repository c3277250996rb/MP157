#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static char gpio_path[100];

static int ze(){
	exit(-1);
}

static int gpio_config(const char *attribute, const char *value){
/*
	在C语言中，const修饰符用于声明常量。它告诉编译器，标识符被声明为只读，即它的值不能被修改。这样做有几个好处：
1.安全性和可维护性： 声明一个值为常量可以防止意外的修改，这在大型项目中特别有用。如果一个值在程序的不同部分被多次使用，通过将其声明为常量，你可以确保它的值不会被无意中更改，从而提高代码的可维护性和安全性。
2.编译器优化： 编译器可以利用const声明来进行优化。对于const修饰的变量，编译器可能会将它们存储在只读内存区域中，这样可以提高程序的执行效率。
3.更好的代码理解： 使用const可以让代码更易于理解。当你看到一个变量被声明为常量时，你知道它的值在程序执行期间不会改变，这有助于更清晰地理解代码。
*/
	char file_path[100];
	int length;
	int descriptor;

	sprintf(file_path, "%s/%s", gpio_path, attribute);
	if(0 > (descriptor = open(file_path, O_WRONLY))){	//打开
		perror("open error");
		return descriptor;
	}

	length = strlen(value);
	if(length != write(descriptor, value, length)){	//写入
		perror("write error");
		close(descriptor);
		return -1;
	}

	close(descriptor);	//关闭
	return 0;
}

int main(int argc,char *argv[]){
	char file_path[100];
	int value;
	int descriptor;

	/* 校验传参 */
	if(3 != argc){
		fprintf(stderr, "usage: %s <gpio> <value>\n", argv[0]);
/*
	fprintf:	printf to file.
	stderr:		standard error.
*/
		ze();
	}

	/* 判断指定编号的GPIO是否导出 */
    sprintf(gpio_path, "/sys/class/gpio/gpio%s", argv[1]);
    if(access(gpio_path, F_OK) > 0){	//如果目录不存在 则需要导出
/*
	在C语言中，`access()` 函数用于检查指定路径的文件或目录是否存在以及当前进程是否具有对其的访问权限。它的原型如下：
	int access(const char *path, int mode);
	其中，`path` 参数是要检查的文件或目录的路径，而 `mode` 参数是要进行的操作，通常是 `F_OK` 或者下面列出的几个值的按位或组合：
	- `F_OK`：用于检查文件是否存在。
	- `R_OK`：用于检查文件是否可读。
	- `W_OK`：用于检查文件是否可写。
	- `X_OK`：用于检查文件是否可执行。
*/
    	int length;

    	if(0 > (descriptor = open("/sys/class/gpio/export", O_WRONLY))){
    		perror("open error");
    		ze();
    	}

    	length = strlen(argv[1]);
    	if(length != write(descriptor, argv[1], length)){	//导出gpio
    		perror("write error");
    		close(descriptor);
    		ze();
    	}

    	close(descriptor);
    }

    /* 配置为输出模式 */
    if(gpio_config("direction", "out")){
    	ze();
    }

    /* 极性设置 */
    if(gpio_config("active_low", 0)){
    	ze();
    }

    /* 控制GPIO输出高低电平 */
    if(gpio_config("value", argv[2])){
    	ze();
    }

    ze();
}