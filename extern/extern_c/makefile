main: add.o addcpp.o
	g++ add.o addcpp.o -o main
add.o:add.c
  #-Wall 可以看到所有的警告
  #-g 可以调试
  #-c 只允许执行到汇编步骤，不允许链接。
  #写gcc命令时候，前面要tab按键一下
	gcc add.c -c -Wall -g -o add.o
addcpp.o:add.cpp
	g++ add.cpp -c -Wall -g -o addcpp.o
clean:
	rm *.o && rm main
