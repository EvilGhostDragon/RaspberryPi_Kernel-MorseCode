#include <linux/module.h>	
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/gpio.h> 
#include <linux/kthread.h>
#include <linux/delay.h>

#define LED 18

static struct task_struct *task;
char input[11]="Hello World";



///////////////////////////////////////////
//	GPIO morse settings
//////////////////////////////////////////
void morse_shortBlink(int loop){
	int i = 0;
	while(loop > i){
	    printk(KERN_INFO ".");
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
	    printk(KERN_INFO "-");
		gpio_set_value(LED, 1);
		msleep(450);
		gpio_set_value(LED, 0);
		msleep(450);
		i++;
	}
}

void morse_space(void){
	    printk(KERN_INFO " ");
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
		case 'c':
		case 'C': 	morse_longBlink(1);
					morse_shortBlink(1);
					morse_longBlink(1);
					morse_shortBlink(1);
					break;
		case 'd':
		case 'D': 	morse_longBlink(1);
					morse_shortBlink(2);
					break;
		case 'e':
		case 'E': 	morse_shortBlink(1);
					break;
		case 'f':
		case 'F': 	morse_shortBlink(2);
					morse_longBlink(1);
					morse_shortBlink(1);
					break;
		case 'g':
		case 'G': 	morse_longBlink(2);
					morse_shortBlink(1);
					break;
		case 'h':
		case 'H': 	morse_shortBlink(4);
					break;
		case 'i':
		case 'I': 	morse_shortBlink(2);
					break;
		case 'j':
		case 'J': 	morse_shortBlink(1);
					morse_longBlink(3);
					break;
		case 'k':
		case 'K': 	morse_longBlink(1);
					morse_shortBlink(1);
					morse_longBlink(1);
					break;
		case 'l':
		case 'L': 	morse_shortBlink(1);
					morse_longBlink(1);
					morse_shortBlink(2);
					break;
		case 'm':
		case 'M': 	morse_longBlink(2);
					break;
		case 'n':
		case 'N': 	morse_longBlink(1);
					morse_shortBlink(1);
					break;
		case 'o':
		case 'O': 	morse_longBlink(3);
					break;
		case 'p':
		case 'P': 	morse_shortBlink(1);
					morse_longBlink(2);
					morse_shortBlink(1);
					break;
		case 'q':
		case 'Q': 	morse_longBlink(2);
					morse_shortBlink(1);
					morse_longBlink(1);
					break;
		case 'r':
		case 'R': 	morse_shortBlink(1);
					morse_longBlink(1);
					morse_shortBlink(1);
					break;
		case 's':
		case 'S': 	morse_shortBlink(3);
					break;
		case 't':
		case 'T': 	morse_longBlink(1);
					break;
		case 'u':
		case 'U': 	morse_shortBlink(2);
					morse_longBlink(1);
					break;
		case 'v':
		case 'V': 	morse_shortBlink(3);
					morse_longBlink(1);
					break;
		case 'w':
		case 'W': 	morse_shortBlink(1);
					morse_longBlink(2);
					break;
		case 'x':
		case 'X': 	morse_longBlink(1);
					morse_shortBlink(2);
					morse_longBlink(1);
					break;
		case 'y':
		case 'Y': 	morse_longBlink(1);
					morse_shortBlink(1);
					morse_longBlink(2);
		case 'z':
		case 'Z': 	morse_longBlink(2);
					morse_shortBlink(2);
					break;
		
		default: 	morse_space();

	}
	msleep(450);
}
///////////////////////////////////////////
//Main Thread
///////////////////////////////////////////
int morse_thread(void *data){
	int i = 0;

  	while(1) {
  		while(i < 11){
	    	printk(KERN_INFO "char: %c\n", input[i]);
	    	morse_decode(input[i]);
	   		i++;
   		}
   		i=0;
	    printk(KERN_INFO "\n");
    	if (kthread_should_stop()) break;
  }
  return 0;
}
///////////////////////////////////////////
//INIT & EXIT
///////////////////////////////////////////
static int __init kernel_init(void){
	int ret = 0;

	printk(KERN_INFO "%s\n", __func__);

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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Simon HEIS");
//english skills are not the yello from tha egg
MODULE_DESCRIPTION("Basic kernel module using threads. Morse Code via file to GPIOs (LED on Raspberry).");

module_init(kernel_init);
module_exit(kernel_exit);
