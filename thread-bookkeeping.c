// This file is generated by Simplicity Studio.  Please do not edit manually.
//
//

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#include EMBER_AF_API_ZCL_CORE

// OkToSleep function declarations.
bool emberAfPluginIdleSleepOkToSleepCallback(uint32_t durationMs);  // Plugin: idle-sleep

bool emberAfOkToSleep(uint32_t durationMs)
{
  return emberAfPluginIdleSleepOkToSleepCallback(durationMs);  // Plugin: idle-sleep
}

// WakeUp function declarations.
void emberAfPluginIdleSleepWakeUpCallback(uint32_t durationMs);  // Plugin: idle-sleep

void emberAfPluginIdleSleepWakeUp(uint32_t durationMs)
{
  emberAfPluginIdleSleepWakeUpCallback(durationMs);  // Plugin: idle-sleep
}

// OkToIdle function declarations.
bool emberAfPluginIdleSleepOkToIdleCallback(uint32_t durationMs);  // Plugin: idle-sleep

bool emberAfOkToIdle(uint32_t durationMs)
{
  return emberAfPluginIdleSleepOkToIdleCallback(durationMs);  // Plugin: idle-sleep
}

// Active function declarations.
void emberAfPluginIdleSleepActiveCallback(uint32_t durationMs);  // Plugin: idle-sleep

void emberAfPluginIdleSleepActive(uint32_t durationMs)
{
  emberAfPluginIdleSleepActiveCallback(durationMs);  // Plugin: idle-sleep
}

// Main function declarations.
void emberAfMainCallback(MAIN_FUNCTION_PARAMETERS);  // Plugin: main

void emAfMain(MAIN_FUNCTION_PARAMETERS)
{
  emberAfMainCallback(MAIN_FUNCTION_ARGUMENTS);  // Plugin: main
}

// Init function declarations.
void emberCommandReaderInit(void);  // Plugin: command-interpreter2
void emberAfPluginEepromInitCallback(void);  // Plugin: eeprom
void emberAfInitCallback(void);  // Plugin: main
void emZclOtaBootloadClientInitCallback(void);  // Plugin: ota-bootload-client
void emZclOtaBootloadStorageEepromInitCallback(void);  // Plugin: ota-bootload-storage-eeprom
void emZclInitHandler(void);  // Plugin: zcl-core

void emAfInit(void)
{
  emberCommandReaderInit();  // Plugin: command-interpreter2
  emberAfPluginEepromInitCallback();  // Plugin: eeprom
  emberAfInitCallback();  // Plugin: main
  emZclOtaBootloadClientInitCallback();  // Plugin: ota-bootload-client
  emZclOtaBootloadStorageEepromInitCallback();  // Plugin: ota-bootload-storage-eeprom
  emZclInitHandler();  // Plugin: zcl-core
}

// Tick function declarations.
void emberAfPluginCliTickCallback(void);  // Plugin: cli
void emberAfPluginHeartbeatTickCallback(void);  // Plugin: heartbeat
void emberAfPluginIdleSleepTickCallback(void);  // Plugin: idle-sleep
void emberTick(void);  // Plugin: main
void emberAfTickCallback(void);  // Plugin: main
void emberSerialBufferTick(void);  // Plugin: serial

void emAfTick(void)
{
  emberAfPluginCliTickCallback();  // Plugin: cli
  emberAfPluginHeartbeatTickCallback();  // Plugin: heartbeat
  emberAfPluginIdleSleepTickCallback();  // Plugin: idle-sleep
  emberTick();  // Plugin: main
  emberAfTickCallback();  // Plugin: main
  emberSerialBufferTick();  // Plugin: serial
}

// MarkApplicationBuffers function declarations.
void emberAfMarkApplicationBuffersCallback(void);  // Plugin: main
void emZclAccessMarkApplicationBuffersHandler(void);  // Plugin: zcl-core
void emZclCacheMarkApplicationBuffersHandler(void);  // Plugin: zcl-core
void emZclReportingMarkApplicationBuffersHandler(void);  // Plugin: zcl-core
void emZclDtlsManagerMarkBuffers(void);  // Plugin: zcl-core

void emAfMarkApplicationBuffers(void)
{
  emberAfMarkApplicationBuffersCallback();  // Plugin: main
  emZclAccessMarkApplicationBuffersHandler();  // Plugin: zcl-core
  emZclCacheMarkApplicationBuffersHandler();  // Plugin: zcl-core
  emZclReportingMarkApplicationBuffersHandler();  // Plugin: zcl-core
  emZclDtlsManagerMarkBuffers();  // Plugin: zcl-core
}

