#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_SCALE_STEP 8

char gBuzzerBaseSysDir[128]; ///sys/busz/platform/devices/peribuzzer.XX 가 결정됨
const int musicScale[MAX_SCALE_STEP] =
{
    262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};



int main(int argc , char **argv)
{
    int freIndex;
    if (argc < 2 || findBuzzerSysPath() )
    {
        printf("Error!\n");
        doHelp();
        return 1;
    }

    freIndex = atoi(argv[1]);
    printf("freIndex :%d \n",freIndex);

    if ( freIndex > MAX_SCALE_STEP )
    {
        printf(" <buzzerNo> over range \n");
        doHelp();
        return 1;
    }

    if ( freIndex == 0)// disable
    {
        buzzerStopSong(0);
    }
    else
    {
        buzzerPlaySong(musicScale[freIndex-1]);
    }
    
    return 0;
}