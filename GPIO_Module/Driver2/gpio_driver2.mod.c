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
	{ 0xfe990052, "gpio_free" },
	{ 0x37a0cba, "kfree" },
	{ 0x3a30eeee, "device_create" },
	{ 0x96312b2b, "class_destroy" },
	{ 0x5c5abd92, "device_destroy" },
	{ 0xb6916cd9, "cdev_add" },
	{ 0x2b827688, "cdev_init" },
	{ 0x403f9529, "gpio_request_one" },
	{ 0x61a4da1d, "kmem_cache_alloc_trace" },
	{ 0x79a54ed9, "kmalloc_caches" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x9437905c, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x301fa007, "_raw_spin_unlock" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xdbf17652, "_raw_spin_lock" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x5a460a33, "gpiod_to_irq" },
	{ 0x835d3bfb, "gpiod_set_raw_value" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0xe248e10a, "gpiod_direction_input" },
	{ 0x3812050a, "_raw_spin_unlock_irqrestore" },
	{ 0xe707bb9e, "gpiod_direction_output_raw" },
	{ 0x51760917, "_raw_spin_lock_irqsave" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0xd697e69a, "trace_hardirqs_on" },
	{ 0xc5850110, "printk" },
	{ 0xec3d2e1b, "trace_hardirqs_off" },
	{ 0x9ec6ca96, "ktime_get_real_ts64" },
	{ 0x7d37fecd, "gpiod_get_raw_value" },
	{ 0xaaef9376, "gpio_to_desc" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "C394E44DCD53FF709FE392C");
