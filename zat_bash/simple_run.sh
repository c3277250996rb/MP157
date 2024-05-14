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

# 编译源文件为可执行文件
gcc -o "$filename_no_ext" "$source_file"

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "Compilation successful."
    # echo "Executable: $directory/$filename_no_ext"
    echo "Run: $filename_no_ext."
    ./"$filename_no_ext"
else
    echo "Compilation failed."
fi