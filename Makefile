#
# wrapper for arduino-mk
#
# written by SvOlli
# distributed under the terms of the GPLv3 or later
#

#ifneq (,$(wildcard /opt/arduino))
#ARDUINO_DIR = /opt/arduino
#endif

# this configuration is for an Arduino Pro Mini
BOARD_TAG         ?= pro
MCU               ?= atmega168 # also 168a, 168p, 168pa, 168pb
F_CPU             ?= 16000000
ISP_LOW_FUSE      ?= 0xff
ISP_HIGH_FUSE     ?= 0xdf
ISP_EXT_FUSE      ?= 0xd8

# my programmer
ISP_PROG          ?= usbasp

MONITOR_PORT      ?= /dev/null
ISP_PORT          ?= /dev/null
MONITOR_BAUDRATE  ?= 115200
BUILD_BASE        := $(abspath $(lastword $(MAKEFILE_LIST))/..)/tmp
USER_LIB_PATH     ?= $(HOME)/Arduino/libraries
OBJDIR             = $(BUILD_BASE)
CFLAGS            += -Wall -Wno-error=sign-compare -Wno-error=unused-variable
CXXFLAGS          += -Wall -Wno-error=sign-compare -Wno-error=unused-variable -std=gnu++11
ifneq (,$(CONFIG))
CFLAGS            += -include "$(CONFIG)"
CXXFLAGS          += -include "$(CONFIG)"
endif

include /usr/share/arduino/Arduino.mk

debugmk:
	$(info $(sort $(.VARIABLES)))
	$(foreach v,$(sort $(.VARIABLES)),$(info $(v)="$($(v))"))
	false
