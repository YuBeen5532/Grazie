#include "led.h"
#include <unistd.h>
#include <fcntl.h>

static unsigned int ledValue = 0;
static int fd_led = 0;
/*================================== LED INIT ==============================*/
int ledInit(void)
{
    fd_led=open("/dev/periled", O_WRONLY);  // file open
    ledValue = 0xFF;                        // 처음에는 체력 최대치 => LED 8개 모두 ON   
    write (fd_led, &ledValue, 4);
}
/*=====================================================================================*/

/*================================== LED EXIT ==============================*/
int ledExit(void)
{
ledValue = 0;
ledOnOff (0, 0);
close(fd_led);  // file close
}
/*=====================================================================================*/

/*============================== 띵코리타 체력 => LED로 표시 ============================*/
void ledLife(int life)
{
	int life_led;
	switch(life)
	{
		case 0: life_led = 0b00000000; write (fd_led, &life_led, 4);  break;
		case 1: life_led = 0b00000001; write (fd_led, &life_led, 4);  break; 
		case 2: life_led = 0b00000011; write (fd_led, &life_led, 4);  break; 
		case 3: life_led = 0b00000111; write (fd_led, &life_led, 4);  break;   
		case 4: life_led = 0b00001111; write (fd_led, &life_led, 4);  break;    
		case 5: life_led = 0b00011111; write (fd_led, &life_led, 4);  break;   
		case 6: life_led = 0b00111111; write (fd_led, &life_led, 4);  break;   
		case 7: life_led = 0b01111111; write (fd_led, &life_led, 4);  break;  
		case 8: life_led = 0b11111111; write (fd_led, &life_led, 4);  break;   
	}
}
/*=====================================================================================*/

/*================================ LED ON/OFF ==============================*/
int ledOnOff (int ledNum, int onOff)
{
	int i=1;
	i = i<<ledNum;
	ledValue = ledValue& (~i);
	if (onOff !=0) ledValue |= i

;
	write (fd_led, &ledValue, 4);
}
/*=====================================================================================*/

int ledStatus (void)
{
return ledValue;
}

