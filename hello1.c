/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "hello1.h"

MODULE_AUTHOR("Dremov A. K.");
MODULE_DESCRIPTION("Programming on Linux kernel");
MODULE_LICENSE("Dual BSD/GPL");

struct my_list {
struct list_head list_node;
ktime_t start;
ktime_t end;
};
static LIST_HEAD(my_list_head);

module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Defines how many times a string will be printed");
static int print_hello(uint count)
{
	ktime_t start, end;
	struct my_list *data;
	uint i;

	BUG_ON(!list_empty(&my_list_head));
	for (i = 0; i < count; i++) {
		start = ktime_get();
		pr_info("Hello, world!\n");
		end = ktime_get();

		data = (struct my_list *) 			                  kmalloc(sizeof(struct my_list), GFP_KERNEL);

		if (i == 5) {
			data = NULL;
		}

		*data = (struct my_list) {
				(struct list_head) {NULL, NULL},
				start,
				end
			};

		list_add_tail(&data->list_node, &my_list_head);
	}
	return 0;
}

EXPORT_SYMBOL(print_hello);

static int __init my_init(void)
{
	pr_info("Loading module...\n");

	WARN_ON(count == 0 || (count >= 5 && count <= 10));
	BUG_ON(count > 10);

	return print_hello(count);
}

static void __exit my_exit(void)
{
	struct my_list *data, *tmp;

	list_for_each_entry_safe(data, tmp, &my_list_head, list_node) {
		pr_info("%lld\n", data->end - data->start);
		list_del(&data->list_node);
		kfree(data);
	}
	pr_info("Unloading module...\n");
}

module_init(my_init);
module_exit(my_exit);
