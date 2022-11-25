all: ledtest.c fndtest.c libMyPeri.a
	arm-linux-gnueabi-gcc -o ledtest ledtest.c -lMyPeri -L.
	arm-linux-gnueabi-gcc -o fndtest fndtest.c -lMyPeri -L.
	arm-linux-gnueabi-gcc -o fndtest buttontest.c -lMyPeri -L.
libMyPeri.a: led.o led.h fnd.o fnd.h fnddrv.h
	arm-linux-gnueabi-ar rc libMyPeri.a led.o fnd.o button.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
fnd.o: fnd.h fnddrv.h fnd.c
	arm-linux-gnueabi-gcc -c fnd.c -o fnd.o
button.o: button.h buttondrv.h button.c
	arm-linux-gnueabi-gcc -c button.c -o button.o	