// NetworkStatus function declarations.
void emberAfNetworkStatusCallback(EmberNetworkStatus newNetworkStatus, EmberNetworkStatus oldNetworkStatus, EmberJoinFailureReason reason);  // Plugin: main
void emZclOtaBootloadClientNetworkStatusCallback(EmberNetworkStatus newNetworkStatus, EmberNetworkStatus oldNetworkStatus, EmberJoinFailureReason reason);  // Plugin: ota-bootload-client
void emZclBindingNetworkStatusHandler(EmberNetworkStatus newNetworkStatus, EmberNetworkStatus oldNetworkStatus, EmberJoinFailureReason reason);  // Plugin: zcl-core
void emZclCacheNetworkStatusHandler(EmberNetworkStatus newNetworkStatus, EmberNetworkStatus oldNetworkStatus, EmberJoinFailureReason reason);  // Plugin: zcl-core
void emZclGroupNetworkStatusHandler(EmberNetworkStatus newNetworkStatus, EmberNetworkStatus oldNetworkStatus, EmberJoinFailureReason reason);  // Plugin: zcl-core
void emZclReportingNetworkStatusHandler(EmberNetworkStatus newNetworkStatus, EmberNetworkStatus oldNetworkStatus, EmberJoinFailureReason reason);  // Plugin: zcl-core

void emberNetworkStatusHandler(EmberNetworkStatus newNetworkStatus, EmberNetworkStatus oldNetworkStatus, EmberJoinFailureReason reason)
{
  emberAfNetworkStatusCallback(newNetworkStatus, oldNetworkStatus, reason);  // Plugin: main
  emZclOtaBootloadClientNetworkStatusCallback(newNetworkStatus, oldNetworkStatus, reason);  // Plugin: ota-bootload-client
  emZclBindingNetworkStatusHandler(newNetworkStatus, oldNetworkStatus, reason);  // Plugin: zcl-core
  emZclCacheNetworkStatusHandler(newNetworkStatus, oldNetworkStatus, reason);  // Plugin: zcl-core
  emZclGroupNetworkStatusHandler(newNetworkStatus, oldNetworkStatus, reason);  // Plugin: zcl-core
  emZclReportingNetworkStatusHandler(newNetworkStatus, oldNetworkStatus, reason);  // Plugin: zcl-core
}

// PreAttributeChange function declarations.
bool emberZclPreAttributeChangeCallback(EmberZclEndpointId_t endpointId, const EmberZclClusterSpec_t *clusterSpec, EmberZclAttributeId_t attributeId, const void *buffer, size_t bufferLength);  // Plugin: zcl-core

bool emZclPreAttributeChange(EmberZclEndpointId_t endpointId, const EmberZclClusterSpec_t *clusterSpec, EmberZclAttributeId_t attributeId, const void *buffer, size_t bufferLength)
{
  return emberZclPreAttributeChangeCallback(endpointId, clusterSpec, attributeId, buffer, bufferLength);  // Plugin: zcl-core
}

// PostAttributeChange function declarations.
void emberZclPostAttributeChangeCallback(EmberZclEndpointId_t endpointId, const EmberZclClusterSpec_t *clusterSpec, EmberZclAttributeId_t attributeId, const void *buffer, size_t bufferLength);  // Plugin: zcl-core
void emZclReportingPostAttributeChangeHandler(EmberZclEndpointId_t endpointId, const EmberZclClusterSpec_t *clusterSpec, EmberZclAttributeId_t attributeId, const void *buffer, size_t bufferLength);  // Plugin: zcl-core

void emZclPostAttributeChange(EmberZclEndpointId_t endpointId, const EmberZclClusterSpec_t *clusterSpec, EmberZclAttributeId_t attributeId, const void *buffer, size_t bufferLength)
{
  emberZclPostAttributeChangeCallback(endpointId, clusterSpec, attributeId, buffer, bufferLength);  // Plugin: zcl-core
  emZclReportingPostAttributeChangeHandler(endpointId, clusterSpec, attributeId, buffer, bufferLength);  // Plugin: zcl-core
}

// Notification function declarations.
void emberZclNotificationCallback(const EmberZclNotificationContext_t *context, const EmberZclClusterSpec_t *clusterSpec, EmberZclAttributeId_t attributeId, const void *buffer, size_t bufferLength);  // Plugin: zcl-core

void emZclNotification(const EmberZclNotificationContext_t *context, const EmberZclClusterSpec_t *clusterSpec, EmberZclAttributeId_t attributeId, const void *buffer, size_t bufferLength)
{
  emberZclNotificationCallback(context, clusterSpec, attributeId, buffer, bufferLength);  // Plugin: zcl-core
}
