#include <linux/kernel.h>

asmlinkage long sys_project(char *buff ,int id)
{
	printk("\n[SLS ICECREAM PARLOUR]: Customer %d: %s;",id,buff);
	return 0;
}
