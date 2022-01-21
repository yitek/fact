#!/bin/bash
# 获取并跳转到工作目录
buildScriptFolder=$(dirname "$0")
workspaceFolder="$buildScriptFolder/.."
cd "$workspaceFolder" || exit
workspaceFolder=$(pwd)
echo "enter workspace folder: $workspaceFolder"

buildArch="$1" # 第一个参数，要构建的代码架构类型。w32表示32位windows,x64表示64位linux，后面的脚本可以根据这个参数选择不同的编译器
buildMode="$2" # 第二个参数，接收release/debug。后继脚本根据这个参数添加编译器参数，来生成调试符号
buildType="$3" # 第三个参数，接收static|shared|executable。生成静态库、动态库还是执行库。可以多选。

# 调用编译器生成代码
gcc -ggdb -o calc calc.c