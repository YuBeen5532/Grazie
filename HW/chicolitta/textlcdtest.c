#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR

int main(void){

    char text[20]="HELLO WORLD!";
    char text2[20]="GOOD BYE!";
    textlcdInit();
    lcdtextwrite(text,text,2);
    sleep(5);
    lcdtextwrite(NULL,text2,1);
    
    textlcdExit();

    return 0;
}