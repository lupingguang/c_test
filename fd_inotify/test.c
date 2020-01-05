#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    char buf[80];
    int fd;
    unlink( "./fifo/a" );
    mkfifo( "./fifo/a", 0777 );
    
    char s[] = "Hello!\n";
    fd = open( "./fifo/a", O_RDWR );
    write( fd, s, sizeof(s) );
    close( fd );
    
    return 0;
}
