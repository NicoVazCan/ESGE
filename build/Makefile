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


vpath %.h 	../include
vpath %.c 	../src
vpath %.cpp ../src

OBJS_GAME = \
	ESGE_game.o \
	ESGE_io.o ESGE_stdlib.o ESGE_objSerial.o ESGE_file.o \
	ESGE_scene.o ESGE_display.o ESGE_objDraw.o ESGE_objActive.o \
	ESGE_objEvent.o ESGE_objUpdate.o ESGE_objPoint.o \
	ESGE_objCollider.o ESGE_spritesheet.o ESGE_objPhysic.o \
	ESGE_objStatic.o ESGE_objDynamic.o ESGE_objDrawSprite.o \
	ESGE_anim.o ESGE_objGrid.o audio.o ESGE_audio.o
	
OBJS_EDIT = \
	ESGE_editor_edit.o \
	ESGE_io.o ESGE_stdlib.o ESGE_objSerial.o ESGE_file.o \
	ESGE_scene_edit.o ESGE_display.o ESGE_objDraw.o ESGE_objActive.o \
	ESGE_objEvent.o ESGE_objUpdate.o ESGE_objPoint.o \
	ESGE_objCollider.o ESGE_spritesheet.o ESGE_objPhysic.o \
	ESGE_objStatic.o ESGE_objDynamic.o ESGE_objDrawSprite.o \
	ESGE_anim.o ESGE_objGrid.o audio.o ESGE_audio.o


all: libESGE_GAME.a libESGE_EDIT.a

libESGE_GAME.a: libESGE_GAME.a($(OBJS_GAME))
libESGE_EDIT.a: libESGE_EDIT.a($(OBJS_EDIT))

ESGE_game.o: ESGE_file.o ESGE_scene.o ESGE_display.o \
	ESGE_objEvent.o ESGE_objUpdate.o ESGE_objPhysic.o ESGE_objDraw.o \
	ESGE_audio.o
ESGE_io.o: ESGE_io.h
ESGE_objSerial.o: ESGE_objSerial.h ESGE_io.o ESGE_stdlib.o
ESGE_file.o: ESGE_file.h ESGE_stdlib.o ESGE_stdlib.o
ESGE_scene.o: ESGE_scene.h ESGE_io.h ESGE_file.h ESGE_stdlib.o
ESGE_display.o: ESGE_display.h ESGE_stdlib.o
ESGE_objDraw.o: ESGE_objDraw.h ESGE_stdlib.o
ESGE_objActive.o: ESGE_objActive.h ESGE_stdlib.o
ESGE_objEvent.o: ESGE_objEvent.h ESGE_stdlib.o
ESGE_objUpdate.o: ESGE_objUpdate.h ESGE_stdlib.o
ESGE_objPoint.o: ESGE_objPoint.h ESGE_stdlib.o
ESGE_objCollider.o: ESGE_objCollider.h ESGE_objPoint.o ESGE_stdlib.o
ESGE_spritesheet.o: ESGE_spritesheet.h ESGE_file.o ESGE_stdlib.o
ESGE_objPhysic.o: ESGE_objPhysic.h ESGE_objCollider.o ESGE_stdlib.o
ESGE_objStatic.o: ESGE_objStatic.h ESGE_objCollider.o ESGE_stdlib.o
ESGE_objDynamic.o: ESGE_objDynamic.h ESGE_objStatic.o ESGE_stdlib.o
ESGE_objDrawSprite.o: ESGE_objDrawSprite.h ESGE_stdlib.o
ESGE_anim.o: ESGE_anim.h ESGE_display.o ESGE_spritesheet.o \
	ESGE_stdlib.o
ESGE_objGrid.o: ESGE_objGrid.h ESGE_objPoint.o ESGE_stdlib.o
audio.o: ../src/audio.h
ESGE_audio.o: ESGE_audio.h audio.o ESGE_stdlib.o

%_edit.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -DESGE_EDITOR $< -o $@
%_edit.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -DESGE_EDITOR $< -o $@

ESGE_editor_edit.o: ESGE_file.o ESGE_scene_edit.o ESGE_display.o \
	ESGE_objDraw.o ESGE_audio.o
ESGE_scene_edit.o: ESGE_io.h ESGE_file.h ESGE_stdlib.o \
	ESGE_objSerial.o

clean:
	$(RM) *.o
