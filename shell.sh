#! /bin/sh

insmod qudong.ko     #加载驱动
mknod /dev/chrdev c 240 0  #创建设备节点

#gcc test.c -o test
./test    #运行测试代码

rmmod qudong.ko  #卸载驱动
