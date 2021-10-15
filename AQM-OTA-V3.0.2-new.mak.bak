# Architecture directories
ARCHITECTURE_DIR = efr32
BUILD_DIR = build
OUTPUT_DIR = $(BUILD_DIR)/$(ARCHITECTURE_DIR)
LST_DIR = lst
PROJECTNAME = AQM-OTA-V3.0.2-new

# Stack and submodule directories
GLOBAL_BASE_DIR     = ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/..

SOURCE_FILES = \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/dmadrv/src/dmadrv.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/gpiointerrupt/src/gpiointerrupt.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/rtcdrv/src/rtcdriver.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/sleep/src/sleep.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/tempdrv/src/tempdrv.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/ustimer/src/ustimer.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_adc.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_cmu.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_core.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_emu.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_gpio.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_i2c.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_ldma.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_leuart.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_msc.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_prs.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_rmu.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_rtcc.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_se.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_system.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_timer.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_usart.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/src/em_wdog.c \
../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../Device/SiliconLabs/MGM12/Source/system_mgm12.c \
  client-implementation.c \
  thread-bookkeeping.c \
  thread-callback-stubs.c \
  thread-callbacks.c \
  thread-cli.c \
  thread-coap-dispatch.c \
  thread-events.c \
  thread-zclip.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/assert-crash-handlers.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/bootloader-interface-app.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/bootloader-interface.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/button.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/cstartup-common.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/diagnostic.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/faults-v7m.s79 \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/hal-config-gpio.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/hal-config.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/isr-stubs.s79 \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/led.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/mfg-token.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/micro-common.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/micro.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/mpu.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/sleep-efm32.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/token.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/generic/crc.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/generic/endian.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/generic/mem-util.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/generic/random.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/generic/token-def.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/plugin/adc/adc-efr32.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/plugin/antenna-stub/antenna-stub.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/plugin/buzzer/buzzer-efr32.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/plugin/debug-jtag/debug-jtag-efr32.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/plugin/debug/rtt/SEGGER_RTT.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/plugin/serial/cortexm/efm32/com.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/plugin/serial/ember-printf.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/plugin/serial/serial.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/bootloader/api/btl_interface.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/bootloader/api/btl_interface_storage.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/emdrv/nvm3/src/nvm3_default.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/emdrv/nvm3/src/nvm3_hal_flash.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/emdrv/nvm3/src/nvm3_lock.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/emdrv/uartdrv/src/uartdrv.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/plugin/coexistence/common/coexistence.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/plugin/coexistence/hal/efr32/coexistence-directional-priority.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/plugin/coexistence/hal/efr32/coexistence-hal.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/plugin/coexistence/protocol/ieee802154/coexistence-802154.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/address-configuration-debug/address-configuration-debug.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/cli/cli.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/coap-cli/coap-cli.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/coap-dispatch/coap-dispatch.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/debug-print/debug-print.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/dhcp-client/dhcp-client.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/dtls-auth-params/dtls-auth-params.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/dtls-auth-params/sample-dotdot-certificates.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/heartbeat/heartbeat.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/icmp-cli/icmp-cli.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/icmp-debug/icmp-debug.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/idle-sleep/idle-sleep.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/main/main.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/network-management-cli/network-management-cli.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/scan-debug/scan-debug.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/slaac-client/slaac-client.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/udp-cli/udp-cli.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/version-debug/version-debug.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/ota-bootload-client-policy/ota-bootload-client-policy.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/ota-bootload-client/ota-bootload-client.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/ota-bootload-core/ota-bootload-core.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/ota-bootload-storage-cli/ota-bootload-storage-cli.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/ota-bootload-storage-eeprom/ota-bootload-storage-eeprom.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core-cli/zcl-core-cli-implementation.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/cbor-encoder.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-access.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-attribute.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-binding.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-cache.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-cli.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-command.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-dtls-manager.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-endpoint.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-ez-mode.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-group.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-message.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-notification.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-reporting.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-uri.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core-well-known.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zcl-core.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/plugin/zcl/zcl-core/zclip-struct.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/util/serial/command-interpreter2-error.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/util/serial/command-interpreter2-util.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/util/serial/command-interpreter2.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/stack/config/ember-ip-configuration.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/util/plugin/plugin-afv6/eeprom/eeprom-print.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/util/plugin/plugin-common/eeprom/eeprom-cli.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/util/plugin/plugin-common/eeprom/eeprom.c \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/util/plugin/plugin-common/fem-control/fem-control.c \


