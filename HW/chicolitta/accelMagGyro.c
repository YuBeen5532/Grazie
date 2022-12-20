#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "accelMagGyro.h"


int fd_acc = 0;
FILE *fp_acc;

void accInit(void){ // 가속도계 초기 설정

   fd_acc = open (ACCELPATH "enable",O_WRONLY); // enable 파일 오픈
   dprintf (fd_acc,"1"); // 1 써주기
   close(fd_acc); // 파일 닫기
}


int getAcc(void){
   fp_acc = fopen (ACCELPATH "data", "rt"); // data 값 읽기 위해 파일 오픈
   int accel[3];
   fscanf(fp_acc,"%d, %d, %d",&accel[0],&accel[1],&accel[2]); // x축, y축, z축
   
   fclose(fp_acc); // 파일 클로즈
   
   return accel[0]; // x축 가속도 리턴

}
