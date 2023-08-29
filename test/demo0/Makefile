PLATFORM_NAME =
PLATFORM_VER  =
#rpi
BUILD    			= release
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
