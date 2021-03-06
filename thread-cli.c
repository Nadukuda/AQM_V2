// This file is generated by Simplicity Studio.  Please do not edit manually.
//
//

// This file contains the mapping for the command line configuration

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_EMBER_TYPES
#ifdef EMBER_AF_API_THREAD_TEST_HARNESS_CLI
#include EMBER_AF_API_THREAD_TEST_HARNESS_CLI
#endif
#include EMBER_AF_API_COMMAND_INTERPRETER2

void attachCommand(void);
void coapDeleteCommand(void);
void coapGetCommand(void);
void coapListenCommand(void);
void coapPostCommand(void);
void coapPutCommand(void);
void coapSetOptionAcceptCommand(void);
void coapSetOptionBlockOptionValueCommand(void);
void coapSetOptionContentFormatCommand(void);
static EmberCommandEntry emberCommandCoapSetOptionTable[] = {
  emberCommandEntryAction("accept", coapSetOptionAcceptCommand, "v", ""),
  emberCommandEntryAction("block-option-value", coapSetOptionBlockOptionValueCommand, "uuw", ""),
  emberCommandEntryAction("content-format", coapSetOptionContentFormatCommand, "v", ""),
  emberCommandEntryTerminator(),
};
static EmberCommandEntry emberCommandCoapTable[] = {
  emberCommandEntryAction("delete", coapDeleteCommand, "bb", ""),
  emberCommandEntryAction("get", coapGetCommand, "bb", ""),
  emberCommandEntryAction("listen", coapListenCommand, "b", ""),
  emberCommandEntryAction("post", coapPostCommand, "bbb*", ""),
  emberCommandEntryAction("put", coapPutCommand, "bbb*", ""),
  emberCommandEntrySubMenu("set-option", emberCommandCoapSetOptionTable, ""),
  emberCommandEntryTerminator(),
};
void coapsDeleteCommand(void);
void coapsGetCommand(void);
void coapsPostCommand(void);
void coapsPutCommand(void);
static EmberCommandEntry emberCommandCoapsTable[] = {
  emberCommandEntryAction("delete", coapsDeleteCommand, "bb", ""),
  emberCommandEntryAction("get", coapsGetCommand, "bb", ""),
  emberCommandEntryAction("post", coapsPostCommand, "bbb*", ""),
  emberCommandEntryAction("put", coapsPutCommand, "bbb*", ""),
  emberCommandEntryTerminator(),
};
void detachCommand(void);
void emAfEepromDataPrintCommand(void);
void emAfEepromInfoCommand(void);
void emAfEepromStatusCommand(void);
static EmberCommandEntry emberCommandEepromTable[] = {
  emberCommandEntryAction("data-print", emAfEepromDataPrintCommand, "w", ""),
  emberCommandEntryAction("info", emAfEepromInfoCommand, "", ""),
  emberCommandEntryAction("status", emAfEepromStatusCommand, "", ""),
  emberCommandEntryTerminator(),
};
void getJoinKeyCommand(void);
void emberPrintCommandTable(void);
void icmpListenCommand(void);
void icmpPingCommand(void);
static EmberCommandEntry emberCommandIcmpTable[] = {
  emberCommandEntryAction("listen", icmpListenCommand, "b", ""),
  emberCommandEntryAction("ping", icmpPingCommand, "bv*", ""),
  emberCommandEntryTerminator(),
};
void networkManagementInfoCommand(void);
void joinCommand(void);
void emberAttachToNetwork(void);
void networkManagementCommissionCommand(void);
void networkManagementCommissioningStartCommand(void);
void emberStopCommissioning(void);
static EmberCommandEntry emberCommandNetworkManagementCommissioningTable[] = {
  emberCommandEntryAction("start", networkManagementCommissioningStartCommand, "b", ""),
  emberCommandEntryAction("stop", emberStopCommissioning, "", ""),
  emberCommandEntryTerminator(),
};
void networkManagementFormCommand(void);
void networkManagementConfigureGatewayCommand(void);
void networkManagementGetGlobalAddressesCommand(void);
void emberGetGlobalPrefixes(void);
void networkManagementJoinCommand(void);
void networkManagementJoinCommissionedCommand(void);
void networkManagementListenersCommand(void);
void emberResetNetworkState(void);
void emberResumeNetwork(void);
void networkManagementScanCommand(void);
void networkManagementScanCommand(void);
void emberStopScan(void);
static EmberCommandEntry emberCommandNetworkManagementScanTable[] = {
  emberCommandEntryAction("active", networkManagementScanCommand, "u*", ""),
  emberCommandEntryAction("energy", networkManagementScanCommand, "u*", ""),
  emberCommandEntryAction("stop", emberStopScan, "", ""),
  emberCommandEntryTerminator(),
};
void networkManagementSetJoinKeyCommand(void);
void networkManagementSetJoiningModeCommand(void);
void networkManagementSetMasterKeyCommand(void);
void networkManagementSteeringAddCommand(void);
void emberSendSteeringData(void);
static EmberCommandEntry emberCommandNetworkManagementSteeringTable[] = {
  emberCommandEntryAction("add", networkManagementSteeringAddCommand, "b", ""),
  emberCommandEntryAction("send", emberSendSteeringData, "", ""),
  emberCommandEntryTerminator(),
};
static EmberCommandEntry emberCommandNetworkManagementTable[] = {
  emberCommandEntryAction("attach", emberAttachToNetwork, "", ""),
  emberCommandEntryAction("commission", networkManagementCommissionCommand, "uwbbbbv*", ""),
  emberCommandEntrySubMenu("commissioning", emberCommandNetworkManagementCommissioningTable, ""),
  emberCommandEntryAction("form", networkManagementFormCommand, "usub*", ""),
  emberCommandEntryAction("gateway", networkManagementConfigureGatewayCommand, "vubuww", ""),
  emberCommandEntryAction("global-addresses", networkManagementGetGlobalAddressesCommand, "b*", ""),
  emberCommandEntryAction("global-prefixes", emberGetGlobalPrefixes, "", ""),
  emberCommandEntryAction("join", networkManagementJoinCommand, "usubbvb", ""),
  emberCommandEntryAction("join-commissioned", networkManagementJoinCommissionedCommand, "suu*", ""),
  emberCommandEntryAction("listeners", networkManagementListenersCommand, "", ""),
  emberCommandEntryAction("reset", emberResetNetworkState, "", ""),
  emberCommandEntryAction("resume", emberResumeNetwork, "", ""),
  emberCommandEntrySubMenu("scan", emberCommandNetworkManagementScanTable, ""),
  emberCommandEntryAction("set-join-key", networkManagementSetJoinKeyCommand, "bb*", ""),
  emberCommandEntryAction("set-joining-mode", networkManagementSetJoiningModeCommand, "uu", ""),
  emberCommandEntryAction("set-master-key", networkManagementSetMasterKeyCommand, "b", ""),
  emberCommandEntrySubMenu("steering", emberCommandNetworkManagementSteeringTable, ""),
  emberCommandEntryTerminator(),
};
void otaBootloadClientForceUpdate(void);
static EmberCommandEntry emberCommandOtaBootloadClientTable[] = {
  emberCommandEntryAction("update", otaBootloadClientForceUpdate, "", ""),
  emberCommandEntryTerminator(),
};
void otaBootloadStorageInfoCommand(void);
static EmberCommandEntry emberCommandOtaBootloadStorageTable[] = {
  emberCommandEntryAction("info", otaBootloadStorageInfoCommand, "", ""),
  emberCommandEntryTerminator(),
};
void reportCommand(void);
void emberResetMicro(void);
void udpListenCommand(void);
void udpSendCommand(void);
static EmberCommandEntry emberCommandUdpTable[] = {
  emberCommandEntryAction("listen", udpListenCommand, "vb", ""),
  emberCommandEntryAction("send", udpSendCommand, "bvvbu*", ""),
  emberCommandEntryTerminator(),
};
void emberGetVersions(void);
void emZclCliAccessControlModeCommand(void);
void emZclCliAccessControlPrintCommand(void);
static EmberCommandEntry emberCommandZclAccessControlTable[] = {
  emberCommandEntryAction("mode", emZclCliAccessControlModeCommand, "u", ""),
  emberCommandEntryAction("print", emZclCliAccessControlPrintCommand, "", ""),
  emberCommandEntryTerminator(),
};
void emZclCliAttributePrintCommand(void);
void emZclCliAttributeReadCommand(void);
void emZclCliAttributeRemoteReadCommand(void);
void emZclCliAttributeRemoteWriteCommand(void);
static EmberCommandEntry emberCommandZclAttributeRemoteTable[] = {
  emberCommandEntryAction("read", emZclCliAttributeRemoteReadCommand, "uvvv*", ""),
  emberCommandEntryAction("write", emZclCliAttributeRemoteWriteCommand, "uvvvb", ""),
  emberCommandEntryTerminator(),
};
void emZclCliAttributeResetCommand(void);
void emZclCliAttributeWriteCommand(void);
static EmberCommandEntry emberCommandZclAttributeTable[] = {
  emberCommandEntryAction("print", emZclCliAttributePrintCommand, "", ""),
  emberCommandEntryAction("read", emZclCliAttributeReadCommand, "uuvvv", ""),
  emberCommandEntrySubMenu("remote", emberCommandZclAttributeRemoteTable, ""),
  emberCommandEntryAction("reset", emZclCliAttributeResetCommand, "u", ""),
  emberCommandEntryAction("write", emZclCliAttributeWriteCommand, "uuvvvb", ""),
  emberCommandEntryTerminator(),
};
void emZclCliBindingAddSetCommand(void);
void emZclCliBindingClearCommand(void);
void emZclCliBindingPrintCommand(void);
void emZclCliBindingRemoteAddUpdateCommand(void);
void emZclCliBindingRemoteRemoveCommand(void);
void emZclCliBindingRemoteAddUpdateCommand(void);
static EmberCommandEntry emberCommandZclBindingRemoteTable[] = {
  emberCommandEntryAction("add", emZclCliBindingRemoteAddUpdateCommand, "uvvubvuvu", ""),
  emberCommandEntryAction("remove", emZclCliBindingRemoteRemoveCommand, "uvvu", ""),
  emberCommandEntryAction("update", emZclCliBindingRemoteAddUpdateCommand, "uvvubvuvuu", ""),
  emberCommandEntryTerminator(),
};
void emZclCliBindingRemoveCommand(void);
void emZclCliBindingAddSetCommand(void);
static EmberCommandEntry emberCommandZclBindingTable[] = {
  emberCommandEntryAction("add", emZclCliBindingAddSetCommand, "uuvvubvuvu", ""),
  emberCommandEntryAction("clear", emZclCliBindingClearCommand, "", ""),
  emberCommandEntryAction("print", emZclCliBindingPrintCommand, "", ""),
  emberCommandEntrySubMenu("remote", emberCommandZclBindingRemoteTable, ""),
  emberCommandEntryAction("remove", emZclCliBindingRemoveCommand, "u", ""),
  emberCommandEntryAction("set", emZclCliBindingAddSetCommand, "uuvvubvuvuu", ""),
  emberCommandEntryTerminator(),
};
void emZclCliCacheAddCommand(void);
static EmberCommandEntry emberCommandZclCacheAddTable[] = {
  emberCommandEntryAction("uid-ipv6", emZclCliCacheAddCommand, "bb", ""),
  emberCommandEntryTerminator(),
};
void emZclCliCacheClearCommand(void);
void emZclCliCachePrintCommand(void);
void emZclCliCacheRemoveCommand(void);
static EmberCommandEntry emberCommandZclCacheTable[] = {
  emberCommandEntrySubMenu("add", emberCommandZclCacheAddTable, ""),
  emberCommandEntryAction("clear", emZclCliCacheClearCommand, "", ""),
  emberCommandEntryAction("print", emZclCliCachePrintCommand, "uu", ""),
  emberCommandEntryAction("remove", emZclCliCacheRemoveCommand, "v", ""),
  emberCommandEntryTerminator(),
};
void emZclCliCborForceDefiniteLenCommand(void);
static EmberCommandEntry emberCommandZclCborTable[] = {
  emberCommandEntryAction("force-definite-len", emZclCliCborForceDefiniteLenCommand, "u", ""),
  emberCommandEntryTerminator(),
};
void emZclCliDiscSetAcceptCommand(void);
void emZclCliDiscByClusterIdCommand(void);
void emZclCliDiscByClusterRevisionCommand(void);
void emZclCliDiscByDeviceTypeCommand(void);
void emZclCliDiscByEndpointCommand(void);
void emZclCliDiscInitCommand(void);
void emZclCliDiscSetModeCommand(void);
void emZclCliDiscByResourceVersionCommand(void);
void emZclCliDiscSendCommand(void);
void emZclCliDiscByUidCommand(void);
static EmberCommandEntry emberCommandZclDiscoveryTable[] = {
  emberCommandEntryAction("accept", emZclCliDiscSetAcceptCommand, "v", ""),
  emberCommandEntryAction("cluster", emZclCliDiscByClusterIdCommand, "uvv", ""),
  emberCommandEntryAction("cluster-revision", emZclCliDiscByClusterRevisionCommand, "v", ""),
  emberCommandEntryAction("device-type", emZclCliDiscByDeviceTypeCommand, "v", ""),
  emberCommandEntryAction("endpoint", emZclCliDiscByEndpointCommand, "uv", ""),
  emberCommandEntryAction("init", emZclCliDiscInitCommand, "", ""),
  emberCommandEntryAction("mode", emZclCliDiscSetModeCommand, "u", ""),
  emberCommandEntryAction("resource-version", emZclCliDiscByResourceVersionCommand, "v", ""),
  emberCommandEntryAction("send", emZclCliDiscSendCommand, "", ""),
  emberCommandEntryAction("uid", emZclCliDiscByUidCommand, "b", ""),
  emberCommandEntryTerminator(),
};
void emZclCliEzModeStartCommand(void);
void emberZclStopEzMode(void);
static EmberCommandEntry emberCommandZclEzModeTable[] = {
  emberCommandEntryAction("start", emZclCliEzModeStartCommand, "", ""),
  emberCommandEntryAction("stop", emberZclStopEzMode, "", ""),
  emberCommandEntryTerminator(),
};
void emZclCliClusterIdentifyServerCommandEZModeInvokeRequestCommand(void);
void emZclCliClusterIdentifyServerCommandIdentifyRequestCommand(void);
void emZclCliClusterIdentifyServerCommandIdentifyQueryRequestCommand(void);
void emZclCliClusterIdentifyServerCommandTriggerEffectRequestCommand(void);
static EmberCommandEntry emberCommandZclIdentifyTable[] = {
  emberCommandEntryAction("ez-mode", emZclCliClusterIdentifyServerCommandEZModeInvokeRequestCommand, "u", "Invoke EZMode on an Identify Server"),
  emberCommandEntryAction("id", emZclCliClusterIdentifyServerCommandIdentifyRequestCommand, "v", "Command description for Identify"),
  emberCommandEntryAction("query", emZclCliClusterIdentifyServerCommandIdentifyQueryRequestCommand, "", "Command description for IdentifyQuery"),
  emberCommandEntryAction("trigger", emZclCliClusterIdentifyServerCommandTriggerEffectRequestCommand, "uu", "Command description for TriggerEffect"),
  emberCommandEntryTerminator(),
};
void emZclCliInfoCommand(void);
void emZclCliClusterOnOffServerCommandOffRequestCommand(void);
void emZclCliClusterOnOffServerCommandOffWithEffectRequestCommand(void);
void emZclCliClusterOnOffServerCommandOnRequestCommand(void);
void emZclCliClusterOnOffServerCommandOnWithRecallGlobalSceneRequestCommand(void);
void emZclCliClusterOnOffServerCommandOnWithTimedOffRequestCommand(void);
void emZclCliClusterOnOffServerCommandToggleRequestCommand(void);
static EmberCommandEntry emberCommandZclOnOffTable[] = {
  emberCommandEntryAction("off", emZclCliClusterOnOffServerCommandOffRequestCommand, "", "Command description for Off"),
  emberCommandEntryAction("offeffect", emZclCliClusterOnOffServerCommandOffWithEffectRequestCommand, "uu", "Command description for OffWithEffect"),
  emberCommandEntryAction("on", emZclCliClusterOnOffServerCommandOnRequestCommand, "", "Command description for On"),
  emberCommandEntryAction("onrecall", emZclCliClusterOnOffServerCommandOnWithRecallGlobalSceneRequestCommand, "", "Command description for OnWithRecallGlobalScene"),
  emberCommandEntryAction("ontimedoff", emZclCliClusterOnOffServerCommandOnWithTimedOffRequestCommand, "uvv", "Command description for OnWithTimedOff"),
  emberCommandEntryAction("toggle", emZclCliClusterOnOffServerCommandToggleRequestCommand, "", "Command description for Toggle"),
  emberCommandEntryTerminator(),
};
void emZclCliSendCommand(void);
void emZclCliSendCommand(void);
void emZclCliSendCommand(void);
static EmberCommandEntry emberCommandZclSendTable[] = {
  emberCommandEntryAction("binding", emZclCliSendCommand, "u", ""),
  emberCommandEntryAction("endpoint", emZclCliSendCommand, "ubu", ""),
  emberCommandEntryAction("group", emZclCliSendCommand, "bv", ""),
  emberCommandEntryTerminator(),
};
static EmberCommandEntry emberCommandZclTable[] = {
  emberCommandEntrySubMenu("access-control", emberCommandZclAccessControlTable, ""),
  emberCommandEntrySubMenu("attribute", emberCommandZclAttributeTable, ""),
  emberCommandEntrySubMenu("binding", emberCommandZclBindingTable, ""),
  emberCommandEntrySubMenu("cache", emberCommandZclCacheTable, ""),
  emberCommandEntrySubMenu("cbor", emberCommandZclCborTable, ""),
  emberCommandEntrySubMenu("discovery", emberCommandZclDiscoveryTable, ""),
  emberCommandEntrySubMenu("ez-mode", emberCommandZclEzModeTable, ""),
  emberCommandEntrySubMenu("identify", emberCommandZclIdentifyTable, ""),
  emberCommandEntryAction("info", emZclCliInfoCommand, "", ""),
  emberCommandEntrySubMenu("on-off", emberCommandZclOnOffTable, ""),
  emberCommandEntrySubMenu("send", emberCommandZclSendTable, ""),
  emberCommandEntryTerminator(),
};

