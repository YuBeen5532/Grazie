#ifndef _ACCELMAG_H_
#define _ACCELMAG_H_
#define ACCELPATH "/sys/class/misc/FreescaleAccelerometer/" //application 상에서 DD 억세스 위한 경로
void accInit(void);
int getAcc(void);
#endif

