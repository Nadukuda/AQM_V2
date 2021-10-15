################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include zcl-core-cli/subdir.mk
-include zcl-core/subdir.mk
-include version-debug/subdir.mk
-include udp-cli/subdir.mk
-include stack/thread-stack/EFR32/subdir.mk
-include stack/thread-stack/subdir.mk
-include slaac-client/subdir.mk
-include serial/EFR32/subdir.mk
-include serial/subdir.mk
-include scan-debug/subdir.mk
-include ota-bootload-storage-eeprom/subdir.mk
-include ota-bootload-storage-cli/subdir.mk
-include ota-bootload-core/subdir.mk
-include ota-bootload-client-policy/subdir.mk
-include ota-bootload-client/subdir.mk
-include nvm3/EFR32/subdir.mk
-include nvm3/subdir.mk
-include network-management-cli/subdir.mk
-include main/subdir.mk
-include led/EFR32/subdir.mk
-include idle-sleep/subdir.mk
-include icmp-debug/subdir.mk
-include icmp-cli/subdir.mk
-include heartbeat/subdir.mk
-include hal-library/EFR32/subdir.mk
-include fem-control/subdir.mk
-include external-generated-files/subdir.mk
-include emlib/subdir.mk
-include emdrv/subdir.mk
-include eeprom/subdir.mk
-include dtls-auth-params/subdir.mk
-include diagnostic/EFR32/subdir.mk
-include dhcp-client/subdir.mk
-include debug-print/subdir.mk
-include debug-channel/EFR32/subdir.mk
-include command-interpreter2/subdir.mk
-include coexistence/EFR32/subdir.mk
-include coap-dispatch/subdir.mk
-include coap-cli/subdir.mk
-include cli/subdir.mk
-include buzzer/EFR32/subdir.mk
-include button/EFR32/subdir.mk
-include antenna-stub/subdir.mk
-include address-configuration-debug/subdir.mk
-include adc/EFR32/subdir.mk
-include VMN/VMN_RTCC/subdir.mk
-include VMN/Solenoid_Control/subdir.mk
-include VMN/SI7021/Internal/subdir.mk
-include VMN/SI7021/External/subdir.mk
-include VMN/SDS011/subdir.mk
-include VMN/Network_Analysis/subdir.mk
-include VMN/I2C_drivers/i2cspm/subdir.mk
-include VMN/I2C_drivers/i2c-driver-stub/subdir.mk
-include VMN/Gas_Sensor/subdir.mk
-include VMN/Fan_Control/subdir.mk
-include VMN/Data_Queue/subdir.mk
-include VMN/subdir.mk
-include Data_Protocol/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pre-build main-build

# Main-build Target
main-build: AQM-Firmware-V2.axf

