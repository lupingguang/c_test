#include <stdio.h>
#include "my_shared_lib/my_shared_lib.h"
int main(void)
{
	int Params[3] ={100,1,0};
        Common_call_Interface("NewFuncApi", (void *)Params);
   	printf("[app]get param from so:%d\n",Params[2]);
	return 0;
}

