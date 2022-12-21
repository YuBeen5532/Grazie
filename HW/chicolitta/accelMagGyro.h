#ifndef _ACCELMAG_H_
#define _ACCELMAG_H_
#define ACCELPATH "/sys/class/misc/FreescaleAccelerometer/" //application에서 access하기 위한 경로

void accInit(void);
int getAcc(void);

#endif