# Tool invocations
AQM-Firmware-V2.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/base/hal/micro/cortexm3/efm32/gcc-cfg.ld" -L"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/base/hal/micro/cortexm3/" -Xlinker --defsym=FLASH_SIZE=1048576 -Xlinker --defsym=RAM_SIZE=262144 -Xlinker --defsym=SIMEEPROM_SIZE=36864 -Xlinker --defsym=LOCKBITS_IN_MAINFLASH_SIZE=0 -Xlinker --defsym=APP_GECKO_INFO_PAGE_BTL=1 -Xlinker --defsym=EMBER_MALLOC_HEAP_SIZE=0 -Xlinker --gc-sections -Xlinker -Map="AQM-Firmware-V2.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o AQM-Firmware-V2.axf -Wl,--start-group "./Data_Protocol/VMN_CMD.o" "./Data_Protocol/VMN_COAP.o" "./Data_Protocol/VMN_DAT.o" "./Data_Protocol/VMN_SET.o" "./Data_Protocol/VMN_ack-nack.o" "./Data_Protocol/VMN_checksum-generator.o" "./Data_Protocol/VMN_data-handler.o" "./Data_Protocol/VMN_payload-packager.o" "./VMN/Air-Quality-Control.o" "./VMN/Data_Queue/Buffer_Management.o" "./VMN/Data_Queue/Queue.o" "./VMN/Fan_Control/I2C-Fan-Control.o" "./VMN/Gas_Sensor/VMN_Gas-sensor.o" "./VMN/I2C_drivers/i2c-driver-stub/VMN-I2C0-Stub.o" "./VMN/I2C_drivers/i2c-driver-stub/VMN-I2C1-Stub.o" "./VMN/I2C_drivers/i2cspm/i2cspm.o" "./VMN/Network_Analysis/Scan_data.o" "./VMN/SDS011/AQM_SDS011-sensor.o" "./VMN/SDS011/I2C-aqm-control.o" "./VMN/SDS011/vortex-aqm-events.o" "./VMN/SI7021/External/si7021.o" "./VMN/SI7021/External/si7021_events.o" "./VMN/SI7021/Internal/internal_si7021.o" "./VMN/SI7021/Internal/internal_si7021_events.o" "./VMN/Solenoid_Control/I2C-Solenoid-Control.o" "./VMN/VMN_RTCC/VMN_RTCC.o" "./adc/EFR32/adc-efr32.o" "./address-configuration-debug/address-configuration-debug.o" "./antenna-stub/antenna-stub.o" "./button/EFR32/button.o" "./buzzer/EFR32/buzzer-efr32.o" "./cli/cli.o" "./client-implementation.o" "./thread-bookkeeping.o" "./thread-callback-stubs.o" "./thread-callbacks.o" "./thread-cli.o" "./thread-coap-dispatch.o" "./thread-events.o" "./thread-zclip.o" "./coap-cli/coap-cli.o" "./coap-dispatch/coap-dispatch.o" "./coexistence/EFR32/coexistence-802154.o" "./coexistence/EFR32/coexistence-directional-priority.o" "./coexistence/EFR32/coexistence-hal.o" "./coexistence/EFR32/coexistence.o" "./command-interpreter2/command-interpreter2-error.o" "./command-interpreter2/command-interpreter2-util.o" "./command-interpreter2/command-interpreter2.o" "./debug-channel/EFR32/debug-jtag-efr32.o" "./debug-print/debug-print.o" "./dhcp-client/dhcp-client.o" "./diagnostic/EFR32/diagnostic.o" "./dtls-auth-params/dtls-auth-params.o" "./eeprom/eeprom-cli.o" "./eeprom/eeprom-print.o" "./eeprom/eeprom.o" "./emdrv/dmadrv.o" "./emdrv/gpiointerrupt.o" "./emdrv/rtcdriver.o" "./emdrv/sleep.o" "./emdrv/tempdrv.o" "./emdrv/uartdrv.o" "./emdrv/ustimer.o" "./emlib/em_adc.o" "./emlib/em_cmu.o" "./emlib/em_core.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_ldma.o" "./emlib/em_leuart.o" "./emlib/em_msc.o" "./emlib/em_prs.o" "./emlib/em_rmu.o" "./emlib/em_rtcc.o" "./emlib/em_se.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./emlib/em_usart.o" "./emlib/em_wdog.o" "./emlib/system_mgm12.o" "./external-generated-files/bootloader-interface-app.o" "./external-generated-files/bootloader-interface.o" "./external-generated-files/btl_interface.o" "./external-generated-files/btl_interface_storage.o" "./external-generated-files/sample-dotdot-certificates.o" "./fem-control/fem-control.o" "./hal-library/EFR32/assert-crash-handlers.o" "./hal-library/EFR32/crc.o" "./hal-library/EFR32/cstartup-common.o" "./hal-library/EFR32/endian.o" "./hal-library/EFR32/faults-v7m.o" "./hal-library/EFR32/hal-config-gpio.o" "./hal-library/EFR32/hal-config.o" "./hal-library/EFR32/isr-stubs.o" "./hal-library/EFR32/mem-util.o" "./hal-library/EFR32/mfg-token.o" "./hal-library/EFR32/micro-common.o" "./hal-library/EFR32/micro.o" "./hal-library/EFR32/mpu.o" "./hal-library/EFR32/random.o" "./hal-library/EFR32/sleep-efm32.o" "./hal-library/EFR32/token-def.o" "./hal-library/EFR32/token.o" "./heartbeat/heartbeat.o" "./icmp-cli/icmp-cli.o" "./icmp-debug/icmp-debug.o" "./idle-sleep/idle-sleep.o" "./led/EFR32/led.o" "./main/main.o" "./network-management-cli/network-management-cli.o" "./nvm3/EFR32/nvm3_default.o" "./nvm3/EFR32/nvm3_hal_flash.o" "./nvm3/nvm3_lock.o" "./ota-bootload-client/ota-bootload-client.o" "./ota-bootload-client-policy/ota-bootload-client-policy.o" "./ota-bootload-core/ota-bootload-core.o" "./ota-bootload-storage-cli/ota-bootload-storage-cli.o" "./ota-bootload-storage-eeprom/ota-bootload-storage-eeprom.o" "./scan-debug/scan-debug.o" "./serial/EFR32/ember-printf.o" "./serial/EFR32/serial.o" "./serial/com.o" "./slaac-client/slaac-client.o" "./stack/thread-stack/EFR32/SEGGER_RTT.o" "./stack/thread-stack/ember-ip-configuration.o" "./udp-cli/udp-cli.o" "./version-debug/version-debug.o" "./zcl-core/cbor-encoder.o" "./zcl-core/zcl-core-access.o" "./zcl-core/zcl-core-attribute.o" "./zcl-core/zcl-core-binding.o" "./zcl-core/zcl-core-cache.o" "./zcl-core/zcl-core-cli.o" "./zcl-core/zcl-core-command.o" "./zcl-core/zcl-core-dtls-manager.o" "./zcl-core/zcl-core-endpoint.o" "./zcl-core/zcl-core-ez-mode.o" "./zcl-core/zcl-core-group.o" "./zcl-core/zcl-core-message.o" "./zcl-core/zcl-core-notification.o" "./zcl-core/zcl-core-reporting.o" "./zcl-core/zcl-core-uri.o" "./zcl-core/zcl-core-well-known.o" "./zcl-core/zcl-core.o" "./zcl-core/zclip-struct.o" "./zcl-core-cli/zcl-core-cli-implementation.o" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//protocol/thread/build/thread-stack-cortexm3-gcc-mgm12-rail/thread-stack.a" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/radio/rail_lib/autogen/librail_release/librail_module_efr32xg12_gcc_release.a" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/radio/rail_lib/autogen/librail_release/librail_config_mgm12p32f1024ge_gcc.a" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//protocol/thread/build/mbedtls-library-cortexm3-gcc-mgm12-rail/mbedtls-library.a" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//protocol/thread/build/dhcp-library-cortexm3-gcc-mgm12-rail/dhcp-library.a" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//protocol/thread/build/sim-eeprom2-to-nvm3-upgrade-stub-library-cortexm3-gcc-mgm12-rail/sim-eeprom2-to-nvm3-upgrade-stub-library.a" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//protocol/thread/build/mfglib-library-cortexm3-gcc-mgm12-rail/mfglib-library.a" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//platform/emdrv/nvm3/lib/libnvm3_CM4_gcc.a" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5//protocol/thread/build/hal-library-cortexm3-gcc-mgm12-rail/hal-library.a" -lm -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: AQM-Firmware-V2.hex'
	arm-none-eabi-objcopy -O ihex "AQM-Firmware-V2.axf" "AQM-Firmware-V2.hex"
	@echo ' '

	@echo 'Building bin file: AQM-Firmware-V2.bin'
	arm-none-eabi-objcopy -O binary "AQM-Firmware-V2.axf" "AQM-Firmware-V2.bin"
	@echo ' '

	@echo 'Building s37 file: AQM-Firmware-V2.s37'
	arm-none-eabi-objcopy -O srec "AQM-Firmware-V2.axf" "AQM-Firmware-V2.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "AQM-Firmware-V2.axf" -A
	@echo ' '

	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) AQM-Firmware-V2.axf
	-@echo ' '

pre-build:
	-cmd //c 'C:\Users\AIDANF~1\SIMPLI~1\V4_WOR~1\AQM-OT~1.2/AQM-OTA-V3.0.2-new-prebuild.bat' 'AQM-Firmware-V2' 'C:/Users/ShravanthiNadukuda/SimplicityStudio/v4_workspace/AQM-Firmware-V2' '' 'C:\SiliconLabs\SimplicityStudio\v4'
	-@echo ' '

post-build:
	-cmd //c 'C:\Users\AIDANF~1\SIMPLI~1\V4_WOR~1\AQM-OT~1.2/AQM-OTA-V3.0.2-new-postbuild.bat' 'AQM-Firmware-V2' 'C:/Users/ShravanthiNadukuda/SimplicityStudio/v4_workspace/AQM-Firmware-V2' '' 'C:\SiliconLabs\SimplicityStudio\v4'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
