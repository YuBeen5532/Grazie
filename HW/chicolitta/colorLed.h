#ifndef _colorLED_H_
#define _colorLED_H_
int pwmActiveAll(void);
int pwmInactiveAll(void);
int pwmSetDuty(int dutyCycle, int pwmIndex);
int pwmSetPeriod(int Period, int pwmIndex);
int pwmSetPercent(int percent, int ledColor);
int pwmStartAll(void);
int pwmLedInit(void);
int color_lv_up(void); // 띵코리타 level up -> colorlde 점등되는 효과
int color_dead(void); // 띵코리타가 죽었을 때 colorled 출력
#endif
