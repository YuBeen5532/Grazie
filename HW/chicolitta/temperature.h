#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H

int spi_init(char filename[40]);
char * spi_read_lm74(int file);
double temp_read(void);

#endif