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

/*======================= 띵코리타 Level Up => ColorLED 점등 효과 ========================*/  
int color_lv_up(void) 
{
    for(int i=0; i<3; i++)
    {
        // Red 출력
        pwmSetPercent(0,0);
        pwmSetPercent(0,1);
        pwmSetPercent(100,2);
        usleep(200000);
        // Blue 출력
        pwmSetPercent(100,0);
        pwmSetPercent(0,1);
        pwmSetPercent(0,2);
        usleep(200000);
        // Green 출력
        pwmSetPercent(0,0);
        pwmSetPercent(100,1);
        pwmSetPercent(0,2);
        usleep(200000);
    }
    // White 출력
    pwmSetPercent(100,0);
    pwmSetPercent(100,1);
    pwmSetPercent(100,2); 
}
/*========================================================================================*/ 

/*========================= 띵코리타 산책 실패 => RED ColorLED ========================*/ 
int color_dead(void) 
{
    // Red 출력
    pwmSetPercent(0,0);
    pwmSetPercent(0,1);
    pwmSetPercent(100,2); 
    usleep(800000);
}
/*========================================================================================*/ 

/*================================ PWMLED INIT ================================*/ 
int pwmLedInit(void)
{ 
    pwmActiveAll();
    // PWM Duty Cycle 지정
    pwmSetDuty(0, 0); // BLUE
    pwmSetDuty(0, 1); // GREEN 
    pwmSetDuty(0, 2); // RED
    // PWM 주기 설정
    pwmSetPeriod(PWM_PERIOD_NS, 0); // BLUE
    pwmSetPeriod(PWM_PERIOD_NS, 1); // GREEN 
    pwmSetPeriod(PWM_PERIOD_NS, 2); // RED
    pwmStartAll();
    return 0;
}
/*========================================================================================*/ 
 
/*============================= PWM 채널 활성화 =====================================*/
int pwmActiveAll(void) 
{
    // /sys/class/pwm/pwmchipX/export file open -> 1 써서 활성화 -> file close
    int fd = 0;
    fd = open ( COLOR_LED_DEV_R_ PWM_EXPORT, O_WRONLY); // RED
    write(fd,&"0",1);
    close(fd);
    fd = open ( COLOR_LED_DEV_G_ PWM_EXPORT, O_WRONLY); // GREEN
    write(fd,&"0",1);
    close(fd);
    fd = open ( COLOR_LED_DEV_B_ PWM_EXPORT, O_WRONLY); // BLUE
    write(fd,&"0",1);
    close(fd); 
    return 1;
}
/*========================================================================================*/

/*============================= PWM 채널 비활성화 =====================================*/ 
int pwmInactiveAll(void) 
{
    // /sys/class/pwm/pwmchipX/unexport file open -> 1 써서 비활성화 ->  file close
    int fd = 0;
    fd = open ( COLOR_LED_DEV_R_ PWM_UNEXPORT, O_WRONLY); // RED
    write(fd,&"0",1);
    close(fd);
    fd = open ( COLOR_LED_DEV_G_ PWM_UNEXPORT, O_WRONLY); // GREEN
    write(fd,&"0",1);
    close(fd);
    fd = open ( COLOR_LED_DEV_B_ PWM_UNEXPORT, O_WRONLY); // BLUE
    write(fd,&"0",1);
    close(fd); 
    return 1;
}
/*========================================================================================*/
 
/*=========================== PWM Duty Cycle 지정 ==================================*/ 
int pwmSetDuty(int dutyCycle, int pwmIndex) // 
{
    int fd = 0;
    // /sys/class/pwm/pwmchipX/pwm0/duty_cycle file open 
    switch (pwmIndex)
    {
        case 2: fd = open ( COLOR_LED_DEV_R_ PWM_DUTY, O_WRONLY); break; // RED
        case 1: fd = open ( COLOR_LED_DEV_G_ PWM_DUTY, O_WRONLY); break; // GREEN
        case 0:
        default:fd = open ( COLOR_LED_DEV_B_ PWM_DUTY, O_WRONLY); break; // BLUE
    }
    // file에 dutyCycle 쓰고 -> file close
    dprintf(fd, "%d", dutyCycle);
    close(fd);
    return 1;
}
/*========================================================================================*/ 

/*============================= PWM 주기 설정 ========================================*/  
int pwmSetPeriod(int Period, int pwmIndex) 
{
    int fd = 0;
    // /sys/class/pwm/pwmchipX/pwm0/period file open
    switch (pwmIndex)
    {
        case 2: fd = open ( COLOR_LED_DEV_R_ PWM_PERIOD, O_WRONLY); break; // RED
        case 1: fd = open ( COLOR_LED_DEV_G_ PWM_PERIOD, O_WRONLY); break; // GREEN
        case 0:
        default: fd = open ( COLOR_LED_DEV_B_ PWM_PERIOD, O_WRONLY); break; // BLUE
    }
    // printf ("Set pwm%d, Period:%d\r\n",pwmIndex, Period);
    // file에 Period 쓰고 -> file close
    dprintf(fd, "%d", Period);
    close(fd);
    return 1;
}
/*========================================================================================*/ 
 
/*============================= PWM %값 지정 =========================================*/  
int pwmSetPercent(int percent, int ledColor) // %값 지정 => 수치 넣으면 자동으로 시간 생성
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
/*========================================================================================*/ 
 
/*================================ PWM 시작 =========================================*/ 
int pwmStartAll(void)
{
    // /sys/class/pwm/pwmchipX/pwm0/enable file open -> 1 써서 PWM 시작 -> file close
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
/*========================================================================================*/ 
