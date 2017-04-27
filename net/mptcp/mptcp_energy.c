#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

/*
 * History:
 * - https://pradheepshrinivasan.github.io/2015/07/02/Creating-an-simple-sysfs/
 *   Not bad, but only one file
 * - https://github.com/spotify/linux/blob/master/samples/kobject/kobject-example.c
 *   With sysfs-groups but problems with access-rights
 * - https://github.com/BotCyborg/adk-driver-skeleton/commit/b19a0e8aa21ccc7e4c0cd5c0e1882f49c0749cee
 *   Give's idea to deal with new access-rights-restrictions from commit:
 *   https://github.com/torvalds/linux/commit/58f86cc89c3372d3e61d5b71e5513ec5a0b02848
 */

static u32 iface_main;
static u32 iface_backup;

static u32 mptcp_energy_iface_main_func(void)
{
    return iface_main;
}
EXPORT_SYMBOL_GPL(mptcp_energy_iface_main_func);

static u32 mptcp_energy_iface_backup_func(void)
{
    return iface_backup;
}
EXPORT_SYMBOL_GPL(mptcp_energy_iface_backup_func);

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