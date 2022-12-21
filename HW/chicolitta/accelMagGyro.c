#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "accelMagGyro.h"

int fd_acc = 0;
FILE *fp_acc;

/*============================== Accel INIT ==============================*/
void accInit(void){                              // 가속도계 초기 설정

   fd_acc = open (ACCELPATH "enable",O_WRONLY);  // enable file open
   dprintf (fd_acc,"1");                         // enable file에 1 써준 후
   close(fd_acc);                                // file close
}
/*===================================================================================*/

/*============================== Accel 값 읽기 =================================*/
int getAcc(void){

   fp_acc = fopen (ACCELPATH "data", "rt");      // data 값 읽기 위해 file open
   int accel[3];
   fscanf(fp_acc,"%d, %d, %d",&accel[0],&accel[1],&accel[2]); // x축, y축, z축   
   fclose(fp_acc);                                            // file close
   
   return accel[0];                           // accel[0]값 => x축 가속도 return
}
/*===================================================================================*/
