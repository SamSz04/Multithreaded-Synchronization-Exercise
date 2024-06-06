## 多线程同步练习

### 项目简介

该项目旨在实现一个多线程环境下的资源初始化类，确保资源只被初始化一次，并且每个线程在调用完成时可以确定资源已经初始化完毕。

### 目录结构

````sh
.
├── main.cc
├── waiting_once_api.hpp
├── waiting_once.cc
├── Makefile
└── README.md
````

### 编译和运行

````sh
# 要求在 Linux 环境下进行
# 使用以下命令编译项目：
make
# 编译成功后，使用以下命令运行程序：
./main
````

### 样例输出

````sh
Testing waiting_once:
ThreadNum: 1    Time: 0.471172s.  Throughput: 21223666 ops/s
ThreadNum: 2    Time: 1.89998s.  Throughput: 10526439 ops/s
ThreadNum: 4    Time: 4.54659s.  Throughput: 8797807 ops/s
ThreadNum: 8    Time: 8.74454s.  Throughput: 9148560 ops/s
Testing optimized_waiting_once:
ThreadNum: 1    Time: 0.265767s.  Throughput: 37626938 ops/s
ThreadNum: 2    Time: 0.309249s.  Throughput: 64672738 ops/s
ThreadNum: 4    Time: 0.53941s.  Throughput: 74155041 ops/s
ThreadNum: 8    Time: 1.0467s.  Throughput: 76430695 ops/s
````

