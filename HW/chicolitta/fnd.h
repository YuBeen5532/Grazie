#ifndef _FND_H_
#define _FND_H_

#define FND_MESSAGE_ID 500

int fndInit(void);
int fndExit(void);
void fnd_hour(int hour , int day); // FND에 인게임 일차와 시간 출력

int fndDisp(int num , int dotflag);

#endif
