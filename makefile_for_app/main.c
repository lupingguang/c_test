#include <stdio.h>
#include <sub.h>
#include <add.h>
#include <multiply.h>

int main(int argc, char **argv)
{

  int a = 10;
  int b =5;
  printf("multiply(10,5) = %d\n", _multiply(a,b));
  printf("divide(10,5) = %d\n",   _divide(a,b));
  printf("sub(10,5) = %d\n", _sub(a,b));
  printf("add1(10,5) = %d\n", _add1(a,b));
  printf("add2(10,5) = %d\n", _add2(a,b));


}
