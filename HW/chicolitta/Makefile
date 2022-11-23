ledtest: ledtest.c libMyPeri.a
	arm-linux-gnueabi-gcc -o ledtest ledtest.c -lMyPeri -L.
libMyPeri.a: led.o led.h
	arm-linux-gnueabi-ar rc libMyPeri.a led.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o

