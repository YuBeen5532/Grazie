#include "fnd.h"
#include "fnddrv.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


#define FND_DRIVER_NAME "/dev/perifnd"

int fd_fnd;


void fnd_hour(int hour , int day)
{
	stFndWriteForm stWriteData;
	for (int i = 0; i < MAX_FND_NUM ; i++ )
    {
    stWriteData.DataDot[i] =  0;
    stWriteData.DataValid[i] = 1;
    }
	stWriteData.DataDot[2] =  1;
	int hour_temp, day_temp;	
	
	day_temp = day %1000; stWriteData.DataNumeric[0] = day_temp /100;
	day_temp = day %100; stWriteData.DataNumeric[1] = day_temp /10;
	stWriteData.DataNumeric[2] = day %10;
	
	hour_temp = hour % 1000; stWriteData.DataNumeric[3]= hour_temp /100;
	hour_temp = hour % 100; stWriteData.DataNumeric[4]= hour_temp /10;
	stWriteData.DataNumeric[5] = hour % 10;
	
	write(fd_fnd, &stWriteData, sizeof(stFndWriteForm));	
}


int fndInit(void){
	fd_fnd = open(FND_DRIVER_NAME,O_RDWR);
	if ( fd_fnd < 0 )
	{
		perror("driver open error.\n");
		return 0;
	}
}

int fndExit(void){
	close(fd_fnd);
	return 1;
}

int fndDisp(int num , int dotflag) //0-999999 숫자, 비트로 인코딩된 dot on/off
{
    int temp,i;
    stFndWriteForm stWriteData;
    for (i = 0; i < MAX_FND_NUM ; i++ )
    {
    stWriteData.DataDot[i] = (dotflag & (0x1 << i)) ? 1 : 0;
    stWriteData.DataValid[i] = 1;
    }
    // if 6 fnd
    temp = num % 1000000; stWriteData.DataNumeric[0]= temp /100000;
    temp = num % 100000; stWriteData.DataNumeric[1]= temp /10000;
    temp = num % 10000; stWriteData.DataNumeric[2] = temp /1000;
    temp = num % 1000; stWriteData.DataNumeric[3] = temp /100;
    temp = num %100; stWriteData.DataNumeric[4] = temp /10;
    stWriteData.DataNumeric[5] = num %10;


    write(fd_fnd, &stWriteData, sizeof(stFndWriteForm));

    return 1;
}



