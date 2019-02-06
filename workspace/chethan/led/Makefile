obj-m+=led_driver.o
#obj-m+=serial.o


all: 
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/chethan/workspace/kernerl_ws/linux M=$(PWD) modules
clean :
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/chethan/workspace/kernerl_ws/linux M=$(PWD) clean
