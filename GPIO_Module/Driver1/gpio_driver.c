#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#define DRIVER_NAME "GPIO_Driver"
#define DRIVER_CLASS "MyDriverClass"

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pablo R. Picasso");
MODULE_DESCRIPTION("GPIO Example DDC");

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

/**
 * @brief Read data out of the buffer
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offset) {
	int to_copy, not_copied, delta;
	char tmp[3] = " \n";

	/* Get amount of data to copy */
	to_copy = min(count, sizeof(tmp));

	/* Read value of button */
	printk("Value of button: %d\n", gpio_get_value(17));
	tmp[0] = gpio_get_value(17) + '0';

	/* Copy data to user */
	not_copied = copy_to_user(user_buffer, &tmp, to_copy);

	/* Calculate data */
	delta = to_copy - not_copied;
	return delta;
}

/**
 * @brief Write data to the buffer
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offset) {
	int to_copy, not_copied, delta;
	char value;

	/* Get amount of data to copy */
	to_copy = min(count, sizeof(value));

	/* Copy data to user */
	not_copied = copy_from_user(&value, user_buffer, to_copy);
	
	value = user_buffer[0];
	
	printk("Copying: %c", value);
	
	/* Setting the LED */
	switch(value) {
		case '0':
			gpio_set_value(4, 0);
			break;
		case '1':
			gpio_set_value(4, 1);
			break;
		default:
			printk("Invalid Input\n");
			break;
	}

	/* Calculate data */
	delta = to_copy - not_copied;
	return delta;
}

/**
 * @brief This function is called when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance) {
	printk("dev_nr - open was called!\n");
	return 0;
}	

/**
 * @brief This function is called when the device file is closed
 */
static int driver_close(struct inode *device_file, struct file *instance) {
	printk("dev_nr - closed was called!\n");
	return 0;
}	

static struct file_operations fops = {
	.owner		= THIS_MODULE,
	.open		= driver_open,
	.release	= driver_close,
	.read 		= driver_read,
	.write		= driver_write
};

static int __init ModuleInit(void) {
	printk("Hello, Kernel!\n");
	
	/* Allocate a free device number */
	if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device nr. could not be allocated.\n");
		return -1;
	}

	printk("Driver - Device Number Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

	/* Create a device class */
	if( (my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL ) {
		printk("Device class can't be created.\n");
		goto ClassError;
	}

	/* Create device file */
	if( device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL ) {
		printk("Could not create device file.\n");
		goto FileError;
	}

	/* Initialize device file */
	cdev_init(&my_device, &fops);

	/* Register device to kernel */
	if( cdev_add(&my_device, my_device_nr, 1) == -1 ) {
		printk("Registering of device to kernel failed.\n");
		goto AddError;
	}


	/*
	 	RASPBERRY INIT CONFIG
	*/

	/* GPIO 4 init */
	if(gpio_request(4, "rpi-gpio-4")) {
		printk("Can't allocate GPIO 4\n");
		goto AddError;
	}

	/* Set GPIO Direction */
	if(gpio_direction_output(4, 0)) {
		printk("Cant set GPIO 4 to OUTPUT\n");
		goto Gpio4Error;
	}
	
	/* GPIO 17 init */
	if(gpio_request(17, "rpi-gpio-17")) {
		printk("Can't allocate GPIO 17\n");
		goto Gpio4Error;
	}

	/* Set GPIO Direction */
	if(gpio_direction_input(17)) {
		printk("Cant set GPIO 17 to INPUT\n");
		goto Gpio17Error;
	}

	/* GPIO 21 init */
	if(gpio_request(21, "rpi-gpio-21")) {
		printk("Can't allocate GPIO 21\n");
		goto Gpio17Error;
	}

	/* Set GPIO Direction */
	if(gpio_direction_input(21)) {
		printk("Cant set GPIO 21 to INPUT\n");
		goto Gpio21Error;
	}

	return 0;
Gpio21Error:
	gpio_free(21);
Gpio17Error:
	gpio_free(17);
Gpio4Error:
	gpio_free(4);
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);	
	return -1;
}

static void __exit ModuleExit(void) {
	/* Free GPIO */
	gpio_set_value(4, 0);
	gpio_free(21);
	gpio_free(17);
	gpio_free(4);

	/* Unregister Device Number */
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	
	unregister_chrdev_region(my_device_nr, 1);	

	printk("Goodbye, Kernel!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
