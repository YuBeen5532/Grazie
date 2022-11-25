#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include <pthread.h>
#include "button.h"
// first read input device
#define INPUT_DEVICE_LIST "/dev/input/event" //실제 디바이스 드라이버 노드파일: 뒤에 숫자가 붙음., ex)/dev/input/event5
#define PROBE_FILE "/proc/bus/input/devices" //PPT에 제시된 "이 파일을 까보면 event? 의 숫자를 알수 있다"는 바로 그 파일
#define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
#define HAVE_TO_FIND_2 "H: Handlers=kbd event"

//////////////////////////////////////////////////////////////
int fd;
int msgID;
pthread_t buttonTh_id;
/////////////////////////////////////////////////////////////////////////////////////////////

int probeButtonPath(char *newPath)	{
	int returnValue = 0; //button에 해당하는 event#을 찾았나?
	int number = 0; //찾았다면 여기에 집어넣자
	FILE *fp = fopen(PROBE_FILE,"rt"); //파일을 열고

	#define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"   
	#define HAVE_TO_FIND_2 "H: Handlers=kbd event"      

	while(!feof(fp)) //끝까지 읽어들인다.
	{
	    char tmpStr[200]; //200자를 읽을 수 있게 버퍼
	    fgets(tmpStr,200,fp); //최대 200자를 읽어봄
	    //printf ("%s",tmpStr);
	    if (strcmp(tmpStr,HAVE_TO_FIND_1) == 0)
	    {
	        printf("YES! I found!: %s\r\n", tmpStr); //이것도 딱히 필요없을 듯??????
	        returnValue = 1; //찾음
	    }
	    if (
	        (returnValue == 1) && //찾은 상태에서
	        (strncasecmp(tmpStr, HAVE_TO_FIND_2, strlen(HAVE_TO_FIND_2)) == 0)) //Event??을 찾았으면
	    {
	        printf ("-->%s",tmpStr);                     //print들 확인용으로 두고 나중에 없애는게??????
	        printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
	        number = tmpStr[strlen(tmpStr)-3] - '0';
	        //Ascii character '0'-'9' (0x30-0x39)
	        //to interger(0)
	        break;
	    }
    }
	fclose(fp);
	if (returnValue == 1)
	    sprintf (newPath,"%s%d",INPUT_DEVICE_LIST,number);
	return returnValue;
}
/////////////////////////////////////////////////////////////////////////
int buttonInit(void)
{
    char buttonPath[200] = {0,};
    if (probeButtonPath(buttonPath) == 0)
    {
		printf ("ERROR! File Not Found!\r\n");      //나중에 printf는 지워도 될듯
		printf ("Did you insmod?\r\n");
		return 0;
    }
    printf ("buttonPath: %s\r\n", buttonPath);
    fd=open (buttonPath, O_RDONLY);
    msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
    if(msgID==-1){
		printf ("Cannot get msgQueueID, Return!\r\n");
		return -1;
	}
    pthread_create(&buttonTh_id, NULL, buttonThFunc, NULL);
    // pthread_create(&buttonTh_id2, NULL, buttonThFunc, NULL); //추가할 필요가 없는 것같기도
    return 1;
}
////////////////////////////////////////////////////////////////////////////
void buttonThFunc(void)
{
    int readSize;
    BUTTON_MSG_T B;
    struct input_event A;
    while(1)
    {        
        readSize = read(fd, &A, sizeof(A));
        if (readSize != sizeof(stEvent))
            continue;      
        B.messageNum=1;
        B.keyInput=A.code;
        B.pressed=A.value;
        msgsnd(msgID, &B, sizeof(B)-4,0);
    }
}
/////////////////////////////////////////////////////////////////////////
int buttonExit(void)
{
    //pthread_exit((void*)0);
	pthread_join(buttonTh_id, NULL);	
	close(fd);
}
