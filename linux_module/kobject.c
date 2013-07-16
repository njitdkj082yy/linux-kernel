#include<linux/device.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/string.h>
#include<linux/sysfs.h>
#include<linux/stat.h>
MODULE_AUTHOR("young.yang");
MODULE_LICENSE("Dual BSD/GPL");

void obj_test_release(struct kobject *kobject);
ssize_t kobj_test_show(struct kobject *kobject,struct attribute *attr,char *buf);
ssize_t kobj_test_store(struct kobject *kobject,struct attribute *attr,const char *buf,size_t count);

struct attribute kobj_config1={
	.name = "kobj_config1",
	.mode = S_IRWXUGO,
};
struct attribute kobj_config2={
	.name = "kobj_config2",
	.mode = S_IRWXUGO,
};

static struct attribute *def_attrs[]={
	&kobj_config1,
	&kobj_config2,
	NULL,
};


struct sysfs_ops obj_test_sysops=
{
	.show = kobj_test_show,
	.store = kobj_test_store,
};

struct kobj_type ktype = 
{
	.release = obj_test_release,
	.sysfs_ops=&obj_test_sysops,
	.default_attrs = def_attrs,
};

void obj_test_release(struct kobject *kobject)
{
	printk("eric_test:release.\n");
}
ssize_t kobj_test_show(struct kobject *kobject,struct attribute *attr,char *buf)
{
	printk("have show\n");
	printk("attrname:%s\n",attr->name);
	sprintf(buf,"%s\n",attr->name);
	return strlen(attr->name)+2;
}
ssize_t kobj_test_store(struct kobject *kobject,struct attribute *attr,const char *buf,size_t count)
{
	printk("have store\n");
	printk("write:%s\n",buf);
	return count;
}
struct kobject kobj;
static int kobj_test_init()
{
	printk("kobj test init\n");
	printk("kobj->state_initialized=%d\n",kobj.state_initialized);
	printk("kobj->state_in_sysfs=%d\n",kobj.state_add_uevent_sent);
	printk("kobj->state_add_uevent_sent=%d\n",kobj.state_remove_uevent_sent);
	printk("kobj->uevent_suppress=%d\n",kobj.uevent_suppress);
	printk("kobj->name = %s\n",kobj.name);
	kobject_init_and_add(&kobj,&ktype,NULL,"kobject_test");
	printk("\n\n*******************************\n\n");
	printk("kobj->state_initialized=%d\n",kobj.state_initialized);
	printk("kobj->state_in_sysfs=%d\n",kobj.state_add_uevent_sent);
	printk("kobj->state_add_uevent_sent=%d\n",kobj.state_remove_uevent_sent);
	printk("kobj->uevent_suppress=%d\n",kobj.uevent_suppress);
	printk("kobj->name = %s\n",kobj.name);
	return 0;
}
static int kobj_test_exit()
{
	printk("kobj test exit\n");
	kobject_del(&kobj);
	return 0;
}
module_init(kobj_test_init);
module_exit(kobj_test_exit);
