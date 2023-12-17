#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

struct time_point {
    struct list_head node;
    ktime_t start;
    ktime_t end;
};

MODULE_AUTHOR("IO-11 Denysenko Bohdan");
MODULE_DESCRIPTION("Laboratory Work 5");
MODULE_LICENSE("GPL");

static LIST_HEAD(time_record_list);

static uint amounter = 1;
module_param(amounter, uint, 0444);
MODULE_PARM_DESC(amounter, "Number of repetitions");

static int __init hello_start(void)
{
    uint index = 0;
    struct time_point *point;

    pr_info("Number of repetitions: %d\n", amounter);

    BUG_ON(amounter == 0);

    for (index = 0; index < amounter; index++) {
        point = kmalloc(sizeof(struct time_point), GFP_KERNEL);

        if (index == 5)
            point = NULL;

        if (ZERO_OR_NULL_PTR(point))
            goto error_handler;

        point->start = ktime_get();
        pr_info("Hello, world!\n");
        point->end = ktime_get();

        list_add_tail(&point->node, &time_record_list);
    }

    return 0;

error_handler:
    {
        struct time_point *temp_point, *safe_point;

        pr_err("Out of memory...\n");

        list_for_each_entry_safe(temp_point, safe_point, &time_record_list, node) {
            list_del(&temp_point->node);
            kfree(temp_point);
        }

        BUG();
        return -ENOMEM;
    }
}

static void __exit hello_stop(void)
{
    struct time_point *temp_point, *safe_point;

    list_for_each_entry_safe(temp_point, safe_point, &time_record_list, node) {
        pr_info("Execution time: %lld", temp_point->end - temp_point->start);

        list_del(&temp_point->node);
        kfree(temp_point);
    }

    BUG_ON(!list_empty(&time_record_list));
}

module_init(hello_start);
module_exit(hello_stop);

