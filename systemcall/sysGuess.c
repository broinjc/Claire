#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main()
{
	int guess;
	printf("Guess the kernels favorite number: ");
	scanf("%d", &guess);
	
	long int rtn = syscall(351, guess);
	printf("System call sys_guessNum return %ld\nCheck dmesg\n", rtn);
	
	// Hint: The ultimate answer to life the universe and everything.
	
	return 0;
}



