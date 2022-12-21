#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H

float temp_read(void); // 온도센서 값 읽기
int spi_init(char filename[40]);
char * spi_read_lm74(int file);

#endif
