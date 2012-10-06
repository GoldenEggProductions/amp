# Golden Egg Productions
# Author: Chris Ostrum
# Modified: Alex Steinberg

CC=avr-gcc
MCU=atmega644a
OBJCOPY=avr-objcopy
INCLUDES=../common
CFLAGS=-I $(INCLUDES) -mcall-prologues -Os -mmcu=$(MCU) -Wall -Wstrict-prototypes

PROGRAMMER=avrdude
PROGRAMER_TYPE=avrispmkII
PROGRAMMER_MCU=m644
PROGRAMMER_PORT=usb
PROGRAMMER_ARG=-p $(PROGRAMMER_MCU) -c $(PROGRAMER_TYPE) -P $(PROGRAMMER_PORT) -e -U flash:w:

#
HEX=led_test.hex
OUT=led_test.out
CPP=led_test.c
OBJ=led_test.o
MAP=led_test.map

#-------------------
# Targets
#-------------------
all: $(HEX)

#-------------------
# Load
#-------------------
load: $(HEX)
	$(PROGRAMMER) $(PROGRAMMER_ARG)$(HEX)

#-------------------
$(HEX): $(OUT)
	$(OBJCOPY) -R .eeprom -O ihex $(OUT) $(HEX)
$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $(OUT) -Wl,-Map,$(MAP),--cref $(OBJ)
$(OBJ): $(CPP)
	$(CC) $(CFLAGS) -c $(CPP)

#-------------------
# Clean
#-------------------
clean:
	rm -f *.o *.out *.map $(HEX)

#-------------------
# Fuses
#-------------------
# fuse byte settings:
#  Fuse Low Byte      = 0xe1 (1MHz internal), 0xe3 (4MHz internal), 0xe4 (8MHz internal)#  Fuse High Byte     = 0xd9
#  Factory default is 0xe1 for low byte and 0xd9 for high byte
# Check this with make rdfuses
rdfuses:
	avrdude -p $(PROGRAMMER_MCU) -c $(PROGRAMER_TYPE) -P $(PROGRAMMER_PORT) -v -q
# use atmega8 internal RC oscillator 1 Mhz
wrfuses:
	avrdude -p  $(PROGRAMMER_MCU) -c $(PROGRAMER_TYPE) -P $(PROGRAMMER_PORT) -u -v -U lfuse:w:0b11011110:m
#-------------------
