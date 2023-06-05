obj-m +=  gpio_driver.o

# Kernel Sources
KDIR := $(PWD)/linux

all:
	#make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KDIR) M=$(PWD) modules

clean:
	#make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

# Remember to change device file permissions after insterting the module
# "sudo chmod 666 /dev/mydevice"
