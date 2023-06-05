#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x96d13653, "module_layout" },
	{ 0x3bfc989a, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x96312b2b, "class_destroy" },
	{ 0x5c5abd92, "device_destroy" },
	{ 0xfe990052, "gpio_free" },
	{ 0xe248e10a, "gpiod_direction_input" },
	{ 0xe707bb9e, "gpiod_direction_output_raw" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xb6916cd9, "cdev_add" },
	{ 0x2b827688, "cdev_init" },
	{ 0x3a30eeee, "device_create" },
	{ 0x9437905c, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xdcb764ad, "memset" },
	{ 0x835d3bfb, "gpiod_set_raw_value" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x7d37fecd, "gpiod_get_raw_value" },
	{ 0xaaef9376, "gpio_to_desc" },
	{ 0xc5850110, "printk" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "3E5406809BCEB35FD0BCB39");
