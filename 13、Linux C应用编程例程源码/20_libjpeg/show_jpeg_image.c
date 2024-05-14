/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : show_jpeg_image.c
 作者 : 邓涛
 版本 : V1.0
 描述 : libjpeg使用实战
 其他 : 无
 论坛 : www.openedv.com
 日志 : 初版 V1.0 2021/6/15 邓涛创建
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <jpeglib.h>

typedef struct bgr888_color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} __attribute__ ((packed)) bgr888_t;

static struct fb_fix_screeninfo fb_fix;
static struct fb_var_screeninfo fb_var;
static unsigned short *screen_base = NULL;        //映射后的显存基地址

static int show_jpeg_image(const char *path)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *jpeg_file = NULL;
    bgr888_t *jpeg_line_buf = NULL;     //行缓冲区:用于存储从jpeg文件中解压出来的一行图像数据
    unsigned short *fb_line_buf = NULL; //行缓冲区:用于存储写入到LCD显存的一行数据
    unsigned int min_h, min_w;
    unsigned int valid_bytes;
    unsigned int bytes_per_pixel = fb_var.bits_per_pixel / 8;
    unsigned int width = fb_fix.line_length / bytes_per_pixel;
    unsigned short *base = screen_base;
    int i;

    //绑定默认错误处理函数
    cinfo.err = jpeg_std_error(&jerr);

    //打开.jpeg/.jpg图像文件
    jpeg_file = fopen(path, "r");   //只读方式打开
    if (NULL == jpeg_file) {
        perror("fopen error");
        return -1;
    }

    //创建JPEG解码对象
    jpeg_create_decompress(&cinfo);

    //指定图像文件
    jpeg_stdio_src(&cinfo, jpeg_file);

    //读取图像信息
    jpeg_read_header(&cinfo, TRUE);
    printf("jpeg图像大小: %d*%d\n", cinfo.image_width, cinfo.image_height);

    //设置解码参数
    cinfo.out_color_space = JCS_RGB;//默认就是JCS_RGB
    //cinfo.scale_num = 1;
    //cinfo.scale_denom = 2;

    //开始解码图像
    jpeg_start_decompress(&cinfo);

    //为缓冲区分配内存空间
    jpeg_line_buf = malloc(cinfo.output_components * cinfo.output_width);
    fb_line_buf = malloc(fb_fix.line_length);

    //判断图像和LCD屏那个的分辨率更低
    if (cinfo.output_width > fb_var.xres)
        min_w = fb_var.xres;
    else
        min_w = cinfo.output_width;

    if (cinfo.output_height > fb_var.yres)
        min_h = fb_var.yres;
    else
        min_h = cinfo.output_height;

    //读取数据
    valid_bytes = min_w * bytes_per_pixel;//一行的有效字节数 表示真正写入到LCD显存的一行数据的大小
    while (cinfo.output_scanline < min_h) {

        jpeg_read_scanlines(&cinfo, (unsigned char **)&jpeg_line_buf, 1);//每次读取一行数据

        //将读取到的BGR888数据转为RGB565
        for (i = 0; i < min_w; i++)
            fb_line_buf[i] = ((jpeg_line_buf[i].red & 0xF8) << 8) |
                    ((jpeg_line_buf[i].green & 0xFC) << 3) |
                    ((jpeg_line_buf[i].blue & 0xF8) >> 3);

        memcpy(base, fb_line_buf, valid_bytes);
        base += width;//+width  定位到LCD下一行显存地址的起点
    }

    //解码完成
    jpeg_finish_decompress(&cinfo); //完成解码
    jpeg_destroy_decompress(&cinfo);//销毁JPEG解码对象、释放资源

    //关闭文件、释放内存
    fclose(jpeg_file);
    free(fb_line_buf);
    free(jpeg_line_buf);
    return 0;
}

int main(int argc, char *argv[])
{
    unsigned int screen_size;
    int fd;

    /* 传参校验 */
    if (2 != argc) {
        fprintf(stderr, "usage: %s <jpeg_file>\n", argv[0]);
        exit(-1);
    }

    /* 打开framebuffer设备 */
    if (0 > (fd = open("/dev/fb0", O_RDWR))) {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    /* 获取参数信息 */
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);
    screen_size = fb_fix.line_length * fb_var.yres;

    /* 将显示缓冲区映射到进程地址空间 */
    screen_base = mmap(NULL, screen_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (MAP_FAILED == (void *)screen_base) {
        perror("mmap error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* 显示BMP图片 */
    memset(screen_base, 0xFF, screen_size);
    show_jpeg_image(argv[1]);

    /* 退出 */
    munmap(screen_base, screen_size);  //取消映射
    close(fd);  //关闭文件
    exit(EXIT_SUCCESS);    //退出进程
}
