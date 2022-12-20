#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "accelMagGyro.h"
#include "buzzer.h"
#include "button.h"


int main(){
findBuzzerSysPath();
buttonInit();
accInit();
for(int i = 0; i<3; i++){
int Value = getAcc();
printf("%d\r\n", Value);
sleep(1);
}
int value = button_rcv();
printf("%d\r\n",value);
buzzerPlaySong(musicScale[4]);
usleep(500000);
buzzerStopSong();
}
