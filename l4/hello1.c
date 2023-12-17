// SPDX-License-Identifier: GPL-2-Clause
#include <hello1.h>

MODULE_AUTHOR("IO-11 Denysenko Bohdan");
MODULE_DESCRIPTION("Laboratory Work 3");
MODULE_LICENSE("GPL");

struct time_data {
    struct list_head list_node;
    ktime_t time_start;
    ktime_t time_end;
};

static struct list_head head_node = LIST_HEAD_INIT(head_node);

int show_greeting(uint amounter)
{
    struct time_data *item, *temp_item;
    uint i;

    if (amounter <= 0) {
        pr_err("Error: The value of amounter is not positive\n");
    } else if (amounter > 0 && amounter < 5) {
        pr_info("Note: The value of amounter is less than 5\n");
    } else if (amounter >= 5 && amounter <= 10) {
        pr_warn("Warning: The value of amounter is between 5 and 10\n");
    } else { 
        pr_err("Error: The value of amounter exceeds 10\n");
        return -EINVAL;
    }

    for (i = 0; i < amounter; i++) {
        item = kmalloc(sizeof(struct time_data), GFP_KERNEL);
        if (ZERO_OR_NULL_PTR(item))
            goto cleanup;
        item->time_start = ktime_get();
        pr_info("Hello, world! \n");
        item->time_end = ktime_get();
        list_add_tail(&item->list_node, &head_node);
    }
    return 0;

cleanup:
    list_for_each_entry_safe(item, temp_item, &head_node, list_node) {
        list_del(&item->list_node);
       	kfree(item);
    }
    pr_err("Memory shortage\n");
    return -ENOMEM;
}
EXPORT_SYMBOL(show_greeting);

static int __init hello_start(void)
{
    pr_info("hello1 initiated\n");
    return 0;
}

static void __exit hello_end(void)
{
    struct time_data *item, *temp_item;

    list_for_each_entry_safe(item, temp_item, &head_node, list_node) {
        pr_info("Time: %lld", item->time_end - item->time_start);
        list_del(&item->list_node);
        kfree(item);
    }
    BUG_ON(!list_empty(&head_node));
    pr_info("Success! hello1 unloaded\n");
}

module_init(hello_start);
module_exit(hello_end);

