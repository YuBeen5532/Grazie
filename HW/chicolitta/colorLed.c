#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define COLOR_LED_DEV_R_ "/sys/class/pwm/pwmchip0/"
#define COLOR_LED_DEV_G_ "/sys/class/pwm/pwmchip1/"
#define COLOR_LED_DEV_B_ "/sys/class/pwm/pwmchip2/"
#define PWM_EXPORT "export"
#define PWM_UNEXPORT "unexport"
#define PWM_DUTY "pwm0/duty_cycle"
#define PWM_PERIOD "pwm0/period"
#define PWM_ENABLE "pwm0/enable"
#define PWM_COLOR_R 0
#define PWM_COLOR_G 1
#define PWM_COLOR_B 2
#define PWM_PERIOD_NS 1000000 //ns. = 1ms = 1khz
 
#include "colorLed.h"
 
 
int pwmActiveAll(void) // PWM 채널 활성화
{
int fd = 0;
fd = open ( COLOR_LED_DEV_R_ PWM_EXPORT, O_WRONLY);
write(fd,&"0",1);
close(fd);
fd = open ( COLOR_LED_DEV_G_ PWM_EXPORT, O_WRONLY);
write(fd,&"0",1);
close(fd);
fd = open ( COLOR_LED_DEV_B_ PWM_EXPORT, O_WRONLY);
write(fd,&"0",1);
close(fd);
return 1;
}
 
int pwmInactiveAll(void) // PWM 채널 비활성화
{
int fd = 0;
fd = open ( COLOR_LED_DEV_R_ PWM_UNEXPORT, O_WRONLY);
write(fd,&"0",1);
close(fd);
fd = open ( COLOR_LED_DEV_G_ PWM_UNEXPORT, O_WRONLY);
write(fd,&"0",1);
close(fd);
fd = open ( COLOR_LED_DEV_B_ PWM_UNEXPORT, O_WRONLY);
write(fd,&"0",1);
close(fd);
return 1;
}
 
 
int pwmSetDuty(int dutyCycle, int pwmIndex) // PUM Duty Cycle 지정
{
int fd = 0;
switch (pwmIndex)
{
case 2:
fd = open ( COLOR_LED_DEV_R_ PWM_DUTY, O_WRONLY);
break;
case 1:
fd = open ( COLOR_LED_DEV_G_ PWM_DUTY, O_WRONLY);
break;
case 0:
default:
fd = open ( COLOR_LED_DEV_B_ PWM_DUTY, O_WRONLY);
break;
}
dprintf(fd, "%d", dutyCycle);
close(fd);
return 1;
}
 
 
int pwmSetPeriod(int Period, int pwmIndex) // PWM 주기 설정
{
int fd = 0;
switch (pwmIndex)
{
case 2:
fd = open ( COLOR_LED_DEV_R_ PWM_PERIOD, O_WRONLY);
break;
case 1:
fd = open ( COLOR_LED_DEV_G_ PWM_PERIOD, O_WRONLY);
break;
case 0:
default:
fd = open ( COLOR_LED_DEV_B_ PWM_PERIOD, O_WRONLY);
break;
}
//printf ("Set pwm%d, Period:%d\r\n",pwmIndex, Period);
dprintf(fd, "%d", Period);
close(fd);
return 1;
}
 
 
 
int pwmSetPercent(int percent, int ledColor) // PWM %값 지정(수치 넣으면 자동으로 시간 생성)
{
if ((percent <0) || (percent > 100))
{
printf ("Wrong percent: %d\r\n",percent);
return 0;
}
int duty = (100- percent) * PWM_PERIOD_NS / 100;
//LED Sinking.
pwmSetDuty(duty, ledColor);
return 0;
}
 
 
 
int pwmStartAll(void) // PWM 시작
{
int fd = 0;
fd = open ( COLOR_LED_DEV_R_ PWM_ENABLE, O_WRONLY);
write(fd,&"1",1);
close(fd);
fd = open ( COLOR_LED_DEV_G_ PWM_ENABLE, O_WRONLY);
write(fd,&"1",1);
close(fd);
fd = open ( COLOR_LED_DEV_B_ PWM_ENABLE, O_WRONLY);
write(fd,&"1",1);
close(fd);
return 1;
}
 
 
 
 
int pwmLedInit(void)
{ //Initialize
pwmActiveAll();
pwmSetDuty(0, 0); //R <- 0
pwmSetDuty(0, 1); //G <- 0
pwmSetDuty(0, 2); //B <- 0
pwmSetPeriod(PWM_PERIOD_NS, 0); pwmSetPeriod(PWM_PERIOD_NS, 1); pwmSetPeriod(PWM_PERIOD_NS, 2);
pwmStartAll();
return 0;
}
 
int color_lv_up(void) // 띵코리타 level up -> colorlde 점등되는 효과
{
    for(int i=0; i<3; i++)
    {
        pwmSetPercent(0,0);
        pwmSetPercent(0,1);
        pwmSetPercent(100,2);
        usleep(200000);
        //Red 출력
        pwmSetPercent(100,0);
        pwmSetPercent(0,1);
        pwmSetPercent(0,2);
        usleep(200000);
        //blue 출력
        pwmSetPercent(0,0);
        pwmSetPercent(100,1);
        pwmSetPercent(0,2);
        usleep(200000);
        //Green출력
    }
        pwmSetPercent(100,0);
        pwmSetPercent(100,1);
        pwmSetPercent(100,2); //white
}

int color_dead(void) // 띵코리타가 죽었을 때 colorled 출력
{
    pwmSetPercent(0,0);
    pwmSetPercent(0,1);
    pwmSetPercent(100,2); //red
    usleep(800000);
}

