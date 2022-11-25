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
pthread_t buttonTh_id1, buttonTh_id2;
struct input_event Button_EVENT;
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
    pthread_create(&buttonTh_id1, NULL, buttonThFunc1, NULL);
    pthread_create(&buttonTh_id2, NULL, buttonThFunc2, NULL); 
    return 1;
}
////////////////////////////////////////////////////////////////////////////
void buttonThFunc1(void) 
{
    int readSize, inputIndex;
    BUTTON_MSG_T Send;
    while(1)
    {        
        readSize = read(fd, &Button_EVENT, sizeof(Button_EVENT));         
        if (readSize != sizeof(Button_EVENT))
            continue;      
        Send.messageNum=1;
        Send.keyInput=Button_EVENT.code;
        Send.pressed=Button_EVENT.value;
        msgsnd(msgID, &Send, sizeof(Send)-4,0);
    }
}

void buttonThFunc2(void) 
{
    int returnValue=0;
    BUTTON_MSG_T Recieve;
    while(1){
        returnValue=msgrcv(msgID, &Recieve, sizeof(Recieve)-4, 0, 0);
        if(returnValue==-1) continue;
	    if ( Button_EVENT.type == EV_KEY)
	    {
		    printf("EV_KEY(");
		    switch(Recieve.keyInput) // 어떤 버튼 눌렀는지
		    {
			    case KEY_VOLUMEUP: printf("Volume up key):"); break;
			    case KEY_HOME: printf("Home key):"); break;
			    case KEY_SEARCH: printf("Search key):"); break;
			    case KEY_BACK: printf("Back key):"); break;
			    case KEY_MENU: printf("Menu key):"); break;
			    case KEY_VOLUMEDOWN: printf("Volume down key):"); break;
               default: break;
		    }
	        if ( Recieve.pressed ) printf("pressed\n"); // 버튼 눌렸는지 안눌렸는지
	        else printf("released\n");
	    } 
	    else;
    }
}
/////////////////////////////////////////////////////////////////////////
int buttonExit(void)
{
    //pthread_exit((void*)0)
    pthread_join(buttonTh_id1, NULL);	
    pthread_join(buttonTh_id2, NULL);	
	close(fd);
}
