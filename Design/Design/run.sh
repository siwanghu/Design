#!/bin/bash

yes="y"
no="n"
matrix="1"
max="2"
sort="3"
clear
echo "...............需要图形化测试界面吗?(n/y)................"
read console
clear
if [ $console == $no ]
then
	clear
	echo "...............请输入操作(1.matrix/2.max/3.sort)!................"
	read operation
	if [ $operation == $matrix ]
	then
		cd Matrix
		./Matrix.sh
	fi
	if [ $operation == $max ]
	then
		cd Max
		./Max.sh
	fi
	if [ $operation == $sort ]
	then
		cd Sort
		./Sort.sh
	fi
fi
if [ $console == $yes ]
then
	cd UI
	gcc ./UI.c -o ./UI `pkg-config --cflags --libs gtk+-2.0`
	clear
	./UI
fi
