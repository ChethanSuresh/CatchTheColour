#include <stdio.h>
#include "arth.h"
int main(){
  int c; 
  c=sum(1,2);
  printf("Sum 1+2: %d\n",c);
  c=mul(2,2);
  printf("Multiply 2*2: %d\n", c);
  return 0;
}
