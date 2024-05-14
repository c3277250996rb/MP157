#!/bin/bash

if [ $# != 1 ]; then
	echo "usage $0 <path>"
	exit
fi

# 将目录路径替换为你想要修改权限的目录
directory="$1"

# 确保目录存在
if [ -d "$directory" ]; then
    # 修改目录下所有文件的权限为 777
    chmod -R 777 "$directory"
    echo "所有文件的权限已经修改为 777。"
else
    echo "目录不存在，请提供正确的目录路径。"
fi

