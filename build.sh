function clear_bin_files() 
{
	find -type f ! -name "*.so" ! -name "*.rl" ! -name "config_system.json" ! -name "task.json" -delete 
	rm -rf CMakeFiles
	rm -rf .cmake
}
function build() 
{
	cmake -DCMAKE_CXX_COMPILER=clang++ $1
	make -j${11}
	clear_bin_files
}
if [ -d "release" ]; then
	cd ./release 
else 
	mkdir ./release
fi
case "$1" in 
	"-t")
		build ../TasksAlgoritm 
		;;
	"-p")
		build ../Proxy 
		;;
	"-c")
		build ../CoreAlgoritm 
		;;
	"-a")
		build ../AdamAlgoritm 
		;;
	"-l")
		git clone git@github.com:yaroslavsin11092004/MathPlot.git 
		build ./MathPlot 
		rm -rf ./MathPlot
		;;
	"-as")
		build ../TasksAlgoritm
		build ../Proxy
		build ../CoreAlgoritm
		build ../AdamAlgoritm
		;;
esac