LIB_FILES = \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/emdrv/nvm3/lib/libnvm3_CM4_gcc.a \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/autogen/librail_release/librail_config_mgm12p32f1024ge_gcc.a \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/autogen/librail_release/librail_module_efr32xg12_gcc_release.a \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/build/dhcp-library-cortexm3-gcc-mgm12-rail/dhcp-library.a \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/build/hal-library-cortexm3-gcc-mgm12-rail/hal-library.a \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/build/mbedtls-library-cortexm3-gcc-mgm12-rail/mbedtls-library.a \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/build/mfglib-library-cortexm3-gcc-mgm12-rail/mfglib-library.a \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/build/sim-eeprom2-to-nvm3-upgrade-stub-library-cortexm3-gcc-mgm12-rail/sim-eeprom2-to-nvm3-upgrade-stub-library.a \
  ../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/build/thread-stack-cortexm3-gcc-mgm12-rail/thread-stack.a \


CDEFS = -DAPP_GECKO_INFO_PAGE_BTL \
-DMGM12P32F1024GE \
-DMGM12 \
-DCORTEXM3 \
-DCORTEXM3_EFR32 \
-DCORTEXM3_EFR32_MICRO \
-DCORTEXM3_EFM32_MICRO \
-DEFR32_SERIES1_CONFIG2_MICRO \
-DCONFIGURATION_HEADER=\"thread-configuration.h\" \
-DPLATFORM_HEADER=\"../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/compiler/gcc.h\" \
  -DHAL_CONFIG=1 \
  -DEMBER_AF_USE_HWCONF \
  -DEMBER_HEAP_SIZE=8000 \
  -DEMBER_AF_API_EMBER_TYPES=\"stack/include/ember-types.h\" \
  -DEMBER_STACK_IP \
  -DUSE_NVM3 \
  -DNVM3_DEFAULT_NVM_SIZE=NVM3_FLASH_PAGES*FLASH_PAGE_SIZE \
  -DMFGLIB \
  -DHAVE_TLS_JPAKE \
  -DHAVE_TLS_PSK \
  -DHAVE_TLS_ECDHE_ECDSA \
  -DDEBUG_LEVEL=FULL_DEBUG \
  -DEMLIB_USER_CONFIG \
  -DAPPLICATION_TOKEN_HEADER=\"thread-token.h\" \
  -DAPPLICATION_MFG_TOKEN_HEADER=\"thread-mfg-token.h\" \
  -DNVM3_FLASH_PAGES=18 \
  -DNVM3_DEFAULT_CACHE_SIZE=100 \
  -DNVM3_DEFAULT_MAX_OBJECT_SIZE=254 \
  -DNVM3_DEFAULT_REPACK_HEADROOM=0 \
  -DPHY_RAIL=1 \


ASMDEFS = -DAPP_GECKO_INFO_PAGE_BTL \
-DMGM12P32F1024GE \
-DMGM12 \
-DCORTEXM3 \
-DCORTEXM3_EFR32 \
-DCORTEXM3_EFR32_MICRO \
-DCORTEXM3_EFM32_MICRO \
-DEFR32_SERIES1_CONFIG2_MICRO \
-DCONFIGURATION_HEADER=\"thread-configuration.h\" \
-DPLATFORM_HEADER=\"../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/compiler/gcc.h\" \
  -DHAL_CONFIG=1 \
  -DEMBER_AF_USE_HWCONF \
  -DEMBER_HEAP_SIZE=8000 \
  -DEMBER_AF_API_EMBER_TYPES=\"stack/include/ember-types.h\" \
  -DEMBER_STACK_IP \
  -DUSE_NVM3 \
  -DNVM3_DEFAULT_NVM_SIZE=NVM3_FLASH_PAGES*FLASH_PAGE_SIZE \
  -DMFGLIB \
  -DHAVE_TLS_JPAKE \
  -DHAVE_TLS_PSK \
  -DHAVE_TLS_ECDHE_ECDSA \
  -DDEBUG_LEVEL=FULL_DEBUG \
  -DEMLIB_USER_CONFIG \
  -DAPPLICATION_TOKEN_HEADER=\"thread-token.h\" \
  -DAPPLICATION_MFG_TOKEN_HEADER=\"thread-mfg-token.h\" \
  -DNVM3_FLASH_PAGES=18 \
  -DNVM3_DEFAULT_CACHE_SIZE=100 \
  -DNVM3_DEFAULT_MAX_OBJECT_SIZE=254 \
  -DNVM3_DEFAULT_REPACK_HEADROOM=0 \
  -DPHY_RAIL=1 \


