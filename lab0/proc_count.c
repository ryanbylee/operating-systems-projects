#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>

static int seq_show(struct seq_file *s, void *v)
{
	int counter = 0;
	struct task_struct *task_list;
	for_each_process(task_list) {
		counter++;
	}
	seq_printf(s, "%d\n", counter);
	return 0;
}
static struct proc_dir_entry* my_new_entry;
static int __init proc_count_init(void)
{
	
	my_new_entry = proc_create_single("count", 0644, NULL, seq_show);
	pr_info("proc_count: init\n");
	return 0;
}

static void __exit proc_count_exit(void)
{
	proc_remove(my_new_entry);
	pr_info("proc_count: exit\n");
}

module_init(proc_count_init);
module_exit(proc_count_exit);

MODULE_AUTHOR("Ryan Lee");
MODULE_DESCRIPTION("One sentence description");
MODULE_LICENSE("GPL");
