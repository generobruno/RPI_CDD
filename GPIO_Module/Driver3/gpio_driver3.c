#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <uapi/asm-generic/errno-base.h>
#include <linux/string.h>

/* Macros */
#define DRIVER_NAME "GPIO_Driver"
#define DRIVER_CLASS "MyDriverClass"
#define NUM_GPIO_PINS 21
#define MAX_GPIO_NUMBER 32
#define DEVICE_NAME "raspi-gpio"
#define BUF_SIZE 512

/* User data types */
enum state {low, high};
enum direction {in, out};

/**
 * @brief Estructura para un pin GPIO
 */
struct raspi_gpio_dev {
    struct cdev cdev;
    struct gpio pin;
    enum state state;
    enum direction dir;
};

/* Variables for device and device class */
struct raspi_gpio_dev *raspi_gpio_devp[NUM_GPIO_PINS];
static dev_t first;
static struct class *raspi_gpio_class;

/**
 * @brief Read data out of the buffer
 */
static ssize_t driver_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) {
	unsigned int gpio;
    ssize_t retval;
    char byte;

    gpio = iminor(filp->f_path.dentry->d_inode);
    for(retval = 0; retval < count; retval++) {
        byte = '0' + gpio_get_value(gpio);
        if(put_user(byte, buf + retval))
            break;
    }

    return retval;
}

/**
 * @brief Write data to the buffer
 */
static ssize_t driver_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
	unsigned int gpio, len = 0, value = 0;
    char kbuf[BUF_SIZE];
    struct raspi_gpio_dev *raspi_gpio_devp = filp->private_data;

    gpio = iminor(filp->f_path.dentry->d_inode);

    len = count < BUF_SIZE ? count-1 : BUF_SIZE-1;
    if(raw_copy_from_user(kbuf, buf, len) != 0) {
        return -EFAULT;
    }
    kbuf[len] = '\0';

    printk(KERN_INFO "Request from user: %s\n", kbuf);

    // Check the content of kbuf and set GPIO pin accordingly
    if (strcmp(kbuf, "out") == 0) {
        printk(KERN_ALERT "Set GPIO[%d] direction: ouput\n", gpio);
        if (raspi_gpio_devp->dir != out) {
            gpio_direction_output(gpio, low);
            raspi_gpio_devp->dir = out;
            raspi_gpio_devp->state = low;
        }
    } else if (strcmp(kbuf, "in") == 0) {
        if (raspi_gpio_devp->dir != in) {
            printk(KERN_INFO "Set GPIO[%d] direction: input\n", gpio);
            gpio_direction_input(gpio);
            raspi_gpio_devp->dir = in;
        }
    }  else if ((strcmp(kbuf, "1") == 0) || (strcmp(kbuf, "0") == 0)) {
        sscanf(kbuf, "%d", &value);

        if (raspi_gpio_devp->dir == in) {
            printk("Cannot set GPIO %d, direction: input\n", gpio);
            return -EPERM;
        }

        if (raspi_gpio_devp->dir == out) {
            if (value > 0) {
                gpio_set_value(gpio, high);
                raspi_gpio_devp->state = high;
            } else {
                gpio_set_value(gpio, low);
                raspi_gpio_devp->state = low;
            }
        }
    } else {
        printk(KERN_ERR "Invalid value\n");
        return -EINVAL;
    }

    *f_pos += count;
    return count;
}

/**
 * @brief This function is called when the device file is opened
 */
static int driver_open(struct inode *inode, struct file *filp) {
	struct raspi_gpio_dev *raspi_gpio_devp;
    unsigned int gpio;

    gpio = iminor(inode);
    printk(KERN_INFO "GPIO[%d] opened\n", gpio);
    raspi_gpio_devp = container_of(inode->i_cdev, struct raspi_gpio_dev, cdev);

    filp->private_data = raspi_gpio_devp;

    return 0;
}

/**
 * @brief This function is called when the device file is closed
 */
static int driver_close(struct inode *inode, struct file *filp) {
    struct raspi_gpio_dev *raspi_gpio_devp;
    unsigned int gpio;

    raspi_gpio_devp = container_of(inode->i_cdev, struct raspi_gpio_dev, cdev);
    gpio = iminor(inode);
    printk(KERN_INFO "GPIO[%d] closed\n", gpio);

    return 0;
}	

