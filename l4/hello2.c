// SPDX-License-Identifier: GPL-2-Clause
#include <hello1.h>

MODULE_AUTHOR("IO-11 Denysenko Bohdan");
MODULE_DESCRIPTION("Laboratory Work 4");
MODULE_LICENSE("GPL");

static uint amounter = 1;

module_param(amounter, uint, 0);
MODULE_PARM_DESC(amounter, "Counter for my lab 4\n");

static int __init initialize_hello(void)
{
    pr_info("hello2 initiated\n");
    show_greeting(amounter);
    return 0;
}

static void __exit cleanup_hello(void)
{
    pr_info("hello2 unloaded\n");
}

module_init(initialize_hello);
module_exit(cleanup_hello);

