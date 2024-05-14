# 获取源文件名（不带路径和扩展名）
source_file="$1"
filename=$(basename -- "$source_file")
filename_no_ext="${filename%.*}"

${CC} -I/home/alientek/tools/tslib/include -L/home/alientek/tools/tslib/lib -lts -o filename_no_ext "$1"
ls -CF