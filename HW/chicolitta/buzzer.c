#include "buzzer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/msg.h>
#include <pthread.h>

#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"
#define MAX_SCALE_STEP 8


static int fd;
char gBuzzerBaseSysDir[128]; ///sys/bus/platform/devices/peribuzzer.XX 가 결정됨

const int musicScale[MAX_SCALE_STEP] =
{
    262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};

pthread_t buzzerTh_id;
int buzzermsgID;
 
int buzzerInit(void)
{
	findBuzzerSysPath();

    buzzermsgID = msgget (MESSAGE_ID, IPC_CREAT|0666);	
    if(buzzermsgID == -1){
		printf ("buzzer Cannot get msgQueueID, Return!\r\n");
		return -1;
	}
    pthread_create(&buzzerTh_id, NULL, &buzzerThFunc, NULL);
}

void* buzzerThFunc(void *arg) 
{
    int returnValue=0;
    BUZZER_MSG_T Receive;
    while(1)
    {
        returnValue = msgrcv(buzzermsgID, &Receive, sizeof(Receive)-4, 0, 0);
        if(returnValue == -1) continue;
	    
        if ( Receive.messageNum == 1)
	    {   
            switch(Receive.buzzer_msg) // 어떤 버튼 눌렀는지
		    {
			    case 1: {printf("MainIntro Sound)\r\n"); buzzerMainIntro();} break; // 화면상변화, 아이템창에서 |아이템창나가기|옷|특식/간식|   |     | =>left, right 사용
               default: break;
		    }
	    } 
	    else;
    }
}

void buzzerMainIntro(void)
{
    buzzerPlaySong(musicScale[0]);
    usleep(500000);
    buzzerPlaySong(musicScale[2]);
    usleep(500000);
    buzzerPlaySong(musicScale[4]);
    usleep(500000);
    buzzerStopSong();
}

int buzzerPlaySong(int scale)
{	
    setFrequency(scale);
	buzzerEnable(1);
}

int buzzerStopSong(void)
{
	buzzerEnable(0);
}

int buzzerExit(void)
{
	pthread_join(buzzerTh_id, NULL);
    close(fd);
}

int findBuzzerSysPath()
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


void buzzerEnable(int bEnable)
{
    char path[200];
	sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
	fd=open(path,O_WRONLY);
		if ( bEnable) 		write(fd, &"1", 1);
		else 				write(fd, &"0", 1);
    close(fd);
}

void setFrequency(int frequency)
{    
    char path[200];
	sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
	fd=open(path,O_WRONLY);
    dprintf(fd, "%d", frequency);
    close(fd);
}

void doHelp()
{
		printf("Usage:\n");
		printf("buzzertest <buzzerNo> \n");
		printf("buzzerNo: \n");
		printf("do(1),re(2),mi(3),fa(4),sol(5),ra(6),si(7),do(8) \n");
		printf("off(0)\n");
}