void airQualityMasterStart(void);
extern void CORE_si7021Start(void);
extern void CORE_si7021Stop(void);
extern void si7021Start(void);
extern void si7021Stop(void);
void rtccPrintDateTime(void);
extern void rtc_datetime_set(void);
extern void rtc_datetime_get(void);

EmberCommandEntry emberCommandTable[] = {
  emberCommandEntryAction("attach", attachCommand, "b", ""),
  emberCommandEntrySubMenu("coap", emberCommandCoapTable, ""),
  emberCommandEntrySubMenu("coaps", emberCommandCoapsTable, ""),
  emberCommandEntryAction("detach", detachCommand, "", ""),
  emberCommandEntrySubMenu("eeprom", emberCommandEepromTable, ""),
  emberCommandEntryAction("get-join-key", getJoinKeyCommand, "", ""),
  emberCommandEntryAction("help", emberPrintCommandTable, "", ""),
  emberCommandEntrySubMenu("icmp", emberCommandIcmpTable, ""),
  emberCommandEntryAction("info", networkManagementInfoCommand, "", ""),
  emberCommandEntryAction("join", joinCommand, "", ""),
  emberCommandEntrySubMenu("network-management", emberCommandNetworkManagementTable, ""),
  emberCommandEntrySubMenu("ota-bootload-client", emberCommandOtaBootloadClientTable, ""),
  emberCommandEntrySubMenu("ota-bootload-storage", emberCommandOtaBootloadStorageTable, ""),
  emberCommandEntryAction("report", reportCommand, "", ""),
  emberCommandEntryAction("reset", emberResetMicro, "", ""),
  emberCommandEntrySubMenu("udp", emberCommandUdpTable, ""),
  emberCommandEntryAction("versions", emberGetVersions, "", ""),
  emberCommandEntrySubMenu("zcl", emberCommandZclTable, ""),
  emberCommandEntryAction("aqm_start", airQualityMasterStart, "", ""),
  emberCommandEntryAction("temp_internalstart", CORE_si7021Start, "", ""),
  emberCommandEntryAction("temp_internalstop", CORE_si7021Stop, "", ""),
  emberCommandEntryAction("temp_externalstart", si7021Start, "", ""),
  emberCommandEntryAction("temp_externalstop", si7021Stop, "", ""),
  emberCommandEntryAction("set_datetime", rtc_datetime_set, "", ""),
  emberCommandEntryAction("get_datetime", rtc_datetime_get, "", ""),

#ifdef EMBER_AF_ENABLE_CUSTOM_COMMANDS
  CUSTOM_COMMANDS
#endif // EMBER_AF_ENABLE_CUSTOM_COMMANDS
  emberCommandEntryTerminator(),
};
