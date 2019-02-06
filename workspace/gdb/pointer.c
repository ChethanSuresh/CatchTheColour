#include <stdio.h>
int func(int *p){
	func1(p);
	return 0;
}
int func1(int *p){
	func2(p);
	return 0;
}
int func2(int *p){
	func3(p);
	return 0;
}
int func3(int *p){
	func4(p);
	return 0;
}
int func4(int *p){
	printf("p=%d\n",*p);
	return 0;
}
int main(){
	int *p;
	int a=10;
	int b=20;
	int c=30;
	printf("Assigned values\n");
	func(p=5);
	return 0;
}
