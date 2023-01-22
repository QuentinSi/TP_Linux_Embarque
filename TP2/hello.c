#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>


#define INTERVALLE 100
#define DRIVER_AUTHOR "Quentin SIMON"
#define DRIVER_DESC "Hello world Module"
#define DRIVER_LICENSE "GPL"

static int param;

module_param(param,int,0);
MODULE_PARM_DESC(param,"Un parametre entier de ce module");

int hello_init(void)
{
	printk(KERN_INFO "Hello world!\n");
	return 0;
}

void hello_exit(void)
{
	printk(KERN_ALERT "Bye bye...\n");
}

static struct timer_list timer;
static void montimer(struct timer_list *t) {
	mod_timer(&timer,jiffies + INTERVALLE);
}
static int __init timer_init(void) {
	setup_timer(&timer, montimer, 0);
	mod_timer(&timer, jiffies + INTERVALLE);
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
