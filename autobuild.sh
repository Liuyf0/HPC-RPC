#!/bin/bash

set -e

# 如果没有build目录，创建该目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -rf `pwd`/build/*

cd `pwd`/build &&
    cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 &&
    make

# 回到项目根目录
cd ..

# 把头文件拷贝到 /usr/include/Cppnetwork  so库拷贝到 /usr/lib    PATH
if [ ! -d /usr/include/Cppnetwork ]; then 
    mkdir /usr/include/Cppnetwork
fi

for header in `ls include/*.h`
do
    cp $header /usr/include/Cppnetwork
done

cp `pwd`/lib/libCppnetwork.so /usr/lib

ldconfig