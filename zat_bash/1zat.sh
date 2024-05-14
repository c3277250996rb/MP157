# ########## #
# 2024/04/12 #
# ########## #

# compile_C.sh "$1"

# if [ $? -eq 0 ]; then
#     if [ $# -eq 2 ]; then
#     	if [ $2 -eq 1 ]; then
#     		source_file="$1"
# 			filename=$(basename -- "$source_file")
# 			filename_no_ext="${filename%.*}"
# 			echo "Excute: $filename_no_ext"
#     		../obj/$filename_no_ext
#     	fi
#     fi
# fi

# ############# #
# 2024/04/13 17 #
# ############# #

# compile_share_obj.sh "$1"

# ############# #
# 2024/04/22 13 #
# ############# #

compile_share_obj_continuously.sh "$1" "$2"