#PLATFORM 	= linux
#PLATFORM 	= windows # not suported
#PLATFORM 	= rpi 1
#PLATFORM 	= rpi 2
#PLATFORM 	= rpi 3

BUILD  	  = release
#BUILD 			= debug

#LINKING  	= dynamic
#LINKING  	= static


ifeq ($(firstword $(PLATFORM)), rpi)
	ifeq (		 $(lastword $(PLATFORM)), 1)
		RPI_FLAGS = -mcpu=arm1176jzf-s \
			-mfloat-abi=hard \
			-mfpu=vfp 
	else ifeq ($(lastword $(PLATFORM)), 2)
		RPI_FLAGS = -mcpu=cortex-a7 \
			-mfloat-abi=hard \
			-mfpu=neon-vfpv4
	else ifeq ($(lastword $(PLATFORM)), 3)
		RPI_FLAGS = -mcpu=cortex-a53 \
			-mfloat-abi=hard \
			-mfpu=neon-fp-armv8 \
			-mneon-for-64bits
	endif

	SYSROOT = /media/nico/rootfs
	# replace this with the directory where you have mounted the
	# Raspberry Pi root directory

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

	SDL2_CONFIG = $(SYSROOT)/usr/bin/sdl2-config

else ifeq ($(firstword $(PLATFORM)), windows)
	CC 	= x86_64-w64-mingw32-gcc
	CXX = x86_64-w64-mingw32-g++

	SDL2_CONFIG = sdl2-config
else
	SDL2_CONFIG = sdl2-config
endif

ifneq (, $(findstring release, $(BUILD)))
	BUILD_FLAGS += -O3
endif
ifneq (, $(findstring debug, $(BUILD)))
	BUILD_FLAGS += -Wall -Wextra -pedantic -ggdb3
endif

CFLAGS   += $(BUILD_FLAGS)
CXXFLAGS += $(BUILD_FLAGS)

CFLAGS 	 += -I../../include
CXXFLAGS += -I../../include

SDL_CFLAGS += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --cflags sdl2)

CFLAGS 	 += $(SDL_CFLAGS)
CXXFLAGS += $(SDL_CFLAGS)

LDFLAGS += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-L sdl2)

ifeq (static, $(LINKING)) 
	LDLIBS 	+= $(filter-out -L%, $(shell \
		$(SDL2_CONFIG) --prefix=$(SYSROOT) --static-libs))
else
	LDFLAGS	+= $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
		pkg-config --libs-only-other sdl2)

	ifeq ($(firstword $(PLATFORM)), windows)
		AUX_LDFLAGS := $(LDFLAGS)
		ELF_FLAG := -Wl,--enable-new-dtags
		LDFLAGS = $(filter-out $(ELF_FLAG), $(AUX_LDFLAGS))
	endif

	LDLIBS 	+= $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
		pkg-config --libs-only-l     sdl2)
endif

CFLAGS 	 += -std=c11
CXXFLAGS += -std=c++11 -nostdinc++ -fno-exceptions -fno-rtti


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

%_edit.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -DESGE_EDITOR $< -o $@
%_edit.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -DESGE_EDITOR $< -o $@

clean:
	$(RM) *.o
