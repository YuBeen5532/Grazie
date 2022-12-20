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

const int Musicscale[8] =
{
    262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};
 

//buzzer-미니게임성공, 레벨업, 체력 적을때 넣을수도
//교수님이 보내주신 음악 메인BGM쓰고 음향꺼놓는 것도 시현할 때 보여주기 => 봐서. 메인BGM만 끄도록?
int fd_buzzer;
char gBuzzerBaseSysDir[128]; ///sys/bus/platform/devices/peribuzzer.XX 가 결정됨

int buzzerInit(void)
{
	FindBuzzerSysPath();
	fd_buzzer = open(gBuzzerBaseSysDir, O_WRONLY);
}

int buzzerPlaySong(int scale)
{
		SetFrequency(scale);
		BuzzerEnable(1);
}

int buzzerMinigame()
{
    for(int i=1; i<8; i+=2){
       buzzerPlaySong(Musicscale[i]);
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
       buzzerStopSong();
    }
    pwmSetPercent(100,0);
    pwmSetPercent(100,1);
    pwmSetPercent(100,2);
       
}

int buzzerStopSong(void)
{
	BuzzerEnable(0);
}

int buzzerExit(void)
{
	close(fd_buzzer);
}

void SetFrequency(int frequency)
{
		char path[200];
		sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
		int fd_buzzer=open(path,O_WRONLY);
		dprintf(fd_buzzer, "%d", frequency);
		close(fd_buzzer);
}

void BuzzerEnable(int bEnable)
{
		char path[200];
		sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
		int fd_buzzer=open(path,O_WRONLY);
		if ( bEnable) 		write(fd_buzzer, &"1", 1);
		else 				write(fd_buzzer, &"0", 1);
		close(fd_buzzer);
}


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
												ifNotFound = 0;
												sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH,dir_entry->d_name);
									}
							}
				}
				printf("find %s\n",gBuzzerBaseSysDir);
				return ifNotFound;
}
//버저 경로 찾기: /sys/bus/platform/devices/peribuzzer.XX 의 XX를 결정하는 것










void doHelp()
{
		printf("Usage:\n");
		printf("buzzertest <buzzerNo> \n");
		printf("buzzerNo: \n");
		printf("do(1),re(2),mi(3),fa(4),sol(5),ra(6),si(7),do(8) \n");
		printf("off(0)\n");
}
