#ifndef _LED_H_
#define _LED_H_

int ledInit(void);
int ledExit(void); 
void ledLife(int life); // 띵코리타 체력 => LED로 표시

int ledOnOff (int ledNum, int onOff);
int ledStatus (void);


#define LED_DRIVER_NAME "/dev/periled"
#endif

