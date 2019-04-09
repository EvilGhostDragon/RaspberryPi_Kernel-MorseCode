#include <linux/module.h>	
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/gpio.h> 
#include <linux/fs.h>      
#include <asm/uaccess.h> 
#include <linux/kthread.h>
#include <linux/delay.h>

#define LED 18

static struct task_struct *task;
char input[6]="aBsMON";

void morse_shortBlink(int loop){
	int i = 0;
	while(loop > i){
		gpio_set_value(LED, 1);
		msleep(150);
		gpio_set_value(LED, 0);
		msleep(150);
		i++;
	}
}

void morse_longBlink(int loop){
	int i = 0;
	while(loop > i){
		gpio_set_value(LED, 1);
		msleep(450);
		gpio_set_value(LED, 0);
		msleep(450);
		i++;
	}
}

void morse_space(void){
	msleep(1050);
}

void morse_decode(char data){
	switch (data){
		case 'a':
		case 'A': 	morse_shortBlink(1);
					morse_longBlink(1);
					break;
		case 'b':
		case 'B': 	morse_longBlink(1);
					morse_shortBlink(3);
					break;
		case 's':
		case 'S': 	morse_shortBlink(3);
					break;
		default: 	morse_space();

	}
	msleep(450);
}

int morse_thread(void *data){
	int powerState = 0;
	int i = 0;
	
  	while(1) {
  		while(i < 6){
	    	printk(KERN_INFO "omg %i\n", powerState);
	    	printk(KERN_INFO "char: %c\n", input[i]);
	    	morse_decode(input[i]);
	   		i++;
   		}
   		i=0;
    	if (kthread_should_stop()) break;
  }
  return 0;
}

static int __init kernel_init(void){
	int ret = 0;

	printk(KERN_INFO "%s\n", __func__);

	// register, turn off 
	ret = gpio_request_one(LED, GPIOF_OUT_INIT_LOW, "led");

	if (ret) {
		printk(KERN_ERR "Unable to request GPIOs: %d\n", ret);
		return ret;
	}
	
	task = kthread_run(morse_thread, NULL, "morse");

	

	return ret;
}

static void __exit kernel_exit(void){
	printk(KERN_INFO "%s\n", __func__);

	gpio_set_value(LED, 0); 
	
	gpio_free(LED);
	kthread_stop(task);
}

MODULE_LICENSE("free4all");
MODULE_AUTHOR("Simon HEIS");
MODULE_DESCRIPTION("Basic kernel module using threads. Morse Code via file to GPIOs (LED on Raspberry).");

module_init(kernel_init);
module_exit(kernel_exit);
