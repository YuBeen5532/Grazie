#include "buzzer.h"
#include <unistd.h>
#include <fcntl.h>

int buzzerPlaySong(int scale)
int buzzerStopSong(void)

int buzzerInit(void)
{
	fd = open("/sys/bus/platform/devices/peribuzzer.XX/enable", O_WRONLY);
	
}


int buzzerExit(void)
{
	close(fd);
}









int findBuzzerSysPath(){
DIR * dir_info = opendir(BUZZER_BASE_SYS_PATH);
int ifNotFound = 1;
if (dir_info != NULL){
while (1){
struct dirent *dir_entry;
dir_entry = readdir (dir_info);
if (dir_entry == NULL) break;
if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0){
ifNotFound = 0;
sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH,dir_entry->d_name);
}
}
}
printf("find %s\n",gBuzzerBaseSysDir);
return ifNotFound;
}
//버저 경로 찾기: /sys/bus/platform/devices/peribuzzer.XX 의 XX를 결정하는 것

void doHelp(void)
{
printf("Usage:\n");
printf("buzzertest <buzzerNo> \n");
printf("buzzerNo: \n");
printf("do(1),re(2),mi(3),fa(4),sol(5),ra(6),si(7),do(8) \n");
printf("off(0)\n");
}
void buzzerEnable(int bEnable)
{
char path[200];
sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
int fd=open(path,O_WRONLY);
if ( bEnable)
write(fd, &"1", 1);
else
write(fd, &"0", 1);
close(fd);
}

void setFrequency(int frequency)
{
char path[200];
sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
int fd=open(path,O_WRONLY);
dprintf(fd, "%d", frequency);
close(fd);
}
