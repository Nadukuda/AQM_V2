// This file is generated by Simplicity Studio.  Please do not edit manually.
//
//

// Enclosing macro to prevent multiple inclusion
#ifndef SILABS_ZCL_DEVICE_H
#define SILABS_ZCL_DEVICE_H


// This file contains the useful definitions for the use of ZCL configured device.
#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_ZCL_CORE

// Cluster ids.
enum {
  EMBER_ZCL_CLUSTER_BASIC        = 0x0000,
  EMBER_ZCL_CLUSTER_IDENTIFY     = 0x0003,
  EMBER_ZCL_CLUSTER_ON_OFF       = 0x0006,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD = 0x2000,
};

// -----------------------------------------------------------------------------
// Basic cluster (0x0000).

// Basic cluster client (0x0000).
extern const EmberZclClusterSpec_t emberZclClusterBasicClientSpec;

// Basic cluster client (0x0000) - attribute ids.
enum {
  EMBER_ZCL_CLUSTER_BASIC_CLIENT_ATTRIBUTE_BASIC_CLUSTER_CLUSTER_REVISION_CLIENT = 0xFFFD,
  EMBER_ZCL_CLUSTER_BASIC_CLIENT_ATTRIBUTE_BASIC_CLUSTER_REPORTING_STATUS_CLIENT = 0xFFFE,
};

// Basic cluster server (0x0000).
extern const EmberZclClusterSpec_t emberZclClusterBasicServerSpec;

// Basic cluster server (0x0000) - attribute ids.
enum {
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_VERSION                               = 0x0000,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_APPLICATION_VERSION                   = 0x0001,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_STACK_VERSION                         = 0x0002,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_HW_VERSION                            = 0x0003,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_MANUFACTURER_NAME                     = 0x0004,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_MODEL_IDENTIFIER                      = 0x0005,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_DATE_CODE                             = 0x0006,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_POWER_SOURCE                          = 0x0007,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_GENERIC_DEVICE_CLASS                  = 0x0008,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_GENERIC_DEVICE_TYPE                   = 0x0009,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_PRODUCT_CODE                          = 0x000A,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_PRODUCT_URL                           = 0x000B,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_LOCATION_DESCRIPTION                  = 0x0010,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_PHYSICAL_ENVIRONMENT                  = 0x0011,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_DEVICE_ENABLED                        = 0x0012,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_ALARM_MASK                            = 0x0013,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_DISABLE_LOCAL_CONFIG                  = 0x0014,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_SW_BUILD_ID                           = 0x4000,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_BASIC_CLUSTER_CLUSTER_REVISION_SERVER = 0xFFFD,
  EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_BASIC_CLUSTER_REPORTING_STATUS_SERVER = 0xFFFE,
};

// Basic cluster server (0x0000) - command ids.
enum {
  EMBER_ZCL_CLUSTER_BASIC_SERVER_COMMAND_RESET_TO_FACTORY_DEFAULTS = 0x00,
};

// Basic cluster server (0x0000) - ResetToFactoryDefaults (0x00) command.
typedef struct {
  bool dummy;
} EmberZclClusterBasicServerCommandResetToFactoryDefaultsRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterBasicServerCommandResetToFactoryDefaultsResponse_t;
EmberStatus emberZclSendClusterBasicServerCommandResetToFactoryDefaultsResponse(
  const EmberZclCommandContext_t *context,
  const EmberZclClusterBasicServerCommandResetToFactoryDefaultsResponse_t *response);

// -----------------------------------------------------------------------------
// Identify cluster (0x0003).

// Identify cluster client (0x0003).
extern const EmberZclClusterSpec_t emberZclClusterIdentifyClientSpec;

// Identify cluster client (0x0003) - attribute ids.
enum {
  EMBER_ZCL_CLUSTER_IDENTIFY_CLIENT_ATTRIBUTE_IDENTIFY_CLUSTER_CLUSTER_REVISION_CLIENT = 0xFFFD,
  EMBER_ZCL_CLUSTER_IDENTIFY_CLIENT_ATTRIBUTE_IDENTIFY_CLUSTER_REPORTING_STATUS_CLIENT = 0xFFFE,
};

