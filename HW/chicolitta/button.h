#ifndef _BUTTON_H_
#define _BUTTON_H_

#define BUTTON_MESSAGE_ID 300
#define timeout 30

typedef struct // 메시지큐 구조체
{
long int messageNum;
int keyInput; // 어떤 버튼
int pressed; // 눌렸는지 여부
} BUTTON_MSG_T;

int buttonInit(void);
void *buttonTh_snd(void *arg);
int button_rcv(void);
int buttonExit(void);

#endif

