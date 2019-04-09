obj-m+=morse_code.o

all:
	make -C ../pi-headers/ M=$(PWD) modules ARCH=arm CROSS_COMPILE=../tools/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
clean:
	make -C ../pi-headers/ M=$(PWD) clean
