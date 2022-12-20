#ifndef _MY_MESSAGE_H_
#define _MY_MESSAGE_H_
#define DISPLAY_MESSAGE_ID 100
typedef struct
{
long int messageType;
int display_msg; //(타이틀화면1, 성장화면2, 밥주기3, 물주기4, 청소하기5)
} DISPLAY_Msg_T;
#endif
