all: ledtest.c buttontest.c fndtest.c libMyPeri.a
	arm-linux-gnueabi-gcc -o ledtest ledtest.c -lMyPeri -L.
	arm-linux-gnueabi-gcc -o fndtest fndtest.c -lMyPeri -L.
	arm-linux-gnueabi-gcc -o buttontest buttontest.c -lMyPeri -lpthread -L.
libMyPeri.a: led.o led.h button.o button.h fnd.o fnd.h fnddrv.h
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o fnd.o 
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
button.o: button.h button.c
	arm-linux-gnueabi-gcc -c button.c -o button.o -lpthread
fnd.o: fnd.h fnddrv.h fnd.c
	arm-linux-gnueabi-gcc -c fnd.c -o fnd.o

