#include<stdio.h>

#define _C(a) #a    //将书面符号字符串化
#define C(a) _C(a)  //带#的宏不会展开参数，因此需要多包装一层
#define B(a,b) a##b //将宏参数(书面符号)贴合到一起 

int main()
{
int va =3;
printf("1##2: %d\n", B(1,2));
printf("v##a: %d\n", B(v,a));

printf("#'*': %s\n", _C('*'));
printf("#1: %s\n", _C(1));
printf("#m: %s\n", _C(m));


printf("string0: %s\n", C(B(1,2)));
printf("string1: %s\n", _C(B(1,2)));

return 1;
}
