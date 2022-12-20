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
#include "fnddrv.h"

#define FND_DRIVER_NAME "/dev/perifnd"

int main(void)
{
fndInit();
fnd_hour(16 , 4);

return 0;
}

