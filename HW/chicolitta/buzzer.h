#ifndef _BUZZER_H_
#define _BUZZER_H_


int buzzerInit(void);
int buzzerPlaySong(int scale);
int buzzerStopSong(void);
int buzzerExit(void);
int buzzerMinigame();
int FindBuzzerSysPath();
void doHelp(void);
void BuzzerEnable(int bEnable);
void SetFrequency(int frequency);
#endif

