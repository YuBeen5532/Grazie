#include "buzzer.h"
#include "colorLed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"

#define MAX_SCALE_STEP 8 

int fd_buzzer;
const int Musicscale[8] = // 음계
{ 262, /*do*/ 294,330,349,392,440,494, /* si */ 523 };
char gBuzzerBaseSysDir[128]; ///sys/bus/platform/devices/peribuzzer.XX 가 결정됨

/*============================== Buzzer 경로 찾기 ==================================*/
// sys/bus/platform/devices/peribuzzer.XX 의 XX를 결정하는 것
int FindBuzzerSysPath()
{
    DIR * dir_info = opendir(BUZZER_BASE_SYS_PATH);
    int ifNotFound = 1;
        if (dir_info != NULL){
            while (1){
                struct dirent *dir_entry;
                dir_entry = readdir (dir_info);
                if (dir_entry == NULL) break;
                if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0){
                       // 경로 찾음 -> gBuzzerBaseSysDir에 경로 넣어줌 
                    ifNotFound = 0;
                    sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH,dir_entry->d_name); 
                  }
              }
          }
    printf("find %s\n",gBuzzerBaseSysDir);
    return ifNotFound;
}
/*========================================================================================*/

/*============================== Buzzer INIT ==================================*/
int buzzerInit(void) 
{
	FindBuzzerSysPath();
	fd_buzzer = open(gBuzzerBaseSysDir, O_WRONLY); // 경로 설정
}
/*========================================================================================*/

/*============================== Buzzer EXIT ==================================*/
int buzzerExit(void)
{
	close(fd_buzzer);
}
/*========================================================================================*/

/*============================== Buzzer ENABLE ================================*/
// BuzzerEnable함수: 매개변수 bEnable값에 따라 enable 0 or 1
void BuzzerEnable(int bEnable) 
{
		char path[200];
		sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
		int fd_buzzer=open(path,O_WRONLY);       // enable file open
		if ( bEnable) write(fd_buzzer, &"1", 1); // enable file에 1 써준 후
		else 			write(fd_buzzer, &"0", 1); // enable file에 0 써준 후
		close(fd_buzzer);                        // file close
}
/*========================================================================================*/

/*============================== Buzzer Frequency =============================*/
void SetFrequency(int frequency) // 음 설정
{
		char path[200];
		sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
		int fd_buzzer=open(path,O_WRONLY);      // frequency file open
		dprintf(fd_buzzer, "%d", frequency);    // frequency값 써준 후
		close(fd_buzzer);                       // file close
}
/*========================================================================================*/

/*============================== Buzzer Stop ==================================*/
int buzzerStopSong(void) 
{	BuzzerEnable(0); }
/*========================================================================================*/

/*========================= 미니게임 buzzer 효과음 및 colorled 설정 ========================*/
int buzzerMinigame() 
{
    for(int i=1; i<8; i+=2){
        buzzerPlaySong(Musicscale[i]);
        pwmSetPercent(0,0);
        pwmSetPercent(0,1);
        pwmSetPercent(100,2);
        usleep(200000);
        // colorled Red 출력할 것
        pwmSetPercent(100,0);
        pwmSetPercent(0,1);
        pwmSetPercent(0,2);
        usleep(200000);
        // colorled Blue 출력할 것
        pwmSetPercent(0,0);
        pwmSetPercent(100,1);
        pwmSetPercent(0,2);
        usleep(200000);
        //  colorled Green 출력할 것
        buzzerStopSong();
    }
    pwmSetPercent(100,0);
    pwmSetPercent(100,1);
    pwmSetPercent(100,2);       
}
/*========================================================================================*/

/*======================== Frequency 지정하여 한 음 재생 ==============================*/
int buzzerPlaySong(int scale) 
{
		SetFrequency(scale);
		BuzzerEnable(1);
}
/*========================================================================================*/

void doHelp()
{
		printf("Usage:\n");
		printf("buzzertest <buzzerNo> \n");
		printf("buzzerNo: \n");
		printf("do(1),re(2),mi(3),fa(4),sol(5),ra(6),si(7),do(8) \n");
		printf("off(0)\n");
}
