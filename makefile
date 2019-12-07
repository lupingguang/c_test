all:
#	/usr/local/arm/arm-2009q3/bin/arm-linux-gcc main.c  -o main_static.out -lmy_shared_lib -L./my_shared_lib -static
	gcc main.c  -o main.out -lmy_shared_lib -L./my_shared_lib
