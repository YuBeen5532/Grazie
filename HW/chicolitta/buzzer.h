#ifndef _BUZZER_H_
#define _BUZZER_H_

int FindBuzzerSysPath();          // Buzzer 경로 찾기
int buzzerInit(void);
int buzzerExit(void);
void BuzzerEnable(int bEnable);   // Buzzer ENABLE
void SetFrequency(int frequency); // Buzzer Frequency
int buzzerStopSong(void);         // Buzzer Stop
int buzzerMinigame();             // 미니게임 buzzer 효과음 및 colorled 설정
int buzzerPlaySong(int scale);    // Frequency 지정하여 한 음 재생
void doHelp(void);

#endif

