#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>


static int iface_main;
static int iface_backup;

static ssize_t iface_main_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", iface_main);
}

static ssize_t iface_main_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%du", &iface_main);
    return count;
}

static ssize_t iface_backup_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", iface_backup);
}

static ssize_t iface_backup_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%du", &iface_backup);
    return count;
}

static struct kobj_attribute iface_main_attribute =
        __ATTR(iface_main, 0660, iface_main_show, iface_main_store);
static struct kobj_attribute iface_backup_attribute =
        __ATTR(iface_backup, 0660, iface_backup_show, iface_backup_store);

static struct attribute *attrs[] = {
        &iface_main_attribute.attr,
        &iface_backup_attribute.attr,
        NULL,
};

static struct attribute_group attr_group = {
        .attrs = attrs,
};

static struct kobject *mptcp_energy_kobj;

static int __init energy_register(void)
{
    int error = 0;

    mptcp_energy_kobj = kobject_create_and_add("mptcp_energy", kernel_kobj);

    if(!mptcp_energy_kobj)
        return -ENOMEM;

    error = sysfs_create_group(mptcp_energy_kobj, &attr_group);
    if(error)
    {
        kobject_put(mptcp_energy_kobj);
    }

	return error;
}

static void energy_unregister(void)
{
    kobject_put(mptcp_energy_kobj);
}

module_init(energy_register);
module_exit(energy_unregister);

MODULE_AUTHOR("Alexander Eifler");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ENERGY-AWARE SCHEDULER MPTCP");
MODULE_VERSION("0.91");