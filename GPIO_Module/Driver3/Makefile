obj-m +=  gpio_driver3.o

# Kernel Sources
KDIR := $(PWD)/../linux

all:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

# Remember to change device file permissions after insterting the module
# "sudo chmod 666 /dev/mydevice"
