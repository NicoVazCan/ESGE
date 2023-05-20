PLATFORM = 
BUILD    = debug
TARGET   = libREF.a
SRCS		 = REF.c 
OBJS     = $(SRCS:.c=.o)


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

	CFLAGS += --sysroot=$(SYSROOT) \
		-I$(SYSROOT)/opt/vc/include \
		-I$(SYSROOT)/usr/include \
		-I$(SYSROOT)/opt/vc/include/interface/vcos/pthreads \
		-I$(SYSROOT)/opt/vc/include/interface/vmcs_host/linux

	LDLIBS  += -L$(SYSROOT)/opt/vc/lib

	CC = /opt/rpi-tools/arm-bcm2708$\
		/gcc-linaro-arm-linux-gnueabihf-raspbian$\
		/bin/arm-linux-gnueabihf-gcc
endif

ifeq ($(BUILD), release)
	CFLAGS += -O3
else ifeq ($(BUILD), debug)
	CFLAGS += -Wall -Wextra -pedantic -ggdb3
endif


CFLAGS += -I ../SGLIB


all: $(TARGET)

test: $(TARGET)

%.a: $(OBJS)
	$(AR) r $@ $(OBJS)
	$(RM) $(OBJS)