CINC = -I./ \
-I$(ARM_IAR7_DIR)/ARM/INC \
-I..\..\SimplicityStudio_v4\developer\sdks\gecko_sdk_suite\v2.5 \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/../.. \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/../../stack \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/protocol/thread/app/thread/../util \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/plugin \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/.. \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32 \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/config \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/efr32 \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../CMSIS/Include \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../Device/SiliconLabs-Restricted/mgm12/Include \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../Device/SiliconLabs/mgm12/Include \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/common/inc \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/config \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/dmadrv/inc \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/gpiointerrupt/inc \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/rtcdrv/inc \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/sleep/inc \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/spidrv/inc \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/tempdrv/inc \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/uartdrv/inc \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emdrv/ustimer/inc \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../emlib/inc \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../middleware/glib \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../middleware/glib/glib \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../radio/rail_lib/plugin \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/../../halconfig/inc/hal-config \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5 \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/halconfig/inc/hal-config \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/hardware/module/config \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/hardware/kit/common/halconfig \
-I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/hardware/kit/common/bsp \
  -Ihal-config \
  -I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/bootloader \
  -I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/emdrv/nvm3/inc \
  -I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/chip/efr32/efr32xg1x \
  -I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/common \
  -I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/plugin/coexistence/common \
  -I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/plugin/coexistence/hal/efr32 \
  -I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/plugin/coexistence/protocol/ieee802154 \
  -I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/protocol/ble \
  -I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/protocol/ieee802154 \
  -I../../SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/radio/rail_lib/protocol/zwave \


TARGET = AQM-OTA-V3.0.2-new

CSOURCES = $(filter %.c, $(SOURCE_FILES))
ASMSOURCES = $(filter %.s79, $(SOURCE_FILES))
ASMSOURCES2 = $(filter %.s, $(SOURCE_FILES))
ASMSOURCES3 = $(filter %.S, $(SOURCE_FILES))

COBJS = $(addprefix $(OUTPUT_DIR)/,$(CSOURCES:.c=.o))
ASMOBJS = $(addprefix $(OUTPUT_DIR)/,$(ASMSOURCES:.s79=.o))
ASMOBJS2 = $(addprefix $(OUTPUT_DIR)/,$(ASMSOURCES2:.s=.o))
ASMOBJS3 = $(addprefix $(OUTPUT_DIR)/,$(ASMSOURCES3:.S=.o))

OUTPUT_DIRS = $(sort $(dir $(COBJS)) $(dir $(ASMOBJS)) $(dir $(ASMOBJS2)) $(dir $(ASMOBJS3)))

ARCHITECTUREID = efr32~family[m]~series[1]~device_configuration[2]~performance[p]~radio[432]~flash[1024k]~temp[g]~package[m]~pins[48]~module[mgm12p32f1024ge]+none+gcc

