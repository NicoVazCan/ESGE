PLATFORM = 
#rpi_1
BUILD    = debug
#debug
#release

PLATFORM_FIELDS = $(subst _, ,$(PLATFORM))
PLATFORM_NAME   = $(firstword $(PLATFORM_FIELDS))
PLATFORM_VER    = $(lastword $(PLATFORM_FIELDS))

ifeq ($(PLATFORM_NAME),rpi)
	ifeq ($(PLATFORM_VER),1)
		CFLAGS += -mcpu=arm1176jzf-s \
			-mfloat-abi=hard \
			-mfpu=vfp 
	else ifeq ($(PLATFORM_VER),2)
		CFLAGS += -mcpu=cortex-a7 \
			-mfloat-abi=hard \
			-mfpu=neon-vfpv4
	else ifeq ($(PLATFORM_VER),3)
		CFLAGS += -mcpu=cortex-a53 \
			-mfloat-abi=hard \
			-mfpu=neon-fp-armv8 \
			-mneon-for-64bits
	endif

	SYSROOT = /media/nico/rootfs

	CFLAGS += \
		-I$(SYSROOT)/opt/vc/include \
		-I$(SYSROOT)/usr/include \
		-I$(SYSROOT)/opt/vc/include/interface/vcos/pthreads \
		-I$(SYSROOT)/opt/vc/include/interface/vmcs_host/linux

	LDLIBS  += -L$(SYSROOT)/opt/vc/lib

	CC = /opt/rpi-tools/arm-bcm2708$\
		/gcc-linaro-arm-linux-gnueabihf-raspbian$\
		/bin/arm-linux-gnueabihf-gcc --sysroot=$(SYSROOT)
	CXX = /opt/rpi-tools/arm-bcm2708$\
		/gcc-linaro-arm-linux-gnueabihf-raspbian$\
		/bin/arm-linux-gnueabihf-g++ --sysroot=$(SYSROOT)
endif

ifeq ($(BUILD), release)
	CFLAGS += -O3
else ifeq ($(BUILD), debug)
	CFLAGS += -Wall -Wextra -pedantic -ggdb3
endif

vpath %.h ../include
vpath %.c ../src:../src/util:../src/test:../src/impl
vpath %.cpp ../src:../src/util:../src/test:../src/impl

HDRS		 = $(wildcard ../include/*.h)

#SRCS_C   = $(notdir $(wildcard ../src/util/*.c))

SRCS_CXX = $(notdir $(wildcard ../src/*.cpp) \
					 $(wildcard ../src/impl/*.cpp))

#OBJS     = $(patsubst %.c,%.o, $(SRCS_C)) \
#					 $(patsubst %.cpp,%.o, $(SRCS_CXX))

OBJS = ESGE_io.o ESGE_stdlib.o ESGE_objSerial.o ESGE_file.o \
	ESGE_scene.o ESGE_display.o ESGE_objDraw.o ESGE_objActive.o \
	ESGE_objEvent.o ESGE_objUpdate.o ESGE_objPoint.o \
	ESGE_objCollider.o ESGE_spritesheet.o ESGE_objPhysic.o \
	ESGE_objStatic.o ESGE_objDynamic.o test.o

CFLAGS += -I../include

CFLAGS  += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --cflags          sdl2)
LDFLAGS += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-L     sdl2)
LDFLAGS += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-other sdl2)
LDLIBS  += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-l     sdl2)

CXXFLAGS += -std=c++11 -nostdinc++ -fno-exceptions -fno-rtti $(CFLAGS)

all: ESGE_editor ESGE_game

ESGE_editor: ESGE_editor.o $(OBJS)
	$(CC) $(LDFLAGS) ESGE_editor.o $(OBJS) $(LDLIBS) -o ESGE_editor

ESGE_game: ESGE_game.o $(OBJS)
	$(CC) $(LDFLAGS) ESGE_game.o $(OBJS) $(LDLIBS) -o ESGE_game

ESGE_io.o: ESGE_io.h

ESGE_objSerial.o: ESGE_objSerial.h ESGE_io.o ESGE_stdlib.o
ESGE_file.o: ESGE_file.h ESGE_stdlib.o ESGE_stdlib.o
ESGE_scene.o: ESGE_io.h ESGE_file.h ESGE_stdlib.o ESGE_stdlib.o
ESGE_display.o: ESGE_display.h ESGE_stdlib.o
ESGE_objDraw.o: ESGE_objDraw.h ESGE_stdlib.o
ESGE_objActive.o: ESGE_objActive.h ESGE_stdlib.o
ESGE_objEvent.o: ESGE_objEvent.h ESGE_stdlib.o
ESGE_objUpdate.o: ESGE_objUpdate.h ESGE_stdlib.o
ESGE_objPoint.o: ESGE_objPoint.h ESGE_stdlib.o
ESGE_objCollider.o: ESGE_objCollider.h ESGE_objPoint.o ESGE_stdlib.o
ESGE_spritesheet.o: ESGE_spritesheet.h ESGE_file.o ESGE_stdlib.o
ESGE_objPhysic.o: ESGE_objPhysic.h ESGE_stdlib.o
ESGE_objStatic.o: ESGE_objStatic.h ESGE_stdlib.o
ESGE_objDynamic.o: ESGE_objDynamic.h ESGE_stdlib.o

clean:
	$(RM) *.o