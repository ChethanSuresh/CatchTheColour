obj-m+=mm_taskstruct.o


all: 
	#make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/chethan/workspace/kernerl_ws/linux M=$(PWD) modules
	make -Wdeclaration-after-statement -C /usr/src/linux-headers-4.15.0-43-generic M=$(PWD) modules
	
clean :
	#make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/chethan/workspace/kernerl_ws/linux M=$(PWD) clean
	make -C /usr/src/linux-headers-4.15.0-43-generic M=$(PWD) clean
	
