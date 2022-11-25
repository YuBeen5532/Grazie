#ifndef _BUTTON_H_
#define _BUTTON_H_

#define MESSAGE_ID 1122

typedef struct
{
long int messageNum;
int keyInput;
int pressed;
} BUTTON_MSG_T;

int buttonInit(void);
void *buttonThFunc1(void *arg);
void *buttonThFunc2(void *arg);
int buttonExit(void);

#endif
