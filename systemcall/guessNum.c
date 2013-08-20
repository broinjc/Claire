#include <linux/kernel.h>
asmlinkage long sys_guessNum(int guess)
{
	if(guess == 42)
		printk("42 is my favorite number!\n");
	else
		printk("Guess again!\n");
	return 0;
}