static struct file_operations raspi_gpio_fops = {
	.owner		= THIS_MODULE,
	.open		= driver_open,
	.release	= driver_close,
	.read 		= driver_read,
	.write		= driver_write,
};

static int __init ModuleInit(void) {
    int i, ret, index = 0;

    printk("Inserting GPIO Kernel Module...\n");

    if(alloc_chrdev_region(&first, 0, NUM_GPIO_PINS, DEVICE_NAME) < 0) {
        printk(KERN_DEBUG "Cannot Register Device.\n");
        return -1;
    }

    if((raspi_gpio_class = class_create(THIS_MODULE, DEVICE_NAME)) == NULL) {
        printk(KERN_DEBUG "Cannot Create Class %s.\n", DEVICE_NAME);
        unregister_chrdev_region(first, NUM_GPIO_PINS);
        return -1;
    }

    for(i = 0; i < MAX_GPIO_NUMBER; i++) {
        if(i != 0 && i != 1 && i != 5 && i != 6 &&
            i != 12 && i != 13 && i != 16 && i != 19 &&
            i != 20 && i != 21 && i != 26 ) {

                raspi_gpio_devp[index] = kmalloc(sizeof(struct raspi_gpio_dev), GFP_KERNEL);

                if(!raspi_gpio_devp[index]) {
                    printk("Bad kmalloc\n");
                    return -ENOMEM;
                }

                if(gpio_request_one(i, GPIOF_OUT_INIT_LOW, NULL) < 0) {
                    printk(KERN_ALERT "Error requesting GPIO %d\n", i);
                    return -ENODEV;
                }

                raspi_gpio_devp[index]->dir = out;
                raspi_gpio_devp[index]->state = low;
                raspi_gpio_devp[index]->cdev.owner = THIS_MODULE;

                cdev_init(&raspi_gpio_devp[index]->cdev, &raspi_gpio_fops);

                if ((ret = cdev_add(&raspi_gpio_devp[index]->cdev, (first + i), 1))) {
                    printk (KERN_ALERT "Error %d adding cdev\n", ret);
                    for (i = 0; i < MAX_GPIO_NUMBER; i++) {
                        if (    i != 0 && i != 1 && i != 5 && i != 6 &&
                                i != 12 && i != 13 && i != 16 && i != 19 &&
                                i != 20 && i != 21 && i != 26   ) {
                            device_destroy (raspi_gpio_class, MKDEV(MAJOR(first), MINOR(first) + i));
                        }
                    }
                    class_destroy(raspi_gpio_class);
                    unregister_chrdev_region(first, NUM_GPIO_PINS);
                    return ret;
                }

                if (device_create( raspi_gpio_class, NULL, MKDEV(MAJOR(first), MINOR(first)+i), NULL, "raspiGpio%d", i) == NULL) {
                    class_destroy(raspi_gpio_class);
                    unregister_chrdev_region(first, NUM_GPIO_PINS);
                    return -1;
                }

                index++;
            }
    }

    printk("RaspberryPi GPIO Driver Initialized.\n");

    return 0;
}

static void __exit ModuleExit(void) {
    int i = 0;

    unregister_chrdev_region(first, NUM_GPIO_PINS);

    for (i = 0; i < NUM_GPIO_PINS; i++)
        kfree(raspi_gpio_devp[i]);

    for (i = 0; i < MAX_GPIO_NUMBER; i++) {
        if (    i != 0 && i != 1 && i != 5 && i != 6 &&
                i != 12 && i != 13 && i != 16 && i != 19 &&
                i != 20 && i != 21 && i != 26   ) {
            gpio_direction_output(i, 0);
            device_destroy ( raspi_gpio_class,
            MKDEV(MAJOR(first), MINOR(first) + i));
            gpio_free(i);
        }
    }
    
    class_destroy(raspi_gpio_class);
    printk(KERN_INFO "RaspberryPi GPIO driver removed\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pablo R. Picasso");
MODULE_DESCRIPTION("GPIO Example DDC");
