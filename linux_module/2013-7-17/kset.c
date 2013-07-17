#include<linux/device.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/string.h>
#include<linux/sysfs.h>
#include<linux/stat.h>
#include<linux/kobject.h>
MODULE_AUTHOR("young.yang");
MODULE_LICENSE("Dual BSD/GPL");
struct kset kset_p;
struct kset kset_c;
struct kobject kobj1;
struct kobject kobj2;
struct kobject kobj3;
struct kobject kobj4;


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

int kset_filter(struct kset *kset,struct kobject *kobj)
{
	printk("Filter:kobj %s.\n",kobj->name);
	return 1;
}
const char *kset_name(struct kset *kset,struct kobject *kobj)
{
	static char buf[20];
	printk("Name:kobj %s.\n",kobj->name);
	sprintf(buf,"%s","kset_name");
	printk("buf=%s\n",buf);
	return buf;
}
int kset_uevent(struct kset *kset,struct kobject *kobj,struct kobj_uevent_env *env)
{
	int i = 0;
	printk("uevent:kobj %s.\n",kobj->name);
	while(i<env->envp_idx){
		printk("%s.\n",env->envp[i]);
		i++;
	}
	return 0;
}
struct kset_uevent_ops uevent_ops=
{
	.filter = kset_filter,
	.name = kset_name,
	.uevent = kset_uevent,
};
static int get_refcount(struct kobject *kobj)
{
	printk("kobj->name=%s  ",kobj->name);
	printk("kobj->refcount=%d\n",kobj->kref.refcount);
	return 0;
}
int kset_test_init()
{
	printk("kset test init.\n");
	kobject_set_name(&kset_p.kobj,"kset_p");
	kset_p.uevent_ops = &uevent_ops;
	kset_register(&kset_p);
	printk("kobj->state_initialized=%d\n",kset_p.kobj.state_initialized);
	printk("kobj->state_in_sysfs=%d\n",kset_p.kobj.state_in_sysfs);
	printk("kobj->state_add_uevent_sent=%d\n",kset_p.kobj.state_add_uevent_sent);
	printk("kobj->state_remove_uevent_sent=%d\n",kset_p.kobj.state_remove_uevent_sent);
	printk("kobj->uevent_suppress=%d\n",kset_p.kobj.uevent_suppress);

	
	kobject_set_name(&kset_c.kobj,"kset_c");
	kset_c.kobj.kset = &kset_p;

	kset_register(&kset_c);
	//&(&kset_c->kobj)

	//kset will +4
	kobj1.kset = &kset_p;
	kobj2.kset = &kset_p;
	kobj3.kset = &kset_p;
	kobj4.kset = &kset_p;

	
	kobject_init_and_add(&kobj1,&ktype,&(kset_p.kobj),"kobject_1");
	printk("kobj->ueven1\n");
	kobject_init_and_add(&kobj2,&ktype,&kobj1,"kobject_2");
	printk("kobj->ueven2\n");
	kobject_init_and_add(&kobj3,&ktype,&kobj2,"kobject_3");
	printk("kobj->ueven3\n");
	kobject_init_and_add(&kobj4,&ktype,&kobj3,"kobject_4");
	printk("kobj->ueven4\n");
	get_refcount(&kset_p.kobj);// 3  //+4
	get_refcount(&kset_c.kobj);// 2
	get_refcount(&kobj1);// 2
	get_refcount(&kobj2);// 2
	get_refcount(&kobj3);// 2
	get_refcount(&kobj4);// 1
	return 0;
}
int kset_test_exit()
{
	printk("kset test exit.\n");
	kset_unregister(&kset_p);
	kset_unregister(&kset_c);
	return 0;
}
module_init(kset_test_init);
module_exit(kset_test_exit);