// Identify cluster server (0x0003).
extern const EmberZclClusterSpec_t emberZclClusterIdentifyServerSpec;

// Identify cluster server (0x0003) - attribute ids.
enum {
  EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_ATTRIBUTE_IDENTIFY_TIME                            = 0x0000,
  EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_ATTRIBUTE_COMMISSION_STATE                         = 0x0001,
  EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_ATTRIBUTE_IDENTIFY_CLUSTER_CLUSTER_REVISION_SERVER = 0xFFFD,
  EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_ATTRIBUTE_IDENTIFY_CLUSTER_REPORTING_STATUS_SERVER = 0xFFFE,
};

// Identify cluster server (0x0003) - command ids.
enum {
  EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_COMMAND_IDENTIFY                = 0x00,
  EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_COMMAND_IDENTIFY_QUERY          = 0x01,
  EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_COMMAND_E_Z_MODE_INVOKE         = 0x02,
  EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_COMMAND_UPDATE_COMMISSION_STATE = 0x03,
  EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_COMMAND_TRIGGER_EFFECT          = 0x40,
};

// Identify cluster server (0x0003) - Identify (0x00) command.
typedef struct {
  int16u identifyTime;
} EmberZclClusterIdentifyServerCommandIdentifyRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterIdentifyServerCommandIdentifyResponse_t;
typedef void (*EmberZclClusterIdentifyServerCommandIdentifyResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterIdentifyServerCommandIdentifyResponse_t *response);
EmberStatus emberZclSendClusterIdentifyServerCommandIdentifyRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterIdentifyServerCommandIdentifyRequest_t *request,
  const EmberZclClusterIdentifyServerCommandIdentifyResponseHandler handler);
EmberStatus emberZclSendClusterIdentifyServerCommandIdentifyResponse(
  const EmberZclCommandContext_t *context,
  const EmberZclClusterIdentifyServerCommandIdentifyResponse_t *response);

// Identify cluster server (0x0003) - IdentifyQuery (0x01) command.
typedef struct {
  bool dummy;
} EmberZclClusterIdentifyServerCommandIdentifyQueryRequest_t;
typedef struct {
  int16u timeout;
} EmberZclClusterIdentifyServerCommandIdentifyQueryResponse_t;
typedef void (*EmberZclClusterIdentifyServerCommandIdentifyQueryResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterIdentifyServerCommandIdentifyQueryResponse_t *response);
EmberStatus emberZclSendClusterIdentifyServerCommandIdentifyQueryRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterIdentifyServerCommandIdentifyQueryRequest_t *request,
  const EmberZclClusterIdentifyServerCommandIdentifyQueryResponseHandler handler);
EmberStatus emberZclSendClusterIdentifyServerCommandIdentifyQueryResponse(
  const EmberZclCommandContext_t *context,
  const EmberZclClusterIdentifyServerCommandIdentifyQueryResponse_t *response);

// Identify cluster server (0x0003) - EZModeInvoke (0x02) command.
typedef struct {
  int8u action;
} EmberZclClusterIdentifyServerCommandEZModeInvokeRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterIdentifyServerCommandEZModeInvokeResponse_t;
typedef void (*EmberZclClusterIdentifyServerCommandEZModeInvokeResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterIdentifyServerCommandEZModeInvokeResponse_t *response);
EmberStatus emberZclSendClusterIdentifyServerCommandEZModeInvokeRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterIdentifyServerCommandEZModeInvokeRequest_t *request,
  const EmberZclClusterIdentifyServerCommandEZModeInvokeResponseHandler handler);
EmberStatus emberZclSendClusterIdentifyServerCommandEZModeInvokeResponse(
  const EmberZclCommandContext_t *context,
  const EmberZclClusterIdentifyServerCommandEZModeInvokeResponse_t *response);

