#!/bin/bash
# 获取并跳转到工作目录
buildScriptFolder=$(dirname "$0")
workspaceFolder="$buildScriptFolder/.."
cd "$workspaceFolder" || exit
workspaceFolder=$(pwd)

# 获取构建参数
source "$buildScriptFolder/build-variables.sh"


# 调用编译器生成代码
$CC -ggdb -o calc calc.c