#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/sysfs.h>

static struct kobject *dev;

static ssize_t start_store(struct kobject *kobj, const struct device_attribute *attr, const char *buf, size_t len)
{
    unsigned long fire;
    int r;
    r = strict_strtoul(buf, 10, &fire);
    if (r || (fire != 1))
        return -EINVAL;
    else {
        gpio_set_value(167, 1);
        gpio_set_value(168, 1);
    }
    return len;
}

static ssize_t stop_store(struct kobject *kobj, const struct device_attribute *attr, const char *buf, size_t len)
{
    unsigned long fire;
    int r;
    r = strict_strtoul(buf, 10, &fire);
    if (r || (fire != 1))
        return -EINVAL;
    else {
        gpio_set_value(167, 0);
        gpio_set_value(168, 0);
    }
    return len;
}

static DEVICE_ATTR(start, 0644, NULL, start_store);
static DEVICE_ATTR(stop, 0644, NULL, stop_store);

static struct gpio em86_3g_gpios[] = {
    { 167, GPIOF_OUT_INIT_HIGH, "3g_on" },
    { 168, GPIOF_OUT_INIT_HIGH, "3g_rst" }
};

int init_module(void)
{
    gpio_request_array(em86_3g_gpios, ARRAY_SIZE(em86_3g_gpios));
    dev = kobject_create_and_add("em86-3g", NULL);
    sysfs_create_file(dev, &dev_attr_start);
    sysfs_create_file(dev, &dev_attr_stop);
    return 0;
}

void cleanup_module(void)
{
    sysfs_remove_file(dev, &dev_attr_start);
    sysfs_remove_file(dev, &dev_attr_stop);
    kobject_del(dev);
    gpio_free_array(em86_3g_gpios, ARRAY_SIZE(em86_3g_gpios));
}

MODULE_LICENSE("GPL");
