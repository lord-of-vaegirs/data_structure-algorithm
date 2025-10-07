#!/bin/bash

# 先编译
./compile.sh

# 如果编译成功，则运行
if [ $? -eq 0 ]; then
    echo "开始运行程序..."
    ./blocktrade
else
    echo "编译失败！"
fi