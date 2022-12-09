#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "accelMagGyro.h"

int main()
{
    accInit();
    getAcc();
    
    magInit();
    getMag();

	gyroInit();
	getGyro();
    
    return 0;
}
