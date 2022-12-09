#ifndef _ACCELMAG_H_
#define _ACCELMAG_H_
#define ACCELPATH "/sys/class/misc/FreescaleAccelerometer/"
#define MAGNEPATH "/sys/class/misc/FreescaleMagnetometer/"
#define GYROPATH "/sys/class/misc/FreescaleGyroscope/"
int accInit(void);
int magInit(void);
int gyroInit(void);
int* getAcc(void);
int getMag(void);
int getGyro(void);
#endif
