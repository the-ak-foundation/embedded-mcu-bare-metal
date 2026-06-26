##############################################################################
# Makefile - 00-blink - STM32L151CBT6
##############################################################################

Print = @echo "~"

# SOC, flash and RAM info
SOC_NAME = STM32L151CBT6
SOC_FLASH_BYTES = 131072 # 128KB
SOC_SRAM_BYTES = 16384	 # 16KB
FLASH_BASE_ADDR_VAL = 0x08000000

# Name of build target ex: 00-blink.bin
NAME_MODULE = 00-blink
PROJECT = $(NAME_MODULE)

GCC_PATH		= /home/phuoc/Workspace/Tools/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux/gcc-arm-none-eabi-10.3-2021.10

APP_START_ADDR_VAL = 0x08003000
APP_ADDR_OPTION = -DAPP_START_ADDR=$(APP_START_ADDR_VAL)

#|---------------------------------------------------------------------------------------------------|
#| OPTIMIZE LEVELS                                                                                   |
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   option   | optimization levels              |execution time|code size|memory usage|complile time|
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   -O0      | compilation time                 |     (+)      |   (+)   |     (-)    |    (-)      |
#| -O1 || -O  | code size && execution time      |     (-)      |   (-)   |     (+)    |    (+)      |
#|   -O2      | more code size && execution time |     (--)     |         |     (+)    |    (++)     |
#|   -O3      | more code size && execution time |     (---)    |         |     (+)    |    (+++)    |
#|   -Os      | code size                        |              |   (--)  |            |    (++)     |
#|  -Ofast    | O3 with none math cals           |     (---)    |         |     (+)    |    (+++)    |
#|------------|----------------------------------|--------------|---------|------------|-------------|
OPTIMIZE_OPTION = -g3 -O0

# Warning option
WARNING_OPTION = -Wall -Wextra -Werror -Wundef -Wshadow

OBJECTS_DIR = build_$(NAME_MODULE)
TARGET = $(OBJECTS_DIR)/$(NAME_MODULE).elf
LDFILE = stm32l151xx.ld

SOURCES		= bm_led_blink.c
OBJECTS		= $(patsubst %.c, $(OBJECTS_DIR)/%.o, $(SOURCES))

# The command for calling the compiler.
CC		=	$(GCC_PATH)/bin/arm-none-eabi-gcc
OBJCOPY		=	$(GCC_PATH)/bin/arm-none-eabi-objcopy
OBJDUMP		=	$(GCC_PATH)/bin/arm-none-eabi-objdump
OBJNM		=	$(GCC_PATH)/bin/arm-none-eabi-nm
ARM_GDB		=	$(GCC_PATH)/bin/arm-none-eabi-gdb
ARM_SIZE	=	$(GCC_PATH)/bin/arm-none-eabi-size

MEMORY_USAGE_BAR_WIDTH = 30
FLASH_SECTION_FILTER = $$1==".isr_vector" || $$1==".text" || $$1==".rodata" || $$1==".data"
RAM_SECTION_FILTER = $$1==".data" || $$1==".bss"

define MEMORY_INFO_COLLECT
	FLASH_USED=`$(ARM_SIZE) -A -d $(TARGET) | awk '$(FLASH_SECTION_FILTER) {sum += $$2} END {print sum+0}'`; \
	RAM_USED=`$(ARM_SIZE) -A -d $(TARGET) | awk '$(RAM_SECTION_FILTER) {sum += $$2} END {print sum+0}'`; \
	BOOT_RESERVED_BYTES=$$(( $(APP_START_ADDR_VAL) - $(FLASH_BASE_ADDR_VAL) )); \
	APP_FLASH_BYTES=$$(( $(SOC_FLASH_BYTES) - $$BOOT_RESERVED_BYTES )); \
	FLASH_REMAIN_BYTES=$$(( $$APP_FLASH_BYTES - $$FLASH_USED )); \
	RAM_REMAIN_BYTES=$$(( $(SOC_SRAM_BYTES) - $$RAM_USED )); \
	BOOT_END_ADDR=$$(( $(APP_START_ADDR_VAL) - 1 )); \
	SOC_FLASH_END_ADDR=$$(( $(FLASH_BASE_ADDR_VAL) + $(SOC_FLASH_BYTES) - 1 )); \
	FLASH_USED_PCT=`awk "BEGIN {printf \"%.2f\", ($$FLASH_USED * 100.0) / $$APP_FLASH_BYTES}"`; \
	FLASH_REMAIN_PCT=`awk "BEGIN {printf \"%.2f\", ($$FLASH_REMAIN_BYTES * 100.0) / $$APP_FLASH_BYTES}"`; \
	RAM_USED_PCT=`awk "BEGIN {printf \"%.2f\", ($$RAM_USED * 100.0) / $(SOC_SRAM_BYTES)}"`; \
	RAM_REMAIN_PCT=`awk "BEGIN {printf \"%.2f\", ($$RAM_REMAIN_BYTES * 100.0) / $(SOC_SRAM_BYTES)}"`; \
	FLASH_USED_BAR=`awk "BEGIN {printf \"%d\", ($$FLASH_USED * $(MEMORY_USAGE_BAR_WIDTH)) / $$APP_FLASH_BYTES}"`; \
	FLASH_FREE_BAR=$$(( $(MEMORY_USAGE_BAR_WIDTH) - $$FLASH_USED_BAR )); \
	RAM_USED_BAR=`awk "BEGIN {printf \"%d\", ($$RAM_USED * $(MEMORY_USAGE_BAR_WIDTH)) / $(SOC_SRAM_BYTES)}"`; \
	RAM_FREE_BAR=$$(( $(MEMORY_USAGE_BAR_WIDTH) - $$RAM_USED_BAR )); \
	FLASH_USED_BLOCKS=`printf "%*s" $$FLASH_USED_BAR | tr ' ' '#'`; \
	FLASH_FREE_BLOCKS=`printf "%*s" $$FLASH_FREE_BAR | tr ' ' '-'`; \
	RAM_USED_BLOCKS=`printf "%*s" $$RAM_USED_BAR | tr ' ' '#'`; \
	RAM_FREE_BLOCKS=`printf "%*s" $$RAM_FREE_BAR | tr ' ' '-'`
