#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <pthread.h>
#include "buzzer.h"
#define MAX_SCALE_STEP 8

char gBuzzerBaseSysDir[128]; ///sys/busz/platform/devices/peribuzzer.XX 가 결정됨
//const int musicScale[MAX_SCALE_STEP] =
//{
//    262, /*do*/ 294,330,349,392,440,494, /* si */ 523
//};



int main(void)
{
    buzzerInit();
    int buzzermsgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
    BUZZER_MSG_T buzzer_snd;
    buzzer_snd.messageNum = 1;
    buzzer_snd.buzzer_msg = 1;
    msgsnd(buzzermsgID, &buzzer_snd, sizeof(buzzer_snd)-4, 0);
    
    buzzerExit();
    return 0;
}
