#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>


static struct kobject *mptcp_energy_obj;
static int iface_count;

static ssize_t iface_count_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf)
{
    return sprintf(buf, "%d\n", iface_count);
}

static ssize_t iface_count_store(struct kobject *kobj, struct kobj_attribute *attr,char *buf, size_t count)
{
    sscanf(buf, "%du", &iface_count);
    return count;
}

static struct kobj_attribute mptcp_energy_attribute = __ATTR(iface_count,0666,iface_count_show,iface_count_store);

static int __init energy_register(void)
{
    int error = 0;

    mptcp_energy_obj = kobject_create_and_add("mptcp_energy", kernel_kobj);

    if(!mptcp_energy_obj)
        return -ENOMEM;

    error = sysfs_create_file(mptcp_energy_obj, &mptcp_energy_attribute.attr);

	return error;
}

static void energy_unregister(void)
{
    kobject_put(mptcp_energy_obj);
}

module_init(energy_register);
module_exit(energy_unregister);

MODULE_AUTHOR("Alexander Eifler");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ENERGY-AWARE SCHEDULER MPTCP");
MODULE_VERSION("0.91");