endef

define MEMORY_INFO_PRINT
	echo "MCU name: $(SOC_NAME)"; \
	printf -- " - Boot/Reserved Flash : 0x%08X - 0x%08X (%d bytes)\n" $(FLASH_BASE_ADDR_VAL) $$BOOT_END_ADDR $$BOOT_RESERVED_BYTES; \
	printf -- " - Application Flash   : 0x%08X - 0x%08X (%d bytes)\n" $(APP_START_ADDR_VAL) $$SOC_FLASH_END_ADDR $$APP_FLASH_BYTES; \
	printf -- " - Flash info          : %d bytes / %d bytes\n" $$FLASH_USED $$APP_FLASH_BYTES; \
	printf -- " - Flash usage         : [%s%s] %s%% used / %s%% free\n" "$$FLASH_USED_BLOCKS" "$$FLASH_FREE_BLOCKS" "$$FLASH_USED_PCT" "$$FLASH_REMAIN_PCT"; \
	printf -- " - RAM info            : %d bytes / %d bytes\n" $$RAM_USED $(SOC_SRAM_BYTES); \
	printf -- " - RAM usage           : [%s%s] %s%% used / %s%% free\n" "$$RAM_USED_BLOCKS" "$$RAM_FREE_BLOCKS" "$$RAM_USED_PCT" "$$RAM_REMAIN_PCT"
endef

# Set the compiler CPU/FPU options.
# https://launchpadlibrarian.net/177524521/readme.txt
CPU = -mthumb -march=armv7-m
FPU = -mfloat-abi=soft

GENERAL_FLAGS +=			\
		$(WARNING_OPTION)	\
		$(OPTIMIZE_OPTION)	\
		$(APP_ADDR_OPTION)	\
		-DSTM32L1XX_MD		\
		-DSTM32L151xB		\

COMPILER_FLAGS +=			\
		$(CPU)			\
		$(FPU)			\
		-ffunction-sections	\
		-fdata-sections		\
		-fno-common		\
		-fno-builtin		\
		-ffreestanding		\
		-c			\

# C compiler flags
CFLAGS +=				\
		$(GENERAL_FLAGS)	\
		$(COMPILER_FLAGS)	\
		-std=c99		\

# linker flags
LDFLAGS	=	$(CPU) $(FPU)					\
		-T$(LDFILE)					\
		-nostartfiles -nostdlib				\
		--specs=nano.specs -lc -lgcc			\
		-Wl,-Map=$(OBJECTS_DIR)/$(PROJECT).map		\
		-Wl,--gc-sections				\
		-Wl,--cref

all: create $(TARGET) info

create:
	$(Print) CREATE $(OBJECTS_DIR) folder
	@mkdir -p $(OBJECTS_DIR)

$(TARGET): $(OBJECTS)
	$(Print) LD $@
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $(@)
	$(Print) OBJCOPY $(@:.elf=.bin)
	@$(OBJCOPY) -O binary $(@) $(@:.elf=.bin)
	@$(ARM_SIZE) $(TARGET)

$(OBJECTS_DIR)/%.o: %.c
	$(Print) CC $@
	@$(CC) $(CFLAGS) -o $@ $<

.PHONY: help
help:
	$(Print) "[make flash dev=/dev/tty*] burn firmware via ak bootloader"
	$(Print) "             Note: This option is needed install ak-flash. Link download & install: https://github.com/ak-embedded-software/ak-flash"
	$(Print) "             Example: make flash dev=/dev/ttyUSB0"
	$(Print) "[make flash] burn firmware via st-link"
	$(Print) "[make clean] clean build project folder"
	$(Print) "[make info] print SOC, flash and RAM usage"
	$(Print) "[make asm] generate asm file"
	$(Print) "[make sym] list symbol from objects file"

.PHONY: flash
flash: all
	$(Print) BURNING $(TARGET:.elf=.bin) to target
ifdef dev
	@ak-flash $(dev) $(TARGET:.elf=.bin) $(APP_START_ADDR_VAL)
else
	@st-flash --connect-under-reset --reset write $(TARGET:.elf=.bin) $(APP_START_ADDR_VAL)
endif

.PHONY: clean
clean:
	$(Print) CLEAN $(OBJECTS_DIR) folder
	@rm -rf $(OBJECTS_DIR)

.PHONY: info
info: $(TARGET)
	@echo "Memory usage info for $(TARGET):"
	@$(MEMORY_INFO_COLLECT); \
	$(MEMORY_INFO_PRINT)

asm: $(TARGET)
	$(Print) disassemble-all $(<:.elf=.asm)
	@$(OBJDUMP) -D $(<) >> $(<:.elf=.asm)

sym: $(TARGET)
	$(Print) export object name $(<:.elf=.sym)
	$(OBJNM) --size-sort --print-size $(<) >> $(<:.elf=.sym)