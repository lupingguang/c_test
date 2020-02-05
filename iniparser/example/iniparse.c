#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"
#include <ctype.h>
 
//去除尾部空格
char *rtrim(char *str)
{
	if (str == NULL || *str == '\0')
	{
		return str;
	}
 
	int len = strlen(str);
	char *p = str + len - 1;
	while (p >= str  && isspace(*p))
	{
		*p = '\0';
		--p;
	}
 
	return str;
}
 
//去除首部空格
char *ltrim(char *str)
{
	if (str == NULL || *str == '\0')
	{
		return str;
	}
 
	int len = 0;
	char *p = str;
	while (*p != '\0' && isspace(*p))
	{
		++p;
		++len;
	}
 
	memmove(str, p, strlen(str) - len + 1);
 
	return str;
}
 
//去除首尾空格
char *trim(char *str)
{
	str = rtrim(str);
	str = ltrim(str);
	
	return str;
}

int main(int argc, char * argv[])
{
    dictionary * ini ;
    char       * ini_name ;

    if (argc<2) {
        ini_name = "default.ini";
    } else {
        ini_name = argv[1] ;
    }

    ini = iniparser_load(ini_name);
    //iniparser_dump(ini, stdout);
    int d;
    char *str;
    char *key = "multi:d";
    d = iniparser_getint(ini, key, 100);
    str = (char *)iniparser_getstring(ini, key, "null");
    printf("%s value as int %d\n", key, d);
    printf("%s value as str %s\n", key, str);
   
   
    //以下为将数组按照字符串读入，然后转化为整形数组的过程
    //获得字符串中第一个非空子串  
    char *index = strtok(str, ",");
    while(index != NULL)
    {
         //将字串两侧空格去掉
         index = trim(index);
         //转换为整形
         d = atoi(index); 
         printf("str split to sub str =%s\n", index);
         printf("str convert to integer =%d\n", d);
         //指向下一个字串
         index = strtok(NULL, ","); 
    }
    iniparser_freedict(ini);

    return 0 ;
}
