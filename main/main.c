/***************************************************************************//**
 * @file
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#include EMBER_AF_API_EVENT_QUEUE
#include EMBER_AF_API_HAL
#include "thread-bookkeeping.h"

#ifdef HAL_CONFIG
  #include "hal-config.h"
  #include "ember-hal-config.h"
#endif

// If serial functionality is enabled, we will initialize the serial ports during
// startup.  This has to happen after the HAL is initialized.
#ifdef EMBER_AF_API_SERIAL
  #include EMBER_AF_API_SERIAL
  #define SERIAL_INIT EMBER_AF_SERIAL_PORT_INIT
#else
  #define SERIAL_INIT()
#endif

// If we are running in simulation, we need to setup our "fake" EEPROM, which
// is just simulated EEPROM. This has to happen right after the HAL is
// initialized, but before the stack or app is initialized so that the EEPROM
// will be usable immediately. Calling this setupFakeEeprom() API will properly
// initialize the HalEepromInformationType data for the "fake" EEPROM. The
// partSize and addressBase are multiple of 2 that match some of the EEPROM
// implementations in base. We set pageEraseRequired to true in order to plan
// for a worst case scenario. We set the wordSizeBytes to 2 because this is the
// case for most EEPROM implementations in base.
#ifdef EMBER_TEST
  #include "micro/unix/simulation/fake-eeprom.h"
  #define EEPROM_INIT()                              \
  setupFakeEeprom(204800,   /* partSize */           \
                  0,        /* addressBase */        \
                  FAKE_EEPROM_DEFAULT_PAGE_SIZE,     \
                  FAKE_EEPROM_DEFAULT_PAGE_ERASE_MS, \
                  true,     /* pageEraseRequired? */ \
                  2);       /* wordSizeBytes */
#else
  #define EEPROM_INIT()
#endif

// If printing is enabled, we will print some diagnostic information about the
// most recent reset and also during runtime.  On some platforms, extended
// diagnostic information is available.
#if defined(EMBER_AF_API_SERIAL) && defined(EMBER_AF_PRINT_ENABLE)
  #ifdef EMBER_AF_API_DIAGNOSTIC_CORTEXM3
    #include EMBER_AF_API_DIAGNOSTIC_CORTEXM3
  #endif
static void printResetInformation(void);
  #define PRINT_RESET_INFORMATION printResetInformation
  #define emberAfGuaranteedPrint(...) \
  emberSerialGuaranteedPrintf(APP_SERIAL, __VA_ARGS__)
  #define emberAfGuaranteedPrintln(format, ...) \
  emberSerialGuaranteedPrintf(APP_SERIAL, format "\r\n", __VA_ARGS__);
#else
  #define PRINT_RESET_INFORMATION()
  #define emberAfGuaranteedPrint(...)
  #define emberAfGuaranteedPrintln(...)
#endif

#ifdef EMBER_AF_API_MANAGEMENT_CLIENT
  #include EMBER_AF_API_MANAGEMENT_CLIENT
  #define PROCESS_MANAGEMENT_COMMAND managementCommandTick
#else
  #define PROCESS_MANAGEMENT_COMMAND()
#endif

#ifdef EMBER_AF_API_MICRIUM_RTOS
  #include EMBER_AF_API_MICRIUM_RTOS
#endif

// Our entry point is typically main(), except during testing.
#ifdef EMBER_TEST
  #define MAIN nodeMain
#else
  #define MAIN main
#endif

void emAfPluginAppFrameworkMainLoop(void *p_arg);

extern const EmberEventData emAppEvents[];
EmberTaskId emAppTask;
EventQueue emAppEventQueue;

static bool stackIsInitialized;
static bool applicationIsInitialized;
static uint32_t initTimeMs;
#define INIT_TIMEOUT_MS (10 * MILLISECOND_TICKS_PER_SECOND)

int MAIN(MAIN_FUNCTION_PARAMETERS)
{
  // Let the application and plugins do early initialization.  This function is
  // generated.
  emAfMain(MAIN_FUNCTION_ARGUMENTS);

  halInit();

#ifdef EMBER_AF_PLUGIN_MICRIUM_RTOS
  emberAfPluginMicriumRtosCpuInit();
#endif

  INTERRUPTS_ON();

  EEPROM_INIT();

  SERIAL_INIT();

  PRINT_RESET_INFORMATION();

  // Run the application loop, which usually never terminates.
#ifdef EMBER_AF_PLUGIN_MICRIUM_RTOS
  emberAfPluginMicriumRtosInitAndRunTasks();
#else
  emAfPluginAppFrameworkMainLoop(NULL);
#endif

  return 0;
}

