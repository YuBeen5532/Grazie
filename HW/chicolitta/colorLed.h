#ifndef _colorLED_H_
#define _colorLED_H_

int color_lv_up(void);     // 띵코리타 Level Up => ColorLED 점등 효과
int color_dead(void);      // 띵코리타 산책 실패 => RED ColorLED 
int pwmLedInit(void);
int pwmActiveAll(void);
int pwmInactiveAll(void);
int pwmSetDuty(int dutyCycle, int pwmIndex);
int pwmSetPeriod(int Period, int pwmIndex);
int pwmSetPercent(int percent, int ledColor);
int pwmStartAll(void);


#endif
