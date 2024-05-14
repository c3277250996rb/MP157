#!/bin/bash

prefix="prefix_string"

# 指定要查找的目录和字符串
search_dir="/home/alientek/ZAT/13、Linux C应用编程例程源码"

# if [ $# -eq 2 ]; then
#     prefix=$2

    
#     # 使用 find 命令查找以指定字符串开头的目录
#     result=$(find "$search_dir" -type d -name "$prefix*" -print -quit)
    
#     # 检查是否找到目录
#     if [ -n "$result" ]; then
#         # 进入找到的目录
#         cd "$result"
#         echo "进入目录：$result/app"
#     else
#         echo "未找到以 $prefix 开头的目录。"
#     fi

#     exit 1
# else
    cd "$search_dir"
# fi