// Identify cluster server (0x0003) - UpdateCommissionState (0x03) command.
typedef struct {
  int8u action;
  int8u commissionStateMask;
} EmberZclClusterIdentifyServerCommandUpdateCommissionStateRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterIdentifyServerCommandUpdateCommissionStateResponse_t;
typedef void (*EmberZclClusterIdentifyServerCommandUpdateCommissionStateResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterIdentifyServerCommandUpdateCommissionStateResponse_t *response);
EmberStatus emberZclSendClusterIdentifyServerCommandUpdateCommissionStateRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterIdentifyServerCommandUpdateCommissionStateRequest_t *request,
  const EmberZclClusterIdentifyServerCommandUpdateCommissionStateResponseHandler handler);
EmberStatus emberZclSendClusterIdentifyServerCommandUpdateCommissionStateResponse(
  const EmberZclCommandContext_t *context,
  const EmberZclClusterIdentifyServerCommandUpdateCommissionStateResponse_t *response);

// Identify cluster server (0x0003) - TriggerEffect (0x40) command.
typedef struct {
  int8u effectId;
  int8u effectVariant;
} EmberZclClusterIdentifyServerCommandTriggerEffectRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterIdentifyServerCommandTriggerEffectResponse_t;
typedef void (*EmberZclClusterIdentifyServerCommandTriggerEffectResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterIdentifyServerCommandTriggerEffectResponse_t *response);
EmberStatus emberZclSendClusterIdentifyServerCommandTriggerEffectRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterIdentifyServerCommandTriggerEffectRequest_t *request,
  const EmberZclClusterIdentifyServerCommandTriggerEffectResponseHandler handler);
EmberStatus emberZclSendClusterIdentifyServerCommandTriggerEffectResponse(
  const EmberZclCommandContext_t *context,
  const EmberZclClusterIdentifyServerCommandTriggerEffectResponse_t *response);

// -----------------------------------------------------------------------------
// On/off cluster (0x0006).

// On/off cluster client (0x0006).
extern const EmberZclClusterSpec_t emberZclClusterOnOffClientSpec;

// On/off cluster client (0x0006) - attribute ids.
enum {
  EMBER_ZCL_CLUSTER_ON_OFF_CLIENT_ATTRIBUTE_ON_OFF_CLUSTER_CLUSTER_REVISION_CLIENT = 0xFFFD,
  EMBER_ZCL_CLUSTER_ON_OFF_CLIENT_ATTRIBUTE_ON_OFF_CLUSTER_REPORTING_STATUS_CLIENT = 0xFFFE,
};

// On/off cluster server (0x0006).
extern const EmberZclClusterSpec_t emberZclClusterOnOffServerSpec;

// On/off cluster server (0x0006) - attribute ids.
enum {
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_ATTRIBUTE_ON_OFF                                 = 0x0000,
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_ATTRIBUTE_GLOBAL_SCENE_CONTROL                   = 0x4000,
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_ATTRIBUTE_ON_TIME                                = 0x4001,
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_ATTRIBUTE_OFF_WAIT_TIME                          = 0x4002,
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_ATTRIBUTE_START_UP_ON_OFF                        = 0x4003,
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_ATTRIBUTE_ON_OFF_CLUSTER_CLUSTER_REVISION_SERVER = 0xFFFD,
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_ATTRIBUTE_ON_OFF_CLUSTER_REPORTING_STATUS_SERVER = 0xFFFE,
};

// On/off cluster server (0x0006) - command ids.
enum {
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_COMMAND_OFF                         = 0x00,
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_COMMAND_ON                          = 0x01,
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_COMMAND_TOGGLE                      = 0x02,
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_COMMAND_OFF_WITH_EFFECT             = 0x40,
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_COMMAND_ON_WITH_RECALL_GLOBAL_SCENE = 0x41,
  EMBER_ZCL_CLUSTER_ON_OFF_SERVER_COMMAND_ON_WITH_TIMED_OFF           = 0x42,
};

// On/off cluster server (0x0006) - Off (0x00) command.
typedef struct {
  bool dummy;
} EmberZclClusterOnOffServerCommandOffRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterOnOffServerCommandOffResponse_t;
typedef void (*EmberZclClusterOnOffServerCommandOffResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOnOffServerCommandOffResponse_t *response);
EmberStatus emberZclSendClusterOnOffServerCommandOffRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterOnOffServerCommandOffRequest_t *request,
  const EmberZclClusterOnOffServerCommandOffResponseHandler handler);