# GNU ARM compiler
ifeq ($(findstring +gcc,$(ARCHITECTUREID)), +gcc)
$(info GCC Build)
	# Add linker circular reference as the order of objects may matter for any libraries used
	GROUP_START =-Wl,--start-group
	GROUP_END =-Wl,--end-group

	CCFLAGS = -g3 \
    -gdwarf-2 \
    -mcpu=cortex-m4 \
    -mthumb \
    -std=gnu99 \
    -Os  \
    -Wall  \
    -c  \
    -fmessage-length=0  \
    -ffunction-sections  \
    -fdata-sections  \
    -mfpu=fpv4-sp-d16  \
    -mfloat-abi=softfp \
	$(CDEFS) \
	$(CINC) \

	ASMFLAGS = -c \
	-g3 \
	-gdwarf-2 \
	-mcpu=cortex-m4 \
	-mthumb \
	-c \
	-x assembler-with-cpp \
	$(CINC) \
	$(ASMDEFS)

	LDFLAGS = -g3 \
	-gdwarf-2 \
	-mcpu=cortex-m4 \
	-mthumb -T "/home/bryce/Desktop/SiLabs/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/gcc-cfg.ld" \
	-L"$(GLOBAL_BASE_DIR)/hal/micro/cortexm3/" \
	-Xlinker --defsym="SIMEEPROM_SIZE=36864" \
	-Xlinker --defsym="LOCKBITS_IN_MAINFLASH_SIZE=0" \
	-Xlinker --defsym="FLASH_SIZE=1048576" \
	-Xlinker --defsym="RAM_SIZE=262144" \
	-Xlinker --defsym="APP_GECKO_INFO_PAGE_BTL=1" \
	-Xlinker --defsym="EMBER_MALLOC_HEAP_SIZE=0" \
	-Xlinker --gc-sections \
	-Xlinker -Map="$(PROJECTNAME).map" \
	-mfpu=fpv4-sp-d16 \
	-mfloat-abi=softfp --specs=nano.specs -u _printf_float \
	-Wl,--start-group -lgcc -lc -lnosys   -Wl,--end-group

	ARCHFLAGS = r

	ELFTOOLFLAGS_BIN = -O binary
	ELFTOOLFLAGS_HEX = -O ihex
	ELFTOOLFLAGS_S37 = -O srec

	ifeq ($(OS),Windows_NT)
		ARCH = $(ARM_GNU_DIR)/bin/arm-none-eabi-gcc-ar.exe
		AS = $(ARM_GNU_DIR)/bin/arm-none-eabi-gcc.exe
		CC = $(ARM_GNU_DIR)/bin/arm-none-eabi-gcc.exe
		ELFTOOL = $(ARM_GNU_DIR)/bin/arm-none-eabi-objcopy.exe
		LD = $(ARM_GNU_DIR)/bin/arm-none-eabi-gcc.exe
	else
		ARCH = $(ARM_GNU_DIR)/bin/arm-none-eabi-gcc-ar
		AS = $(ARM_GNU_DIR)/bin/arm-none-eabi-gcc
		CC = $(ARM_GNU_DIR)/bin/arm-none-eabi-gcc
		ELFTOOL = $(ARM_GNU_DIR)/bin/arm-none-eabi-objcopy
		LD = $(ARM_GNU_DIR)/bin/arm-none-eabi-gcc
	endif

endif

