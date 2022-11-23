obj-m := hello_drv.o
KDIR := /home/sangjin/DD/linux_kernel/
PWD := $(shell pwd)

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-
all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	-rm *.o *.mod.c .*.cmd modules.order Module.symvers
