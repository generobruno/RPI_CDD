obj-m +=  gpio_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

# Remember to change device file permissions after insterting the module
# "sudo chmod 666 /dev/mydevice"
