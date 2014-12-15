####################################
#
#  MAKEFILE:	STM32F407VG
#  AUTHOR: 		SHARAN JUANGPHANICH
#  DATE: 		27 November 2014
#
# ##################################

TARGET		:= Audio_Visualizer

STLINK		:= ../stlink
STM_COMMON 	:= ../STM32F4xx_DSP_StdPeriph_Lib_V1.4.0

SRCDIR 		:= src
INCDIR		:= include
BINDIR 		:= bin

SOURCES		:= $(wildcard $(SRCDIR)/*.c)
SOURCES 	+= $(wildcard $(SRCDIR)/*.s)

CC			:= arm-none-eabi-gcc
OBJCOPY 	:= arm-none-eabi-objcopy

CFLAGS  	:= -g -O2 -Wall -Tstm32_flash.ld
CFLAGS		+= -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS 		+= -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS		+= -DSTM32F40_41xxx -DHSE_VALUE="8000000" -DUSE_STDPERIPH_DRIVER
CFLAGS		+= -DARM_MATH_CM4

LIBS 		:= $(STM_COMMON)/Libraries/CMSIS/Lib/GCC/libarm_cortexM4lf_math.a
CFLAGS 		+= -I$(STM_COMMON)/Libraries/CMSIS/Include
CFLAGS 		+= -I$(STM_COMMON)/Libraries/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS 		+= -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc
CFLAGS 		+= -I$(INCDIR)

RM			:= rm -f

.PHONY: project
all: project
project: $(BINDIR)/$(TARGET).elf

$(BINDIR)/$(TARGET).elf: $(SOURCES)
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
	@$(OBJCOPY) -O ihex $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).hex
	@$(OBJCOPY) -O binary $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).bin
	@echo "Compiled successfully!"

.PHONY: clean
clean:
	@$(RM) $(BINDIR)/$(TARGET).*
	@echo "Target(s) removed!"

.PHONY: burn
burn: project
	@$(STLINK)/st-flash write $(BINDIR)/$(TARGET).bin 0x8000000
	@echo "Board flashed!"