# IAR 7.xx toolchain
ifeq ($(findstring +iar,$(ARCHITECTUREID)), +iar)
$(info IAR Build)

	# IAR is not sensitive to linker lib order thus no groups needed.
	GROUP_START =
	GROUP_END =
	CINC += -I$(ARM_IAR6_DIR)/ARM/INC

	ifndef ARM_IAR7_DIR
	$(error ARM_IAR7_DIR is not set. Please define ARM_IAR7_DIR pointing to your IAR 7.xx installation folder.)
	endif

	CCFLAGS = --cpu=cortex-m3 \
	--cpu_mode=thumb \
	--diag_suppress=Pa050 \
	-e \
	--endian=little \
	--fpu=none \
	--no_path_in_file_macros \
	--separate_cluster_for_initialized_variables \
	--dlib_config=$(ARM_IAR7_DIR)/ARM/inc/c/DLib_Config_Normal.h \
	--debug \
	--dependencies=m $*.d \
	-Ohz \
	$(CDEFS) \
	$(CINC)

	ASMFLAGS = --cpu cortex-M3 \
	--fpu None \
	-s+ \
	"-M<>" \
	-w+ \
	-t2 \
	-r \
	$(CINC) \
	$(ASMDEFS)

	LDFLAGS = --entry __iar_program_start \
	--diag_suppress=Lp012 \
	--config /home/bryce/Desktop/SiLabs/SimplicityStudio_v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/micro/cortexm3/efm32/iar-cfg.icf \
	--config_def APP_GECKO_INFO_PAGE_BTL=1 \
	--config_def EMBER_MALLOC_HEAP_SIZE=0

	ARCH = $(JAMEXE_PREFIX) $(ARM_IAR7_DIR)/arm/bin/iarchive.exe
	AS = $(JAMEXE_PREFIX) $(ARM_IAR7_DIR)/arm/bin/iasmarm.exe
	CC = $(JAMEXE_PREFIX) $(ARM_IAR7_DIR)/arm/bin/iccarm.exe
	ELFTOOL = $(JAMEXE_PREFIX) $(ARM_IAR7_DIR)/arm/bin/ielftool.exe
	LD = $(JAMEXE_PREFIX) $(ARM_IAR7_DIR)/arm/bin/ilinkarm.exe

	# No archiver arguments needed for IAR.
	ARCHFLAGS =

	ELFTOOLFLAGS_BIN = --bin
	ELFTOOLFLAGS_HEX = --ihex
	ELFTOOLFLAGS_S37 = --srec --srec-s3only

endif

.PHONY: all clean PROLOGUE

all: PROLOGUE $(OUTPUT_DIRS) $(COBJS) $(ASMOBJS) $(ASMOBJS2) $(ASMOBJS3) $(LIB_FILES)
	@echo 'Linking...'
	@$(LD) $(GROUP_START) $(LDFLAGS) $(COBJS) $(ASMOBJS) $(ASMOBJS2) $(ASMOBJS3) $(LIB_FILES) $(GROUP_END) -o $(OUTPUT_DIR)/$(TARGET).out
	@$(ELFTOOL) $(OUTPUT_DIR)/$(TARGET).out $(ELFTOOLFLAGS_BIN) $(OUTPUT_DIR)/$(TARGET).bin
	@$(ELFTOOL) $(OUTPUT_DIR)/$(TARGET).out $(ELFTOOLFLAGS_HEX) $(OUTPUT_DIR)/$(TARGET).hex
	@$(ELFTOOL) $(OUTPUT_DIR)/$(TARGET).out $(ELFTOOLFLAGS_S37) $(OUTPUT_DIR)/$(TARGET).s37
	@echo 'Done.'

PROLOGUE:
#	@echo $(COBJS)
#	@echo $(ASMOBJS)
#	@echo $(ASMOBJS2)

$(OUTPUT_DIRS):
	@mkdir -p $@

$(COBJS): %.o:
	@echo 'Building $(notdir $(@:%.o=%.c))...'
	@$(CC) $(CCFLAGS) -o $@ $(filter %$(@:$(OUTPUT_DIR)/%.o=%.c),$(CSOURCES)) > /dev/null \

$(ASMOBJS): %.o:
	@echo 'Building $(notdir $(@:%.o=%.s79))...'
	@$(AS) $(ASMFLAGS) -o $@ $(filter %$(@:$(OUTPUT_DIR)/%.o=%.s79),$(ASMSOURCES)) > /dev/null

$(ASMOBJS2): %.o:
	@echo 'Building $(notdir $(@:%.o=%.s))...'
	@$(AS) $(ASMFLAGS) -o $@ $(filter %$(@:$(OUTPUT_DIR)/%.o=%.s),$(ASMSOURCES2)) > /dev/null

$(ASMOBJS3): %.o:
	@echo 'Building $(notdir $(@:%.o=%.S))...'
	@$(AS) $(ASMFLAGS) -o $@ $(filter %$(@:$(OUTPUT_DIR)/%.o=%.S),$(ASMSOURCES3)) > /dev/null

clean:
	$(RM) -r $(COBJS)
	$(RM) -r $(ASMOBJS)
	$(RM) -r $(ASMOBJS2)
	$(RM) -r $(OUTPUT_DIR)
