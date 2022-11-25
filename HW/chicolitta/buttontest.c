#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include <pthread.h>
//#include "button.h" 넣어야하나 

// first read input device
#define INPUT_DEVICE_LIST "/dev/input/event"
//실제 디바이스 드라이버 노드파일: 뒤에 숫자가 붙음., ex)/dev/input/event5
#define PROBE_FILE "/proc/bus/input/devices"
//PPT에 제시된 "이 파일을 까보면 event? 의 숫자를 알수 있다"는 바로 그 파일

int main(int argc, char *argv[])
{
    struct input event stEvent;    
    buttonInit();
    //int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);	

    while(1){
    
        //메세지 받는내용을 여기에 적어야하나?????
        //msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
	    if ( stEvent.type == EV_KEY)
	    {
		    printf("EV_KEY(");
		    switch(stEvent.code) // 어떤 버튼 눌렀는지
		    {
			    case KEY_VOLUMEUP: printf("Volume up key):"); break;
			    case KEY_HOME: printf("Home key):"); break;
			    case KEY_SEARCH: printf("Search key):"); break;
			    case KEY_BACK: printf("Back key):"); break;
			    case KEY_MENU: printf("Menu key):"); break;
			    case KEY_VOLUMEDOWN: printf("Volume down key):"); break;
		    }
	        if ( stEvent.value ) printf("pressed\n"); // 버튼 눌렸는지 안눌렸는지
	        else printf("released\n");
	    } //End of if
	    else; // EV_SYN
	        // do notthing
	} // End of While
	buttonExit();
}

