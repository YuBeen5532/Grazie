#ifndef _BUZZER_H_
#define _BUZZER_H_


int buzzerInit(void);
int buzzerPlaySong(int scale);
int buzzerStopSong(void);
int buzzerExit(void);
int buzzerMinigame(); // 미니게임에서 사용할 buzzer 효과음 및 colorled 설정 함수
int FindBuzzerSysPath();
void doHelp(void);
void BuzzerEnable(int bEnable);
void SetFrequency(int frequency);
#endif

