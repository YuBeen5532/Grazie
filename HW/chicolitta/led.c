#include "led.h"
#include <unistd.h>
#include <fcntl.h>

static unsigned int ledValue = 0;
static int fd = 0;

int ledOnOff (int ledNum, int onOff)
{
int i=1;
i = i<<ledNum;
ledValue = ledValue& (~i);
if (onOff !=0) ledValue |= i;
write (fd, &ledValue, 4);
}

int ledLibInit(void)
{
    fd=open("/dev/periled", O_WRONLY);
	ledValue = 0xFF;           //풀피
    write (fd, &ledValue, 4);
}

// =>체력깎이거나 오를 때 쉬프트써서 

int ledLibExit(void)
{
ledValue = 0;
ledOnOff (0, 0);
close(fd);
}

int ledStatus (void)
{
return ledValue;
}

