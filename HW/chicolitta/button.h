#ifndef _BUTTON_H_
#define _BUTTON_H_

#define BUTTON_MESSAGE_ID 300
#define timeout 30

// 버튼 메세지 구조체
typedef struct { 
    long int messageNum;
    int keyInput;        // 어떤 버튼인지
    int pressed;         // 버튼 눌림 여부
} BUTTON_MSG_T;

int buttonInit(void);
int buttonExit(void);
void *buttonTh_snd(void *arg);   // 버튼 thread 함수 => 메세지 보냄
int button_rcv(void);            // 버튼 rcv함수 => 메세지 받음 

#endif

