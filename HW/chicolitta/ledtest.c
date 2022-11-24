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
#define LED_DRIVER_NAME "/dev/periled"

int main(void)
{

// open driver
ledLibInit();
ledOnOff (6, 1);
int ledValue = ledStatus();
printf("ledValue : %d\n", ledValue);

sleep(3000); //delay
ledLibExit();
return 0;
}
