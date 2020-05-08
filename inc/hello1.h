#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h>

static uint count = 1;
static int print_hello(uint count);
