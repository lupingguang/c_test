#include <stdio.h>
#include <math.h>

int main()
{
	float b= 1.6; 
	int a; 
	b = b>=0 ? b+0.5:b-0.5; 

	a = (int)b;
	printf("a =%d\n",a);
return 0;
}
