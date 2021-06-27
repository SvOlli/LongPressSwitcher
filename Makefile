#
# wrapper for arduino-mk
#
# written by SvOlli
# distributed under the terms of the GPLv3 or later
#

BOARD_TAG          = pro
ISP_PROG           = usbasp
ISP_LOW_FUSE       = 0xff
ISP_HIGH_FUSE      = 0xdf
ISP_EXT_FUSE       = 0xd8
MONITOR_PORT       = /dev/ttyUSB0
MONITOR_BAUDRATE   = 115200
BUILD_BASE        := $(abspath $(lastword $(MAKEFILE_LIST))/..)/tmp
USER_LIB_PATH      = $(HOME)/Arduino/libraries
OBJDIR             = $(BUILD_BASE)
CFLAGS            += -Wall -Werror -Wno-error=sign-compare -Wno-error=unused-variable
CXXFLAGS          += -Wall -Werror -Wno-error=sign-compare -Wno-error=unused-variable -std=gnu++11

include /usr/share/arduino/Arduino.mk

debugmk:
	@echo $(current_dir)
	@echo "$(.VARIABLES)"
	false
