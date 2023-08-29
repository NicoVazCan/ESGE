PLATFORM_NAME =
PLATFORM_VER  =
#rpi
BUILD    			= debug
#debug
#release

ifeq ($(PLATFORM_NAME), rpi)
	ifeq (		 $(PLATFORM_VER), 1)
		RPI_FLAGS = -mcpu=arm1176jzf-s \
			-mfloat-abi=hard \
			-mfpu=vfp 
	else ifeq ($(PLATFORM_VER), 2)
		RPI_FLAGS = -mcpu=cortex-a7 \
			-mfloat-abi=hard \
			-mfpu=neon-vfpv4
	else ifeq ($(PLATFORM_VER), 3)
		RPI_FLAGS = -mcpu=cortex-a53 \
			-mfloat-abi=hard \
			-mfpu=neon-fp-armv8 \
			-mneon-for-64bits
	endif

	SYSROOT = /media/nico/rootfs

	RPI_FLAGS += \
		-I$(SYSROOT)/opt/vc/include \
		-I$(SYSROOT)/usr/include \
		-I$(SYSROOT)/opt/vc/include/interface/vcos/pthreads \
		-I$(SYSROOT)/opt/vc/include/interface/vmcs_host/linux

	CFLAGS   += $(RPI_FLAGS)
	CXXFLAGS += $(RPI_FLAGS)

	LDLIBS  += -L$(SYSROOT)/opt/vc/lib

	CC = /opt/rpi-tools/arm-bcm2708$\
		/gcc-linaro-arm-linux-gnueabihf-raspbian$\
		/bin/arm-linux-gnueabihf-gcc --sysroot=$(SYSROOT)
	CXX = /opt/rpi-tools/arm-bcm2708$\
		/gcc-linaro-arm-linux-gnueabihf-raspbian$\
		/bin/arm-linux-gnueabihf-g++ --sysroot=$(SYSROOT)
endif

ifeq ($(BUILD), release)
	BUILD_FLAGS += -O3
else ifeq ($(BUILD), debug)
	BUILD_FLAGS += -Wall -Wextra -pedantic -Og -ggdb3
endif

CFLAGS   += $(BUILD_FLAGS)
CXXFLAGS += $(BUILD_FLAGS)

CFLAGS   += -I../include
CXXFLAGS += -I../include

SDL_FLAGS += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --cflags          sdl2)
LDFLAGS 	+= $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-L     sdl2)
LDFLAGS 	+= $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-other sdl2)
LDLIBS  	+= $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-l     sdl2)

CFLAGS 	 += $(SDL_FLAGS)
CXXFLAGS += $(SDL_FLAGS)

CFLAGS 	 += -std=c11
CXXFLAGS += -std=c++11 -nostdinc++ -fno-exceptions -fno-rtti


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
	ESGE_anim.o ESGE_objCell.o audio.o ESGE_audio.o ESGE_objBox.o
	
OBJS_EDIT = \
	ESGE_editor_edit.o \
	ESGE_io.o ESGE_stdlib.o ESGE_objSerial.o ESGE_file.o \
	ESGE_scene_edit.o ESGE_display.o ESGE_objDraw.o ESGE_objActive.o \
	ESGE_objEvent.o ESGE_objUpdate.o ESGE_objPoint.o \
	ESGE_objCollider.o ESGE_spritesheet.o ESGE_objPhysic.o \
	ESGE_objStatic.o ESGE_objDynamic.o ESGE_objDrawSprite.o \
	ESGE_anim.o ESGE_objCell.o audio.o ESGE_audio.o ESGE_objBox.o


all: libESGE_GAME.a libESGE_EDIT.a

libESGE_GAME.a: libESGE_GAME.a($(OBJS_GAME))
libESGE_EDIT.a: libESGE_EDIT.a($(OBJS_EDIT))

ESGE_game.o: ESGE_scene.h ESGE_display.h ESGE_audio.h ESGE_error.h \
	ESGE_objEvent.h ESGE_objUpdate.h ESGE_objPhysic.h ESGE_objDraw.h
ESGE_io.o: ESGE_io.h
ESGE_objSerial.o: ESGE_objSerial.h ESGE_io.h
ESGE_file.o: ESGE_file.h
ESGE_scene.o: ESGE_scene.h ESGE_io.h ESGE_file.h
ESGE_display.o: ESGE_display.h
ESGE_objDraw.o: ESGE_objDraw.h
ESGE_objActive.o: ESGE_objActive.h
ESGE_objEvent.o: ESGE_objEvent.h
ESGE_objUpdate.o: ESGE_objUpdate.h
ESGE_objPoint.o: ESGE_objPoint.h
ESGE_objCollider.o: ESGE_objCollider.h ESGE_objBox.h
ESGE_spritesheet.o: ESGE_spritesheet.h ESGE_file.h
ESGE_objPhysic.o: ESGE_objPhysic.h ESGE_objCollider.h
ESGE_objStatic.o: ESGE_objStatic.h ESGE_objCollider.h
ESGE_objDynamic.o: ESGE_objDynamic.h ESGE_objStatic.h
ESGE_objDrawSprite.o: ESGE_objDrawSprite.h
ESGE_anim.o: ESGE_anim.h ESGE_display.h ESGE_spritesheet.h
ESGE_objCell.o: ESGE_objCell.h ESGE_objPoint.h
audio.o: ../src/audio.h
ESGE_audio.o: ESGE_audio.h ESGE_file.h ../src/audio.h
ESGE_objBox.o: ESGE_objBox.h ESGE_objPoint.h

%_edit.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -DESGE_EDITOR $< -o $@
%_edit.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -DESGE_EDITOR $< -o $@

ESGE_editor_edit.o: ESGE_scene.h ESGE_display.h ESGE_audio.h \
	ESGE_objDraw.h ESGE_error.h
ESGE_scene_edit.o: ESGE_io.h ESGE_file.h ESGE_stdlib.o \
	ESGE_objSerial.o

clean:
	$(RM) *.o *.a
