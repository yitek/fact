#!/bin/bash
build_arch=$1
# platform
if [[ "$build_arch" =~ "w" ]]; then 
	build_platform=win
	build_staticSuffix=".lib"
	build_sharedSuffix=".dll"
	build_executableSuffix=".exe"
elif [[ "$build_arch" =~ "x" ]]; then
	build_platform=linux
	build_staticSuffix=".a"
	build_sharedSuffix=".so"
	build_executableSuffix=""
elif [[ "$build_arch" =~ "a" ]]; then
	build_platform=android
	build_staticSuffix=".a"
	build_sharedSuffix=".so"
	build_executableSuffix=""
elif [[ "$build_arch" =~ "m" ]]; then
	build_platform=mac
	build_staticSuffix=".a"
	build_sharedSuffix=".so"
	build_executableSuffix=""
else 
	build_platform=""
fi
if [ "$build_platform" == "" ]; then
	return
fi

echo "### Build variables"
printf "| varname\t\t| value\t| description\t|\n"
printf "| ----\t\t\t| ----\t| ----\t\t|\n"
printf "| build_platform\t| %s\t| 编译的目标操作系统:win,linux,android(arm),mac |\n" "$build_platform"

# bits
if [[ "$build_arch" =~ "32" ]]; then
	build_bits="32"
elif [[ "$build_arch" =~ "64" ]]; then
	build_bits="64"
else 
	build_bits="64"
fi
printf "| build_bits\t\t| %s\t| 编译目标的位数:32,64 |\n" "$build_bits"

# build mode
if [ "$2" == "debug" ] || [ "$2" == "release" ]; then
	build_mode=$2
else 
	build_mode=debug
fi
printf "| build_mode\t\t| %s\t| 编译类型: debug/release |\n" "$build_mode"

# suffixes
printf "| build_executableSuffix| %s\t| 可执行文件后缀 |\n" "$build_executableSuffix"
printf "| build_sharedSuffix\t| %s\t| 动态库文件后缀 |\n" "$build_sharedSuffix"
printf "| build_staticSuffix\t| %s\t| 静态库文件后缀 |\n" "$build_staticSuffix"
echo ""

# 编译器设置
echo "### Compiler variables"
if [ "$build_platform" == "win" ]; then
	if [ "$build_bits" == "64" ]; then
		export CC=/usr/bin/x86_64-w64-mingw32-gcc.exe
		export CXX=/usr/bin/x86_64-w64-mingw32-g++.exe 
	else
		export CC=/usr/bin/i686-w64-mingw32-gcc.exe
		export CXX=/usr/bin/i686-w64-mingw32-g++.exe 
	fi
else 
	echo "$build_arch is not suported yet"  || return
fi
printf "| varname\t| value\t\t\t\t\t| description\t|\n"
printf "| ----\t\t| ----\t\t\t\t\t| ----\t\t|\n"
printf "| CC\t\t| %s\t| C Compiler |\n" $CC
printf "| CXX\t\t| %s\t| C++ Compiler |\n" $CXX
echo ""


# 目录设置
echo "### Folder variables"
printf "| varname\t\t| value\t\t\t\t\t| description\t|\n"
printf "| ----\t\t\t| ----\t\t\t\t\t| ----\t\t|\n"
printf "| workspaceFolder\t| %s\t| 项目工作目录 |\n" "$workspaceFolder"