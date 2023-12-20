#!/bin/sh

#编译可执行文件
make clean;make

#设置libxx.so库搜索路径
export LD_LIBRARY_PATH=$(pwd)../lib:$LD_LIBRARY_PATH
#export PATH=$(pwd)/../lib:$PATH
echo $LD_LIBRARY_PATH