// On/off cluster server (0x0006) - On (0x01) command.
typedef struct {
  bool dummy;
} EmberZclClusterOnOffServerCommandOnRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterOnOffServerCommandOnResponse_t;
typedef void (*EmberZclClusterOnOffServerCommandOnResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOnOffServerCommandOnResponse_t *response);
EmberStatus emberZclSendClusterOnOffServerCommandOnRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterOnOffServerCommandOnRequest_t *request,
  const EmberZclClusterOnOffServerCommandOnResponseHandler handler);

// On/off cluster server (0x0006) - Toggle (0x02) command.
typedef struct {
  bool dummy;
} EmberZclClusterOnOffServerCommandToggleRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterOnOffServerCommandToggleResponse_t;
typedef void (*EmberZclClusterOnOffServerCommandToggleResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOnOffServerCommandToggleResponse_t *response);
EmberStatus emberZclSendClusterOnOffServerCommandToggleRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterOnOffServerCommandToggleRequest_t *request,
  const EmberZclClusterOnOffServerCommandToggleResponseHandler handler);

// On/off cluster server (0x0006) - OffWithEffect (0x40) command.
typedef struct {
  int8u effectId;
  int8u effectVariant;
} EmberZclClusterOnOffServerCommandOffWithEffectRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterOnOffServerCommandOffWithEffectResponse_t;
typedef void (*EmberZclClusterOnOffServerCommandOffWithEffectResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOnOffServerCommandOffWithEffectResponse_t *response);
EmberStatus emberZclSendClusterOnOffServerCommandOffWithEffectRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterOnOffServerCommandOffWithEffectRequest_t *request,
  const EmberZclClusterOnOffServerCommandOffWithEffectResponseHandler handler);

// On/off cluster server (0x0006) - OnWithRecallGlobalScene (0x41) command.
typedef struct {
  bool dummy;
} EmberZclClusterOnOffServerCommandOnWithRecallGlobalSceneRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterOnOffServerCommandOnWithRecallGlobalSceneResponse_t;
typedef void (*EmberZclClusterOnOffServerCommandOnWithRecallGlobalSceneResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOnOffServerCommandOnWithRecallGlobalSceneResponse_t *response);
EmberStatus emberZclSendClusterOnOffServerCommandOnWithRecallGlobalSceneRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterOnOffServerCommandOnWithRecallGlobalSceneRequest_t *request,
  const EmberZclClusterOnOffServerCommandOnWithRecallGlobalSceneResponseHandler handler);

// On/off cluster server (0x0006) - OnWithTimedOff (0x42) command.
typedef struct {
  int8u onOffControl;
  int16u onTime;
  int16u offWaitTime;
} EmberZclClusterOnOffServerCommandOnWithTimedOffRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterOnOffServerCommandOnWithTimedOffResponse_t;
typedef void (*EmberZclClusterOnOffServerCommandOnWithTimedOffResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOnOffServerCommandOnWithTimedOffResponse_t *response);
EmberStatus emberZclSendClusterOnOffServerCommandOnWithTimedOffRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterOnOffServerCommandOnWithTimedOffRequest_t *request,
  const EmberZclClusterOnOffServerCommandOnWithTimedOffResponseHandler handler);

// -----------------------------------------------------------------------------
// Over the Air Bootloading cluster (0x2000).

// Over the Air Bootloading cluster client (0x2000).
extern const EmberZclClusterSpec_t emberZclClusterOtaBootloadClientSpec;

