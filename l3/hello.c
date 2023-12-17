#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("IO-11 Denysenko Bohdan");
MODULE_DESCRIPTION("Laboratorna robota 3");
MODULE_LICENSE("GPL");

static uint amounter = 1;

module_param(amounter, uint, 0444);
MODULE_PARM_DESC(amounter, "Number of Hello");

struct timing_data {
    struct list_head list_node;
    ktime_t timestamp;
};

static LIST_HEAD(timing_list);

static int __init start_module(void)
{
    int idx;
    
    if (amounter == 0) {
        printk(KERN_WARNING "The amounter parameter is 0\n");
       
    } else if (amounter > 10) {
        printk(KERN_WARNING "The amounter parameter exceeds 10\n");
        return -EINVAL;
    } else if (amounter > 5) {
        printk(KERN_WARNING "The amounter parameter between 5 & 10\n");
        return -EINVAL;
    }
    
    for (idx = 0; idx < amounter; idx++) {
        struct timing_data *data = kmalloc(sizeof(struct timing_data), GFP_KERNEL);
        data->timestamp = ktime_get();
        list_add_tail(&data->list_node, &timing_list);
        printk(KERN_INFO "Hello, world! %d\n", idx);
    }

    return 0;
}

static void __exit end_module(void)
{
    struct timing_data *entry, *temp_entry;
    int sequence = 0;

    list_for_each_entry_safe(entry, temp_entry, &timing_list, list_node) {
        pr_info("Index: %d; Timestamp = %lld\n", sequence++, ktime_to_ns(entry->timestamp));
        list_del(&entry->list_node);
        kfree(entry);
    }
    printk(KERN_INFO "Module successfully unloaded\n");
}

module_init(start_module);
module_exit(end_module);

