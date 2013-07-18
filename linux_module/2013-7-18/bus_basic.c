#include<linux/device.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/string.h>

MODULE_AUTHOR("young.yang");
MODULE_LICENSE("Dual BSD/GPL");

static char *Version = "$Revision:1.0$";
static int yy_device_match(struct device *dev, struct device_driver *driver)
{
	printk("\nyy_device_match,driver->name=%s\n",driver->name);
	return 0;
}
struct bus_type yy_bus_type = {
	.name		= "yy",
	.match		= yy_device_match,
//	.probe		= yy_device_probe,
//	.remove		= yy_device_remove,
//	.shutdown	= yy_device_shutdown,
//	.suspend	= yy_device_suspend,
//	.resume		= yy_device_resume,
};
static ssize_t show_bus_version(struct bus_type *bus,char *buf)
{
	return snprintf(buf,PAGE_SIZE,"%s\n",Version);
}

static BUS_ATTR(version,S_IRUGO,show_bus_version,NULL);


static int __init my_bus_init(void)
{
	
	int retval;

	retval = bus_register(&yy_bus_type);
	if (retval)
		return retval;

	if(bus_create_file(&yy_bus_type,&bus_attr_version))
		printk(KERN_NOTICE "Fail to create version attribute!\n");
	return retval;
}
static void my_bus_exit(void)
{
	bus_unregister(&yy_bus_type);
}
module_init(my_bus_init);
module_exit(my_bus_exit);

