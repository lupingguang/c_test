#ifndef MYSTATICLIB
#define MYSTATICLIB

typedef int (*FuncApi)(void *); 
struct CommonApiInfo
{
	FuncApi    FuncApiPtr;
	const char *FuncApiStrName;
	int        FuncApiNR;
};

int Common_call_Interface(char *name, void *InOutParams);

#endif
