PLATFORM = 
#rpi_1
BUILD    = debug
#release
TARGET	 = ESGE_sceneEditor

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

SRCS_C   = $(notdir $(wildcard ../src/util/*.c))

SRCS_CXX = $(notdir $(wildcard ../src/*.cpp) \
					 $(wildcard ../src/impl/*.cpp))

OBJS     = $(patsubst %.c,%.o, $(SRCS_C)) \
					 $(patsubst %.cpp,%.o, $(SRCS_CXX))

CFLAGS += -I../include

CFLAGS  += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --cflags          sdl2)
LDFLAGS += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-L     sdl2)
LDFLAGS += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-other sdl2)
LDLIBS  += $(shell PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
	pkg-config --libs-only-l     sdl2)

LDLIBS += -lstdc++

CPPFLAGS += -std=c++11 $(CFLAGS)


all: $(TARGET).o $(OBJS)
	$(CC) $(LDFLAGS) $(TARGET).o $(OBJS) $(LDLIBS) -o $(TARGET)
	
clean:
	$(RM) *.o