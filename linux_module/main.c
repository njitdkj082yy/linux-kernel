#include<linux/device.h>
static int kobj_test_init()
{
	printk("kobj test init\n");
	return 0;
}
static int kobj_test_exit()
{
	printk("kobj test exit\n");
	return 0;
}
module_init(kobj_test_init);
module_exit(kobj_test_exit);