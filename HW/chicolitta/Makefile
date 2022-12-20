all: tting.c ledtest.c buttontest.c fndtest.c buzzertest.c colorledtest.c textlcdtest.c temperatureTest.c tting.h libMyPeri.a
	arm-linux-gnueabi-gcc -o tting tting.c display.c -L. -L../ -L../libjpeg -l mylib -lMyPeri -l jpeg -lpthread
	arm-linux-gnueabi-gcc -o colorledtest colorledtest.c -lMyPeri -L.
	arm-linux-gnueabi-gcc -o ledtest ledtest.c -lMyPeri -L.
	arm-linux-gnueabi-gcc -o fndtest fndtest.c -lMyPeri -L.
	arm-linux-gnueabi-gcc -o buttontest buttontest.c -lMyPeri -lpthread -L.
	arm-linux-gnueabi-gcc -o buzzertest buzzertest.c -lMyPeri -L.
	arm-linux-gnueabi-gcc -o textlcdtest textlcdtest.c -lMyPeri -L.
	arm-linux-gnueabi-gcc -o temperaturetest temperatureTest.c -lMyPeri -L.
libMyPeri.a: led.o led.h button.o button.h fnd.o fnd.h fnddrv.h buzzer.o buzzer.h colorLed.o colorLed.h textlcd.o textlcd.h textlcddrv.h temperature.h temperature.o accelMagGyro.h accelMagGyro.o display.h display.o kakaotalk.h kakaotalk.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o fnd.o buzzer.o colorLed.o textlcd.o temperature.o accelMagGyro.o display.o kakaotalk.o
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
textlcd.o: textlcd.h textlcddrv.h textlcd.c
	arm-linux-gnueabi-gcc -c textlcd.c -o textlcd.o
temperature.o: temperature.h temperature.c
	arm-linux-gnueabi-gcc -c temperature.c -o temperature.o
accelMagGyro.o: accelMagGyro.h accelMagGyro.c
	arm-linux-gnueabi-gcc -c accelMagGyro.c -o accelMagGyro.o
display.o: display.h display.c
	arm-linux-gnueabi-gcc -c display.c -o display.o -L. -L../ -L../libjpeg -l mylib -l jpeg -lpthread
kakaotalk.o: kakaotalk.h kakaotalk.c
	arm-linux-gnueabi-gcc -c kakaotalk.c -o kakaotalk.o -lpthread
