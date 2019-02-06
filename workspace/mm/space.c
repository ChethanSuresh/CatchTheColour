#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int global_var;
int loop =1; 
int main(){
	int sum = 0;
	int *ptr;

	ptr = malloc(100);
	global_var++;

	strcpy(ptr,"Name");
	printf("Address of ptr: %p\n",ptr);
	if (loop == 1)
		while(1);
		
	return 0;
}
