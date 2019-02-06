#obj-m+=btn.o
obj-m+=button.o
obj-m+=btn.o

all: 
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/chethan/workspace/kernerl_ws/linux M=$(PWD) modules
	arm-linux-gnueabihf-gcc -o sig_closeapp sig_closeapp.c 
clean :
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/chethan/workspace/kernerl_ws/linux M=$(PWD) clean
	rm sig_closeapp 
