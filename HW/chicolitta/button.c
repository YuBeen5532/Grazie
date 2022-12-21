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

int fd_button;
int msgID;
pthread_t buttonTh_id1, buttonTh_id2;
struct input_event Button_EVENT;

/*==============================  event# 찾았는지 확인 ==============================*/
int probeButtonPath(char *newPath)   {
   int returnValue = 0;   // button에 해당하는 event#을 찾았나
   int number = 0;        // 찾았다면 여기에 집어넣자
   FILE *fp_button = fopen(PROBE_FILE,"rt");  // 파일 열고

   #define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"   
   #define HAVE_TO_FIND_2 "H: Handlers=kbd event"      

   while(!feof(fp_button)) //끝까지 읽어들인다
   {
       char tmpStr[200];            //200자를 읽을 수 있게 버퍼
       fgets(tmpStr,200,fp_button); //최대 200자를 읽어봄
       //printf ("%s",tmpStr);
       if (strcmp(tmpStr,HAVE_TO_FIND_1) == 0)
       {
           printf("YES! I found!: %s\r\n", tmpStr);
           returnValue = 1;      //찾음
       }
       if (
           (returnValue == 1) && //찾은 상태에서
           (strncasecmp(tmpStr, HAVE_TO_FIND_2, strlen(HAVE_TO_FIND_2)) == 0)) //Event??을 찾았으면
       {
           printf ("-->%s",tmpStr); 
           printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
           number = tmpStr[strlen(tmpStr)-3] - '0';
           //Ascii character '0'-'9' (0x30-0x39)
           //to interger(0)
           break;
       }
    }
   fclose(fp_button);
   if (returnValue == 1)
       sprintf (newPath,"%s%d",INPUT_DEVICE_LIST,number); // newPath에 찾은 경로 넣어줌
   return returnValue;
}
/*========================================================================================*/

/*============================== Button INIT ==================================*/
int buttonInit(void)
{
    char buttonPath[200] = {0,};
    if (probeButtonPath(buttonPath) == 0)     // event# 못찾으면 error 메세지
    {
      printf ("ERROR! File Not Found!\r\n");
      printf ("Did you insmod?\r\n");
      return 0;
    }
    printf ("buttonPath: %s\r\n", buttonPath); 
    fd_button=open (buttonPath, O_RDONLY);     // 찾은 경로 file open
    msgID = msgget (BUTTON_MESSAGE_ID, IPC_CREAT|0666);       // 버튼 메세지큐 생성
    if(msgID==-1){
      printf ("Cannot get msgQueueID, Return!\r\n");
      return -1;
   }   
    pthread_create(&buttonTh_id1, NULL, &buttonTh_snd, NULL); // 버튼 thread 생성
    return 1;
}
/*========================================================================================*/

/*============================== Button EXIT ==================================*/
int buttonExit(void)
{
    pthread_join(buttonTh_id1, NULL);  // buttonTh_id1 thread가 종료될 때까지 기다림
    close(fd_button);                  // file close          
    return 1;
}
/*========================================================================================*/

/*========================== 버튼 thread 함수 => 메세지 보냄 ===============================*/
void *buttonTh_snd(void *arg)
{
    int readSize, inputIndex;
    BUTTON_MSG_T Send;
    while(1)
    {        
        readSize = read(fd_button, &Button_EVENT, sizeof(Button_EVENT)); // 버튼 읽음         
        if (readSize != sizeof(Button_EVENT)) 
            continue;      
        Send.messageNum=Button_EVENT.type;
        Send.keyInput=Button_EVENT.code;
        Send.pressed=Button_EVENT.value;
        msgsnd(msgID, &Send, sizeof(Send)-4,0); // 메세지큐 사용해서 버튼 정보 보내기
    }
}
/*============================================================================================*/

/*=========================== 버튼 rcv함수 => 메세지 받음 ======================================*/
// tting.c의 inven_Func함수와 main함수에서 사용
int button_rcv()
{
    int returnValue=0;  
    BUTTON_MSG_T Recieve;
        while(1){
           int Value = 0;
           Value = msgrcv(msgID, &Recieve, sizeof(Recieve)-4, 0, IPC_NOWAIT);
           if (Value == -1) break;
         }

        returnValue=msgrcv(msgID, &Recieve, sizeof(Recieve)-4, 0, 0);
        if(Recieve.messageNum == 10 ) return 10; // 버튼 누르기 전에 인게임 카운터 만료되면 버튼입력 없이 탈출
        if ( Recieve.messageNum == EV_KEY)
        {
           if ( Recieve.pressed )
            {
            printf("EV_KEY(");
            switch(Recieve.keyInput) // 어떤 버튼 눌렀는지
              {
             case KEY_VOLUMEUP: printf("5OK)\r\n"); return 5;   // 행동 선택 확인 버튼 
             case KEY_HOME: printf("1Home key)\r\n"); return 1;
             case KEY_SEARCH: printf("3Left)\r\n"); return 3;   // 행동 선택 커서 왼쪽으로
             case KEY_BACK: printf("Back key)\r\n"); return 2;  
             case KEY_MENU: printf("4Right)\r\n"); return 4;    // 행동 선택 커서 오른쪽으로
             case KEY_VOLUMEDOWN: printf("6Volume On/Off)\r\n"); return 6; 
             default: break;
              }
            }
        }
        else;   
}
/*=====================================================================================================*/
