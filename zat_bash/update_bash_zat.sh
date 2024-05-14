#!/bin/bash

# 检查参数是否正确
# if [ $# -ne 2 ]; then
#     echo "Usage: $0 source_file target_file"
#     exit 1
# fi

# 设置变量
target_file="$HOME/ZAT/zat_bash/zat_source.sh"
source_file="$HOME/.bashrc"

#start
#end

# 读取目标文件的内容
replacement_text=$(<"$target_file")

if [ -z "$replacement_text" ]; then
    echo "The text to replace is empty."
    exit
fi


# 使用 awk 替换源文件中以#start开头并以#end结尾的多行文本
awk -v replacement="$replacement_text" '
    /^#start$/ { print replacement; found = 1; next }
    /^#end$/ && found { found = 0; next }
    { if (!found) print }
' "$source_file" > temp_file && mv temp_file "$source_file"

if [ $? -eq 0 ]; then
    echo "Update completed."
else
    echo "Error: Failed to update."
fi

# 刷新~/.bashrc
if [ $? -eq 0 ]; then
    source ~/.bashrc
    if [ $? -eq 0 ]; then
        echo "Source ~/.bashrc successfully."
    else 
        echo "Error: Failed to source."
    fi
fi
