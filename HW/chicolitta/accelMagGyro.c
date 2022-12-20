#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "accelMagGyro.h"


int fd_acc = 0;
FILE *fp_acc;

void accInit(void){

   fd_acc = open (ACCELPATH "enable",O_WRONLY);
   dprintf (fd_acc,"1");
   close(fd_acc);
}


int getAcc(void){
   fp_acc = fopen (ACCELPATH "data", "rt");
   int accel[3];
   fscanf(fp_acc,"%d, %d, %d",&accel[0],&accel[1],&accel[2]);
   
   fclose(fp_acc);
   
   return accel[0];

}
