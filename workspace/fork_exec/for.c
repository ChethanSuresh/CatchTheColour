#include <stdio.h>
#include <unistd.h>
int count = 0;
int main(){
	for(;;){
		count++;
		sleep(0.1);
		printf("count: %d\n", count);
	}
}
