#ifndef _BUZZER_H_
#define _BUZZER_H_

#define MESSAGE_ID 200

typedef struct
{
long int messageNum;
int buzzer_msg;
} BUZZER_MSG_T;

int buzzerInit(void);
void* buzzerThFunc(void *arg);
void buzzerMainIntro(void);
int buzzerPlaySong(int scale);
int buzzerStopSong(void);
int buzzerExit(void);

int findBuzzerSysPath();
void doHelp(void);
void buzzerEnable(int bEnable);
void setFrequency(int frequency);
#endif

