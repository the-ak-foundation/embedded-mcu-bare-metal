##############################################################################
# Makefile - 00-blink - STM32L151CBT6
##############################################################################

Print = @echo "~"

SOC_NAME            = STM32L151CBT6
SOC_FLASH_BYTES     = 131072
SOC_SRAM_BYTES      = 16384
FLASH_BASE_ADDR_VAL = 0x08000000
APP_START_ADDR_VAL  = 0x08003000

NAME_MODULE = 00-blink
PROJECT     = $(NAME_MODULE)

GCC_PATH = /home/phuoc/Workspace/Tools/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux/gcc-arm-none-eabi-10.3-2021.10

#|---------------------------------------------------------------------------------------------------|
#| OPTIMIZE LEVELS                                                                                   |
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   option   | optimization levels              |execution time|code size|memory usage|complile time|
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   -O0      | compilation time                 |     (+)      |   (+)   |     (-)    |    (-)      |
#|   -Os      | code size                        |              |   (--)  |            |    (++)     |
#|------------|----------------------------------|--------------|---------|------------|-------------|
OPTIMIZE_OPTION = -g3 -O0

WARNING_OPTION = -Wall -Wextra -Werror -Wundef -Wshadow

OBJECTS_DIR = build_$(NAME_MODULE)
TARGET      = $(OBJECTS_DIR)/$(NAME_MODULE).elf
LDFILE      = stm32l151xx.ld

SOURCES = bm_led_blink.c
OBJECTS = $(patsubst %.c, $(OBJECTS_DIR)/%.o, $(SOURCES))

CC       = $(GCC_PATH)/bin/arm-none-eabi-gcc
OBJCOPY  = $(GCC_PATH)/bin/arm-none-eabi-objcopy
OBJDUMP  = $(GCC_PATH)/bin/arm-none-eabi-objdump
ARM_SIZE = $(GCC_PATH)/bin/arm-none-eabi-size

CPU = -mthumb -march=armv7-m
FPU = -mfloat-abi=soft

GENERAL_FLAGS += \
		$(WARNING_OPTION) \
		$(OPTIMIZE_OPTION) \
		-DSTM32L1XX_MD \
		-DSTM32L151xB

COMPILER_FLAGS += \
		$(CPU) \
		$(FPU) \
		-ffunction-sections \
		-fdata-sections \
		-fno-common \
		-fno-builtin \
		-ffreestanding \
		-c

CFLAGS += \
		$(GENERAL_FLAGS) \
		$(COMPILER_FLAGS) \
		-std=c99

LDFLAGS = $(CPU) $(FPU) \
		-T$(LDFILE) \
		-nostartfiles -nostdlib \
		--specs=nano.specs -lc -lgcc \
		-Wl,-Map=$(OBJECTS_DIR)/$(PROJECT).map \
		-Wl,--gc-sections \
		-Wl,--cref

all: create $(TARGET) info

create:
	$(Print) CREATE $(OBJECTS_DIR) folder
	@mkdir -p $(OBJECTS_DIR)

$(TARGET): $(OBJECTS)
	$(Print) LD $@
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(Print) OBJCOPY $(@:.elf=.bin)
	@$(OBJCOPY) -O binary $(@) $(@:.elf=.bin)
	@$(ARM_SIZE) $(TARGET)

$(OBJECTS_DIR)/%.o: %.c
	$(Print) CC $@
	@$(CC) $(CFLAGS) -o $@ $<

.PHONY: help
help:
	$(Print) "[make]                       build firmware"
	$(Print) "[make flash]                 nap qua ST-Link"
	$(Print) "[make flash dev=/dev/ttyXX]  nap qua UART (AK bootloader)"
	$(Print) "[make info]                  print SOC, flash and RAM usage"
	$(Print) "[make clean]                 clean build folder"

.PHONY: flash
flash: all
ifdef dev
	$(Print) FLASH via UART $(dev) - $(TARGET:.elf=.bin) to $(APP_START_ADDR_VAL)
	@ak-flash $(dev) $(TARGET:.elf=.bin) $(APP_START_ADDR_VAL)
else
	$(Print) FLASH via ST-Link - $(TARGET:.elf=.bin) to $(APP_START_ADDR_VAL)
	@st-flash --connect-under-reset --reset write $(TARGET:.elf=.bin) $(APP_START_ADDR_VAL)
endif

.PHONY: clean
clean:
	$(Print) CLEAN $(OBJECTS_DIR) folder
	@rm -rf $(OBJECTS_DIR)

.PHONY: info
info: $(TARGET)
	@FLASH_USED=`$(ARM_SIZE) -A -d $(TARGET) | awk '$$1==".isr_vector" || $$1==".text" || $$1==".rodata" || $$1==".data" {sum += $$2} END {print sum+0}'`; \
	RAM_USED=`$(ARM_SIZE) -A -d $(TARGET) | awk '$$1==".data" || $$1==".bss" {sum += $$2} END {print sum+0}'`; \
	APP_FLASH_BYTES=$$(( $(SOC_FLASH_BYTES) - ($(APP_START_ADDR_VAL) - $(FLASH_BASE_ADDR_VAL)) )); \
	FLASH_PCT=`awk "BEGIN {printf \"%.2f\", ($$FLASH_USED * 100.0) / $$APP_FLASH_BYTES}"`; \
	RAM_PCT=`awk "BEGIN {printf \"%.2f\", ($$RAM_USED * 100.0) / $(SOC_SRAM_BYTES)}"`; \
	echo "MCU name: $(SOC_NAME)"; \
	printf -- " - Boot reserved : 0x%08X - 0x%08X (%d bytes)\n" $(FLASH_BASE_ADDR_VAL) $$(($(APP_START_ADDR_VAL) - 1)) $$(($(APP_START_ADDR_VAL) - $(FLASH_BASE_ADDR_VAL))); \
	printf -- " - App flash     : %d / %d bytes (%s%%)\n" $$FLASH_USED $$APP_FLASH_BYTES $$FLASH_PCT; \
	printf -- " - RAM           : %d / %d bytes (%s%%)\n" $$RAM_USED $(SOC_SRAM_BYTES) $$RAM_PCT