PLATFORM_NAME = 
PLATFORM_VER  =
#rpi
BUILD    			= debug
#debug
#release

ifeq ($(PLATFORM_NAME), rpi)
	ifeq (		 $(PLATFORM_VER), 1)
		CFLAGS += -mcpu=arm1176jzf-s \
			-mfloat-abi=hard \
			-mfpu=vfp 
	else ifeq ($(PLATFORM_VER), 2)
		CFLAGS += -mcpu=cortex-a7 \
			-mfloat-abi=hard \
			-mfpu=neon-vfpv4
	else ifeq ($(PLATFORM_VER), 3)
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

CFLAGS 	+= -I../../include

CFLAGS  += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --cflags          sdl2)
LDFLAGS += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-L     sdl2)
LDFLAGS += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-other sdl2)
LDLIBS  += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-l     sdl2)

CXXFLAGS += -std=c++11 -nostdinc++ -fno-exceptions -fno-rtti $(CFLAGS)


vpath %.h ../../include
vpath %.a ../../build

SRCS = $(wildcard *.cpp)

OBJS_GAME = $(SRCS:.cpp=.o)

OBJS_EDIT = $(SRCS:.cpp=_edit.o)

all: game editor

game: $(OBJS_GAME) -lESGE_GAME
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

editor: $(OBJS_EDIT) -lESGE_EDIT
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

player.o: ESGE_scene.h ESGE_spritesheet.h ESGE_anim.h ESGE_time.h \
	ESGE_audio.h ESGE_file.h ESGE_objStatic.h ESGE_objDynamic.h \
	ESGE_objUpdate.h ESGE_objDrawSprite.h

tileCol.o: ESGE_scene.h ESGE_display.h ESGE_objStatic.h ESGE_objDraw.h

tileSet.o: ESGE_scene.h

%_edit.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -DESGE_EDITOR $< -o $@
%_edit.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -DESGE_EDITOR $< -o $@

player_edit.o: ESGE_scene.h ESGE_spritesheet.h ESGE_anim.h \
	ESGE_time.h ESGE_audio.h ESGE_file.h ESGE_objStatic.h \
	ESGE_objDynamic.h ESGE_objUpdate.h ESGE_objDrawSprite.h

tileCol_edit.o: ESGE_scene.h ESGE_display.h ESGE_objStatic.h \
	ESGE_objDraw.h

tileSet_edit.o: ESGE_scene.h

clean:
	$(RM) *.o
