all: ledtest.c buttontest.c fndtest.c buzzertest.c colorledtest.c libMyPeri.a
   arm-linux-gnueabi-gcc -o colorledtest colorledtest.c -lMyPeri -L.
   arm-linux-gnueabi-gcc -o ledtest ledtest.c -lMyPeri -L.
   arm-linux-gnueabi-gcc -o fndtest fndtest.c -lMyPeri -L.
   arm-linux-gnueabi-gcc -o buttontest buttontest.c -lMyPeri -lpthread -L.
   arm-linux-gnueabi-gcc -o buzzertest buzzertest.c -lMyPeri -L.
libMyPeri.a: led.o led.h button.o button.h fnd.o fnd.h fnddrv.h buzzer.o buzzer.h colorLed.o colorLed.h
   arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o fnd.o buzzer.o colorLed.o
led.o: led.h led.c
   arm-linux-gnueabi-gcc -c led.c -o led.o
button.o: button.h button.c
   arm-linux-gnueabi-gcc -c button.c -o button.o -lpthread
fnd.o: fnd.h fnddrv.h fnd.c
   arm-linux-gnueabi-gcc -c fnd.c -o fnd.o
buzzer.o: buzzer.h buzzer.c
   arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o
colorLed.o: colorLed.h colorLed.c
   arm-linux-gnueabi-gcc -c colorLed.c -o colorLed.o


