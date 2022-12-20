#ifndef _BUTTON_H_
#define _BUTTON_H_

#define BUTTON_MESSAGE_ID 300
#define timeout 30

typedef struct
{
long int messageNum;
int keyInput;
int pressed;
} BUTTON_MSG_T;

int buttonInit(void);
void *buttonTh_snd(void *arg);
int button_rcv(void);
int buttonExit(void);

#endif

