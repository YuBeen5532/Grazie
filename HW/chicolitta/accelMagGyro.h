#ifndef _ACCELMAG_H_
#define _ACCELMAG_H_
#define ACCELPATH "/sys/class/misc/FreescaleAccelerometer/"
void accInit(void);
int getAcc(void);
#endif

