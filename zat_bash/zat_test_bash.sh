#!/bin/bash

# echo -e "# test_bash\n" > "$HOME/test_bash.sh"

# 使用while循环创建一个无限循环
while true; do
    echo "Enter a letter (c, e, x, or r): "
    read -n 1 input  # 读取1个字符

    # case $input in
    #     [exr])
    #         echo "You entered: $input"
    #         ;;
    #     *)
    #         clear
    #         echo "Invalid input. Please enter 'e', 'x', or 'r'."
    #         ;;
    # esac


    if [[ $input == [e] ]]; then    # 编辑
        clear
        echo "编辑"
        vi "$HOME/test_bash.sh"
    fi
    if [[ $input == [c] ]]; then    # 查看
        clear
        echo "查看"
        # cat "$HOME/test_bash.sh" | nl -ba > "$HOME/cat.temp"
        cat "$HOME/test_bash.sh" | nl -n'rn' -w2 -s'  ' -ba > "$HOME/cat.temp"
        # 顺序解读: 右对齐 | 行号保留2位 | 与原始行首间隔2个空格 | 空行也添加行号
        # cat "$HOME/test_bash.sh" | nl -ba > "$HOME/cat.temp"
        cat "$HOME/cat.temp"
        echo "结束"
    fi
    if [[ $input == [x] ]]; then    # 执行
        clear
        echo "Start run."
        chmod 777 "$HOME/test_bash.sh"
        "$HOME/test_bash.sh"
        echo "End run."
    fi
    if [[ $input == [r] ]]; then    # 重置
        clear
        echo "重置"
        echo -e "# test_bash\n" > "$HOME/test_bash.sh"
    fi
done

echo "Loop ended"