void emAfPluginAppFrameworkMainLoop(void *p_arg)
{
  // Initialize a task for the application and plugin events and enable idling.
  emAppTask = emberTaskInit(emAppEvents);
  emberTaskEnableIdling(true);
  emInitializeEventQueue(&emAppEventQueue);

  // Initialize the stack.
  stackIsInitialized = false;
  applicationIsInitialized = false;
  initTimeMs = halCommonGetInt32uMillisecondTick();



  /*
    *  Initialises fan controller to stop fan spooling up on startup
    *  to counteract the fan controller default of full on at startup.
    */
  fanInitialise();
  emberInit();

  while (true) {
    // Reset the watchdog timer to prevent a timeout.
    halResetWatchdog();

    // Process management commands from the NCP, if applicable.  This is done
    // before checking whether stack initialization has finished, because the
    // initialization status is itself a management command.
    PROCESS_MANAGEMENT_COMMAND();

    // Wait until the stack is initialized before allowing the application and
    // plugins to run, so that so that the application doesn't get ahead of the
    // stack.  If initialization takes too long, we try again.
    if (!stackIsInitialized) {
      uint32_t nowMs = halCommonGetInt32uMillisecondTick();
      if (INIT_TIMEOUT_MS <= elapsedTimeInt32u(initTimeMs, nowMs)) {
        emberAfGuaranteedPrintln("Waiting for NCP...", 0);
        initTimeMs = nowMs;
        emberInit();
      }
      continue;
    }

    // Initialize the application and plugins.  Whenever the stack initializes,
    // these must be reinitialized.  This function is generated.
    if (stackIsInitialized && !applicationIsInitialized) {
      emAfInit();
      applicationIsInitialized = true;
    }

    // Let the stack, application, and plugins run periodic tasks.  This
    // function is generated.
    emAfTick();

    // Run the application and plugin events.
    emberRunTask(emAppTask);
    emberRunEventQueue(&emAppEventQueue);

    simulatedTimePassesMs(emberMsToNextEvent(emAppEvents,
                                             emberMsToNextQueueEvent(&emAppEventQueue)));
  }
}

#ifdef EMBER_AF_PRINT_ENABLE

static void printResetInformation(void)
{
  // Information about the most recent reset is printed during startup to aid
  // in debugging.
  emberAfGuaranteedPrintln("Reset info: 0x%x (%p)",
                           halGetResetInfo(),
                           halGetResetString());
#ifdef EMBER_AF_API_DIAGNOSTIC_CORTEXM3
  emberAfGuaranteedPrintln("Extended reset info: 0x%2x (%p)",
                           halGetExtendedResetInfo(),
                           halGetExtendedResetString());
  if (halResetWasCrash()) {
    halPrintCrashSummary(APP_SERIAL);
    halPrintCrashDetails(APP_SERIAL);
    halPrintCrashData(APP_SERIAL);
  }
#endif // EMBER_AF_API_DIAGNOSTIC_CORTEXM3
}

#endif // EMBER_AF_PRINT_ENABLE

void emberInitReturn(EmberStatus status)
{
  // If initialization fails, we have to assert because something is wrong.
  // Whenever the stack initializes, the application and plugins must be
  // reinitialized.
  emberAfGuaranteedPrintln("Init: 0x%x", status);
  assert(status == EMBER_SUCCESS);
  stackIsInitialized = true;
  applicationIsInitialized = false;
}

// TODO: This should not be necessary in the application.  See EMIPSTACK-324.
bool ipModemLinkPreparingForPowerDown(void)
{
  return false;
}

void emberResetMicroHandler(EmberResetCause resetCause)
{
  // We only print the reset cause for host applications because SoC
  // applications will automatically print the reset cause at startup.
#ifdef EMBER_HOST
  static const char * const resetCauses[] = {
    "UNKNOWN",
    "FIB",
    "BOOTLOADER",
    "EXTERNAL",
    "POWER ON",
    "WATCHDOG",
    "SOFTWARE",
    "CRASH",
    "FLASH",
    "FATAL",
    "FAULT",
    "BROWNOUT",
  };
  emberAfGuaranteedPrintln("Reset info: 0x%x (%p)",
                           resetCause,
                           (resetCause < COUNTOF(resetCauses)
                            ? resetCauses[resetCause]
                            : resetCauses[EMBER_RESET_UNKNOWN]));
#endif // EMBER_HOST
}

void emberMarkApplicationBuffersHandler(void)
{
  // Mark scheduled events in the queue and run the marking functions of those
  // events.
  emberMarkEventQueue(&emAppEventQueue);

  // Let the application and plugins mark their buffers.  This function is
  // generated.
  emAfMarkApplicationBuffers();
}
