#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include "textlcd.h"
#include "textlcddrv.h"


int main(void){

    char text[20]="HELLO WORLD!";
    char text2[20]="GOOD BYE!";
    textlcdInit();
   levelwrite(5);
    //lcdtextwrite(text,text,2);
    tempwrite(37.5);
   sleep(2);
   tempwrite(40.5);
    
   //lcdtextwrite(NULL,text2,1);
    
    textlcdExit();

    return 0;
}
