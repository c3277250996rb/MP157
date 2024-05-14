#start


export PATH="$PATH:$HOME/ZAT/zat_bash"
if [ "$HOME" = "/home/root" ]; then
	export PATH="$PATH:$HOME/ZAT/obj"
fi
#test

zat(){
	clear
	echo	"		[函数手册]"
	echo	"szl()		source_zat_ls"
	echo	"l()	清空		c()		查看"
	echo	"t()	创建文件	m()		创建目录"
	echo	"rf()	删除文件	rd()		删除目录"
	echo	"s()	进入		w()		父目录"
	echo	"a()	后退		d()		前进"
	echo	"da()	清除栈		i()		查看网络"
	echo	"test_bash()	测试bash"
	echo	""
	echo	""
	echo	""
	echo	""
	echo	""
	echo	""
	echo	""
}

szl(){	# source_zat_ls
	echo "[$1] 的列表:"
	ls -CF
}

l(){
	clear
}

c(){
	cat "$1" | nl -n'rn' -w2 -s'  ' -ba > "$HOME/cat.temp"
	clear && echo "查看" && cat "$HOME/cat.temp"
}

t(){	# 创建文件
	touch "$1" && chmod 777 "$1" && clear && echo "创建文件: $1" && ls -CF
}

m(){	# 创建目录
	mkdir "$1" && clear && echo "创建目录: [$1]" && ls -CF
}

rf(){	#删除文件
	if [ "$2" != "y" ]; then
		echo "rf: 未确认"
		return
	fi
	rm "$1" && clear && echo "删除文件: $1" && ls -CF
}

rd(){	#删除目录
	if [ "$2" != "y" ]; then
		echo "rd: 未确认"
		return
	fi
	rm -r "$1" && clear && echo "删除目录: [$1]" && ls -CF
}

s(){	# 進入
	 pushd "$1" && clear && szl "$1"
}

w(){	# 返回上一級
	pushd .. && clear && szl "$1"
}

a(){	# 後退
	clear && popd && szl "$1"
}

d(){	# 前進
	clear && pushd - && szl "$1"
}

da(){	# 清除棧
	dirs -c && s
}

i(){	# 查看网络ip
	clear && ifconfig
}

test_bash(){
	"$HOME/ZAT/zat_bash/zat_test_bash.sh"
}


#end
