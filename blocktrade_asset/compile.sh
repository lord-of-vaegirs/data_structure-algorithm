#!/bin/bash

# 编译命令
g++ -std=c++17 -mcmodel=large -g -O0 \
    -I./external/csv-parser/single_include \
    blocktrade.cpp \
    -o blocktrade

echo "编译完成！"
echo "运行程序: ./blocktrade"