// Over the Air Bootloading cluster client (0x2000) - attribute ids.
enum {
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_UPGRADE_SERVER_ID                            = 0x0000,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_FILE_OFFSET                                  = 0x0001,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_CURRENT_FILE_VERSION                         = 0x0002,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_CURRENT_ZIGBEE_STACK_VERSION                 = 0x0003,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_DOWNLOADED_FILE_VERSION                      = 0x0004,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_DOWNLOADED_ZIGBEE_STACK_VERSION              = 0x0005,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_IMAGE_UPGRADE_STATUS                         = 0x0006,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_MANUFACTURER_ID                              = 0x0007,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_IMAGE_TYPE_ID                                = 0x0008,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_MINIMUM_BLOCK_REQUEST_PERIOD                 = 0x0009,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_IMAGE_STAMP                                  = 0x000A,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_UPGRADE_ACTIVATION_POLICY                    = 0x000B,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_UPGRADE_TIMEOUT_POLICY                       = 0x000C,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_OTA_BOOTLOAD_CLUSTER_CLUSTER_REVISION_CLIENT = 0xFFFD,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_OTA_BOOTLOAD_CLUSTER_REPORTING_STATUS_CLIENT = 0xFFFE,
};

// Over the Air Bootloading cluster client (0x2000) - command ids.
enum {
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_COMMAND_IMAGE_NOTIFY = 0x00,
};

// Over the Air Bootloading cluster client (0x2000) - ImageNotify (0x00) command.
typedef struct {
  int8u payloadType;
  int8u queryJitter;
  int16u manufacturerId;
  int16u imageType;
  int32u newFileVersion;
} EmberZclClusterOtaBootloadClientCommandImageNotifyRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterOtaBootloadClientCommandImageNotifyResponse_t;
EmberStatus emberZclSendClusterOtaBootloadClientCommandImageNotifyResponse(
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOtaBootloadClientCommandImageNotifyResponse_t *response);

// Over the Air Bootloading cluster server (0x2000).
extern const EmberZclClusterSpec_t emberZclClusterOtaBootloadServerSpec;

// Over the Air Bootloading cluster server (0x2000) - attribute ids.
enum {
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_SERVER_ATTRIBUTE_OTA_BOOTLOAD_CLUSTER_CLUSTER_REVISION_SERVER = 0xFFFD,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_SERVER_ATTRIBUTE_OTA_BOOTLOAD_CLUSTER_REPORTING_STATUS_SERVER = 0xFFFE,
};

// Over the Air Bootloading cluster server (0x2000) - command ids.
enum {
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_SERVER_COMMAND_QUERY_NEXT_IMAGE_REQUEST    = 0x01,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_SERVER_COMMAND_IMAGE_BLOCK_REQUEST         = 0x03,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_SERVER_COMMAND_IMAGE_PAGE_REQUEST          = 0x04,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_SERVER_COMMAND_UPGRADE_END_REQUEST         = 0x06,
  EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_SERVER_COMMAND_QUERY_SPECIFIC_FILE_REQUEST = 0x08,
};

// Over the Air Bootloading cluster server (0x2000) - QueryNextImageRequest (0x01) command.
typedef struct {
  int8u fieldControl;
  int16u manufacturerId;
  int16u imageType;
  int32u currentFileVersion;
  int16u hardwareVersion;
} EmberZclClusterOtaBootloadServerCommandQueryNextImageRequest_t;
typedef struct {
  int8u status;
  int32u fileVersion;
  int32u imageSize;
  EmberZclStringType_t fileUri;
} EmberZclClusterOtaBootloadServerCommandQueryNextImageResponse_t;
typedef void (*EmberZclClusterOtaBootloadServerCommandQueryNextImageResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOtaBootloadServerCommandQueryNextImageResponse_t *response);
EmberStatus emberZclSendClusterOtaBootloadServerCommandQueryNextImageRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterOtaBootloadServerCommandQueryNextImageRequest_t *request,
  const EmberZclClusterOtaBootloadServerCommandQueryNextImageResponseHandler handler);

