#include<stdio.h>
enum color
{
	red,
	green,
	black,
	yellow
};

#define ENUMNAME(n) case n: return #n
const char* print_enum_name(int n)
{
	switch(n) {
		ENUMNAME(red);
		ENUMNAME(green);
		ENUMNAME(black);
		default: return "unknow enum";
	}	
}

int main(void)
{
	printf("%s\n",print_enum_name(red));
        printf("%s\n",print_enum_name(green));
        printf("%s\n",print_enum_name(black));
        printf("%s\n",print_enum_name(yellow));

}
