#!/bin/bash

# 检查参数是否正确
if [ $# -ne 1 ] && [ $# -ne 2 ]; then
    echo "Usage: $0 <source_file> (mode)"
    exit 1
fi

# 在Bash中，eq是用于整数比较的操作符，而不是用于字符串比较的。要进行字符串比较，你应该使用=或==操作符。
if [ $2 = 1 ]; then
	# 不应该在变量赋值时使用空格。
	i=0
	j=""
	k=0
	while [[ true ]]; do
		sleep 1

		case $((i % 4)) in
        0) printf "| ";;
        1) printf "/ ";;
        2) printf "%s" "| ";;
        3) printf "\\ ";;
    	esac

		printf "%s [$i]: "
		# compile_share_obj.sh "$1"
		make
		echo ""
		((i++))
	done
fi

compile_share_obj.sh "$1"