// Over the Air Bootloading cluster server (0x2000) - ImageBlockRequest (0x03) command.
typedef struct {
  int8u fieldControl;
  int16u manufacturerId;
  int16u imageType;
  int32u fileVersion;
  int32u fileOffset;
  int8u maxDataSize;
  EmberZclStringType_t requestNodeAddress;
} EmberZclClusterOtaBootloadServerCommandImageBlockRequest_t;
typedef struct {
  int8u status;
  int16u manufacturerId;
  int16u imageType;
  int32u fileVersion;
  int32u fileOffset;
  int8u dataSize;
  EmberZclStringType_t imageData;
} EmberZclClusterOtaBootloadServerCommandImageBlockResponse_t;
typedef void (*EmberZclClusterOtaBootloadServerCommandImageBlockResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOtaBootloadServerCommandImageBlockResponse_t *response);
EmberStatus emberZclSendClusterOtaBootloadServerCommandImageBlockRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterOtaBootloadServerCommandImageBlockRequest_t *request,
  const EmberZclClusterOtaBootloadServerCommandImageBlockResponseHandler handler);

// Over the Air Bootloading cluster server (0x2000) - ImagePageRequest (0x04) command.
typedef struct {
  int8u fieldControl;
  int16u manufacturerId;
  int16u imageType;
  int32u fileVersion;
  int32u fileOffset;
  int8u maxDataSize;
  int16u pageSize;
  int16u responseSpacing;
  EmberZclStringType_t requestNodeAddress;
} EmberZclClusterOtaBootloadServerCommandImagePageRequest_t;
typedef struct {
  enum8_t status;
} EmberZclClusterOtaBootloadServerCommandImagePageResponse_t;
typedef void (*EmberZclClusterOtaBootloadServerCommandImagePageResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOtaBootloadServerCommandImagePageResponse_t *response);
EmberStatus emberZclSendClusterOtaBootloadServerCommandImagePageRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterOtaBootloadServerCommandImagePageRequest_t *request,
  const EmberZclClusterOtaBootloadServerCommandImagePageResponseHandler handler);

// Over the Air Bootloading cluster server (0x2000) - UpgradeEndRequest (0x06) command.
typedef struct {
  int8u status;
  int16u manufacturerId;
  int16u imageType;
  int32u fileVersion;
} EmberZclClusterOtaBootloadServerCommandUpgradeEndRequest_t;
typedef struct {
  int16u manufacturerId;
  int16u imageType;
  int32u fileVersion;
  int32u currentTime;
  int32u upgradeTime;
} EmberZclClusterOtaBootloadServerCommandUpgradeEndResponse_t;
typedef void (*EmberZclClusterOtaBootloadServerCommandUpgradeEndResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOtaBootloadServerCommandUpgradeEndResponse_t *response);
EmberStatus emberZclSendClusterOtaBootloadServerCommandUpgradeEndRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterOtaBootloadServerCommandUpgradeEndRequest_t *request,
  const EmberZclClusterOtaBootloadServerCommandUpgradeEndResponseHandler handler);

// Over the Air Bootloading cluster server (0x2000) - QuerySpecificFileRequest (0x08) command.
typedef struct {
  EmberZclStringType_t requestNodeAddress;
  int16u manufacturerId;
  int16u imageType;
  int32u fileVersion;
  int16u currentZigbeeStackVersion;
} EmberZclClusterOtaBootloadServerCommandQuerySpecificFileRequest_t;
typedef struct {
  int8u status;
  int32u fileVersion;
  int32u imageSize;
  EmberZclStringType_t fileUri;
} EmberZclClusterOtaBootloadServerCommandQuerySpecificFileResponse_t;
typedef void (*EmberZclClusterOtaBootloadServerCommandQuerySpecificFileResponseHandler)(
  EmberZclMessageStatus_t status,
  const EmberZclCommandContext_t *context,
  const EmberZclClusterOtaBootloadServerCommandQuerySpecificFileResponse_t *response);
EmberStatus emberZclSendClusterOtaBootloadServerCommandQuerySpecificFileRequest(
  const EmberZclDestination_t *destination,
  const EmberZclClusterOtaBootloadServerCommandQuerySpecificFileRequest_t *request,
  const EmberZclClusterOtaBootloadServerCommandQuerySpecificFileResponseHandler handler);

// -----------------------------------------------------------------------------
// Endpoints.

