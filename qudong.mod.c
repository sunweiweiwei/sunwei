#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
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
__used __section("__versions") = {
	{ 0xd9726f80, "module_layout" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x37a0cba, "kfree" },
	{ 0x1bcee483, "cdev_del" },
	{ 0xd5f10699, "cdev_add" },
	{ 0x4240b5cb, "cdev_init" },
	{ 0x69ecc112, "kmem_cache_alloc_trace" },
	{ 0x36c11c94, "kmalloc_caches" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0x514011c4, "kthread_stop" },
	{ 0x37874eb8, "wake_up_process" },
	{ 0x40794748, "kthread_create_on_node" },
	{ 0xcf2a6966, "up" },
	{ 0x6bd0e573, "down_interruptible" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0x69af1880, "current_task" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "0D6C773CBB762D0714F42D7");
