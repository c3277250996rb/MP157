#!/bin/bash

# 检查参数是否正确
if [ $# -ne 1 ]; then
    echo "Usage: $0 <source_file>"
    exit 1
fi

# 获取源文件名（不带路径和扩展名）
source_file="$1"
filename=$(basename -- "$source_file")
filename_no_ext="${filename%.*}"

# 获取当前路径的父目录路径
parent_dir=$(dirname "$(pwd)")

# 获取目录路径
directory="$parent_dir/obj"

# 检查目录是否存在
if [ ! -d "$directory" ]; then
    mkdir "$parent_dir/obj"
    echo "Directory $directory create successfully."
fi

# 编译源文件为可执行文件
gcc -o "$parent_dir/obj/$filename_no_ext" "$source_file"

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "Compilation successful."
    # echo "Executable: $directory/$filename_no_ext"
else
    # 检查目录是否为空
    if [ "$(ls -A "$directory")" ]; then
        echo "Error: $directory is not empty."
    else    
        # 删除空目录
        rmdir "$directory"
        # 检查删除是否成功
        if [ $? -eq 0 ]; then
            echo "Directory $directory deleted successfully."
        else
            echo "Error: Failed to delete directory $directory."
        fi
    fi

    echo "Compilation failed."
fi
