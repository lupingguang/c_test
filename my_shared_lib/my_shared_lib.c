#include <stdio.h>
#include "my_shared_lib.h"
#include <string.h>
static int NewFuncApi(void *InOutParams)
{
	printf("[so]use first params as char %c\n", ((char *)InOutParams)[0] );
	printf("[so]use first params as int %d\n", ((int *)InOutParams)[1] );
	
	((int *)InOutParams)[2] = 55;
	return 0;
}

#define MAX_COMMON_API_NR 100

struct CommonApiInfo  CommonApiArray[MAX_COMMON_API_NR] =
{
	{NewFuncApi, "NewFuncApi", 1}
	//add new api...
};

int Common_call_Interface(char *name, void *InOutParams)
{
	unsigned int u32EntryIndex = 0;
	int	     ret = 0;
	for(u32EntryIndex  = 0; u32EntryIndex <MAX_COMMON_API_NR; u32EntryIndex ++)
	{
		//
		if(CommonApiArray[u32EntryIndex].FuncApiNR == 0)
		{
			printf("Func: %s, isnt found in CommonApiArray,force return \n", name);
			return -1;
		}
		if(!strcmp(name, CommonApiArray[u32EntryIndex].FuncApiStrName))
		{
			if(NULL != CommonApiArray[u32EntryIndex].FuncApiPtr && NULL != InOutParams)
			{
				ret = CommonApiArray[u32EntryIndex].FuncApiPtr(InOutParams);
				printf("Func: %s, found in CommonApiArray,FuncApiNR=%d\n", name, u32EntryIndex + 1);
				return ret;
			}
			else
			{
				return -1;
			}
		}
	}
}
