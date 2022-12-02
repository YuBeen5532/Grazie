#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "fnd.h"

#define FND_DRIVER_NAME "/dev/perifnd"

int main(void)
{
fndDisp(990819,10);

return 0;
}

