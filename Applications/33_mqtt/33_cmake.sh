cd ./build
current_dir=$(basename "$(pwd)")
if [ "$current_dir" = "build" ]; then
	echo "当前目录: */build"
	echo ""
	~/tools/cmake-3.16.2/bin/cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-linux-setup.cmake -DCMAKE_BUILD_TYPE=Release ../
else
	echo "⚠ 当前目录不是 */build"
fi