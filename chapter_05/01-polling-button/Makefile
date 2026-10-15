TARGET := 01-polling-button

CC      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE    := arm-none-eabi-size

BUILD_DIR := build

CPU_FLAGS := \
    -mcpu=cortex-m4 \
    -mthumb \
    -mfpu=fpv4-sp-d16 \
    -mfloat-abi=hard

CFLAGS := \
    $(CPU_FLAGS) \
    -std=c11 \
    -O0 \
    -g3 \
    -Wall \
    -Wextra \
    -ffreestanding \
    -fno-builtin \
    -DSTM32F401xE \
    -I. \
    -Iapp \
    -Idrivers \
    -Ihal \
    -Icore/CMSIS/Include \
    -Icore/CMSIS/Device/ST/STM32F4xx/Include

CFLAGS += -DBME280_ACCEPT_BMP280=1

LDFLAGS := \
    $(CPU_FLAGS) \
    -Tlinker.ld \
    -nostartfiles \
    --specs=nano.specs \
    --specs=nosys.specs \
    -u _printf_float \
    -Wl,--gc-sections \
    -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

OBJECTS := \
    $(BUILD_DIR)/main.o \
    $(BUILD_DIR)/app/delay.o \
    $(BUILD_DIR)/app/report_temperature.o \
    $(BUILD_DIR)/drivers/log.o \
    $(BUILD_DIR)/drivers/bme280.o \
    $(BUILD_DIR)/drivers/button.o \
    $(BUILD_DIR)/drivers/led.o \
    $(BUILD_DIR)/hal/gpio.o \
    $(BUILD_DIR)/hal/uart.o \
    $(BUILD_DIR)/hal/i2c.o \
    $(BUILD_DIR)/core/system_stm32f4xx.o \
    $(BUILD_DIR)/core/startup_stm32f401xx.o

.PHONY: all clean flash

all: $(BUILD_DIR)/$(TARGET).elf \
     $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

$(BUILD_DIR)/main.o: main.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/app/delay.o: app/delay.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/app/report_temperature.o: app/report_temperature.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/log.o: drivers/log.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/bme280.o: drivers/bme280.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/button.o: drivers/button.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/led.o: drivers/led.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hal/gpio.o: hal/gpio.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hal/uart.o: hal/uart.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hal/i2c.o: hal/i2c.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/core/system_stm32f4xx.o: core/system_stm32f4xx.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/core/startup_stm32f401xx.o: core/startup_stm32f401xx.s
	@mkdir -p $(dir $@)
	$(CC) $(CPU_FLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(BUILD_DIR)/$(TARGET).bin
	STM32_Programmer_CLI \
		-c port=SWD \
		-d $< 0x08000000 \
		-v \
		-hardRst

clean:
	rm -rf $(BUILD_DIR)