#define EMBER_ZCL_CLUSTER_BASIC_SERVER_COUNT 1
#define EMBER_ZCL_CLUSTER_IDENTIFY_CLIENT_COUNT 1
#define EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_COUNT 1
#define EMBER_ZCL_CLUSTER_ON_OFF_CLIENT_COUNT 1
#define EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_COUNT 1

// -----------------------------------------------------------------------------
// Attributes.

#define EM_ZCL_ATTRIBUTE_DATA_SIZE 27
#define EM_ZCL_ATTRIBUTE_COUNT 25
#define EMBER_ZCL_ATTRIBUTE_MAX_SIZE 8
#define EM_ZCL_MAX_SIZE_NV_REPORTABLE_ATTRIBUTES 1
#define EM_ZCL_MAX_SIZE_NV_REPORTABLE_CHANGES 1
#define EM_ZCL_ENDPOINT_CLUSTERS_WITH_REPORTABLE_ATTRIBUTES 0

#if BIGENDIAN_CPU
  #define EM_ZCL_ATTRIBUTE_DEFAULTS { \
    0x00, 0x01, /* EMBER_ZCL_CLUSTER_IDENTIFY_CLIENT_ATTRIBUTE_IDENTIFY_CLUSTER_CLUSTER_REVISION_CLIENT */ \
    0x00, 0x01, /* EMBER_ZCL_CLUSTER_ON_OFF_CLIENT_ATTRIBUTE_ON_OFF_CLUSTER_CLUSTER_REVISION_CLIENT */ \
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_UPGRADE_SERVER_ID */ \
    0xFF, 0xFF, 0xFF, 0xFF, /* EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_FILE_OFFSET */ \
    0x00, /* EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_IMAGE_UPGRADE_STATUS */ \
    0x00, 0x01, /* EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_OTA_BOOTLOAD_CLUSTER_CLUSTER_REVISION_CLIENT */ \
    0x03, /* EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_VERSION */ \
    0x00, /* EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_POWER_SOURCE */ \
    0x00, 0x01, /* EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_BASIC_CLUSTER_CLUSTER_REVISION_SERVER */ \
    0x00, 0x00, /* EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_ATTRIBUTE_IDENTIFY_TIME */ \
    0x00, 0x01, /* EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_ATTRIBUTE_IDENTIFY_CLUSTER_CLUSTER_REVISION_SERVER */ \
  }
#else
  #define EM_ZCL_ATTRIBUTE_DEFAULTS { \
    0x01, 0x00, /* EMBER_ZCL_CLUSTER_IDENTIFY_CLIENT_ATTRIBUTE_IDENTIFY_CLUSTER_CLUSTER_REVISION_CLIENT */ \
    0x01, 0x00, /* EMBER_ZCL_CLUSTER_ON_OFF_CLIENT_ATTRIBUTE_ON_OFF_CLUSTER_CLUSTER_REVISION_CLIENT */ \
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_UPGRADE_SERVER_ID */ \
    0xFF, 0xFF, 0xFF, 0xFF, /* EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_FILE_OFFSET */ \
    0x00, /* EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_IMAGE_UPGRADE_STATUS */ \
    0x01, 0x00, /* EMBER_ZCL_CLUSTER_OTA_BOOTLOAD_CLIENT_ATTRIBUTE_OTA_BOOTLOAD_CLUSTER_CLUSTER_REVISION_CLIENT */ \
    0x03, /* EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_VERSION */ \
    0x00, /* EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_POWER_SOURCE */ \
    0x01, 0x00, /* EMBER_ZCL_CLUSTER_BASIC_SERVER_ATTRIBUTE_BASIC_CLUSTER_CLUSTER_REVISION_SERVER */ \
    0x00, 0x00, /* EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_ATTRIBUTE_IDENTIFY_TIME */ \
    0x01, 0x00, /* EMBER_ZCL_CLUSTER_IDENTIFY_SERVER_ATTRIBUTE_IDENTIFY_CLUSTER_CLUSTER_REVISION_SERVER */ \
  }
#endif

#endif // SILABS_ZCL_DEVICE_H
