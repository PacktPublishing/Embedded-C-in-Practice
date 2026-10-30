TARGET := 03b-driver-callbacks

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
    $(BUILD_DIR)/app/report.o \
    $(BUILD_DIR)/app/soft_timer.o \
    $(BUILD_DIR)/app/event_queue.o \
    $(BUILD_DIR)/app/fsm.o \
    $(BUILD_DIR)/app/limits.o \
    $(BUILD_DIR)/drivers/log.o \
    $(BUILD_DIR)/drivers/bme280.o \
    $(BUILD_DIR)/drivers/button.o \
    $(BUILD_DIR)/drivers/console.o \
    $(BUILD_DIR)/drivers/led.o \
    $(BUILD_DIR)/drivers/system_timer.o \
	$(BUILD_DIR)/drivers/soil_moisture.o \
	$(BUILD_DIR)/drivers/fan.o \
	$(BUILD_DIR)/drivers/power_manager.o \
    $(BUILD_DIR)/hal/gpio.o \
    $(BUILD_DIR)/hal/uart.o \
    $(BUILD_DIR)/hal/i2c.o \
    $(BUILD_DIR)/hal/exti.o \
    $(BUILD_DIR)/hal/systick.o \
    $(BUILD_DIR)/hal/timer.o \
	$(BUILD_DIR)/hal/adc.o \
	$(BUILD_DIR)/hal/pwm.o \
	$(BUILD_DIR)/hal/power.o \
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

$(BUILD_DIR)/app/report.o: app/report.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/app/soft_timer.o: app/soft_timer.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/app/event_queue.o: app/event_queue.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/app/fsm.o: app/fsm.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/app/limits.o: app/limits.c
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

$(BUILD_DIR)/drivers/console.o: drivers/console.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/system_timer.o: drivers/system_timer.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/soil_moisture.o: drivers/soil_moisture.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/fan.o: drivers/fan.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/power_manager.o: drivers/power_manager.c
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

$(BUILD_DIR)/hal/exti.o: hal/exti.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hal/systick.o: hal/systick.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hal/timer.o: hal/timer.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hal/adc.o: hal/adc.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hal/pwm.o: hal/pwm.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hal/power.o: hal/power.c
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