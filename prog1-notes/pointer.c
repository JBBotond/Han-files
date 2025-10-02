#include <stdio.h>

int main() {
  int num = 69;
  int* p;
  p = &num;
  *p = num; 
  printf("%d \n", *p);
}
