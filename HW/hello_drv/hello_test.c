#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define HELLO_DRIVER_NAME "/dev/hello1"

int main(int argc, char **argv)
{
    char array[2000] = {'1', '2', '3', '4', '5', '6', '7'};
    int fd;
    printf("enter test\n");

    // open driver
    fd = open(HELLO_DRIVER_NAME,O_RDWR);
    if (fd < 0)
    {
        printf(" open fail\n");
        perror("driver (/dev/hello1) open error.\n");
        return 1;
    }
    printf("open success\n");

    write(fd, &array, 1);
    read(fd, array, 1);
    
    unsigned int inputCmd = _IOW(0x55, 98, int);
    int count = 7;
    unsigned int long returnValue = ioctl(fd, inputCmd, &count);

    close(fd);
    return 0;
}
