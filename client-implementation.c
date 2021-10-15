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
#include EMBER_AF_API_HAL
#include EMBER_AF_API_COMMAND_INTERPRETER2
#ifdef EMBER_AF_API_DEBUG_PRINT
  #include EMBER_AF_API_DEBUG_PRINT
#endif
#ifdef CORTEXM3_EFM32_MICRO
  #include "tempdrv.h"
#endif

#define ALIAS(x) x##Alias
#include "app/thread/plugin/udp-debug/udp-debug.c"

#include <Data_Protocol/VMN_globals.h>
#include <Data_Protocol/VMN_definitions.h>
#include "VMN_setup_parameters.h"

// WARNING: Random join keys MUST be used for real devices, with the keys
// typically programmed during product manufacturing and printed on the devices
// themselves for access by the user.  This application will read the join key
// from a manufacturing token.  For ease of use, this application will generate
// a random join key at runtime if the manufacturing token was not set
// previously.  This will then be saved in a manufacturing token.  Join keys
// SHOULD NOT be generated at runtime.  They should be produced at
// manufacturing time.

// The client/server sample applications use a fixed network id to simplify
// the join process.
static const uint8_t networkId[EMBER_NETWORK_ID_SIZE] = "VMN/SETUP";

#define JOIN_BUTTON BUTTON1
#define JOIN_KEY_SIZE 8

static void resumeNetwork(void);
static void joinNetwork(void);
static void getJoinKey(uint8_t *joinKey, uint8_t *joinKeyLength);
static void createRandomJoinKey(uint8_t *joinKey, uint8_t *joinKeyLength);
static void waitForServerAdvertisement(void);
static void attachToServer(const EmberIpv6Address *newServer);
static void reportDataToServer(void);
static int32_t getTemp_mC(void);
static void detachFromServer(void);
static void resetNetworkState(void);
void setHeartbeatInterval(long interval_in);
void getHeartbeatInterval(void);
static
uint8_t currentNodeType(void);

static EmberIpv6Address server;
static uint8_t failedReports;
#define REPORT_FAILURE_LIMIT 3
#define WAIT_PERIOD_MS   (60 * MILLISECOND_TICKS_PER_SECOND)
#define REPORT_PERIOD_MS (300 * MILLISECOND_TICKS_PER_SECOND)
static long heartbeat_interval = 60000;

static const uint8_t clientReportUri[] = "client/heartbeat";

enum {
  INITIAL                       = 0,
  RESUME_NETWORK                = 1,
  JOIN_NETWORK                  = 2,
  WAIT_FOR_SERVER_ADVERTISEMENT = 4,
  REPORT_DATA_TO_SERVER         = 5,
  WAIT_FOR_DATA_CONFIRMATION    = 6,
  RESET_NETWORK_STATE           = 7,
  COMMISSION					= 8,
  JOIN_COMMISSIONED				= 9,
  ATTACH_TO_NETWORK				=10,
};
static uint8_t state = INITIAL;
EmberEventControl stateEventControl;
static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs);
#define setNextState(nextState)       setNextStateWithDelay((nextState), 0)
#define repeatState()                 setNextStateWithDelay(state, 0)
#define repeatStateWithDelay(delayMs) setNextStateWithDelay(state, (delayMs))

static const uint8_t *printableNetworkId(void);

/*
 * mode 0 = manual joining
 * mode 1 = VMN\SETUP
 * mode 2 = SECURE Settings
 */
static uint8_t VMN_join_mode = 1;
static uint8_t network_backoff_count = 0;

///********************************SETUP NETWORK DATA*****************************************/

//#define VMN_SETUP_NETWORK_NAME "VMN/OFFICE"
//#define VMN_SETUP_NETWORK_NAME_LENGTH (10)
//#define VMN_SETUP_PAN_ID 5418
//#define VMN_SETUP_EXTENDED_PAN_ID { 0x7E, 0xC1, 0xE5, 0xDE, 0x5F, 0xA2, 0xE4, 0xE6 }
//#define VMN_SETUP_ULA_PREFIX { 0xFD, 0xE6, 0x4C, 0xD3, 0xA4, 0xE0, 0x00, 0x01 }
//#define VMN_SETUP_RADIO_TX_POWER 10
//#define VMN_SETUP_CHANNEL 25


static EmberNetworkParameters setup_parameters = {{0}};

static uint8_t setup_networkId[EMBER_NETWORK_ID_SIZE] = VMN_SETUP_NETWORK_NAME;

static EmberIpv6Prefix setup_static_prefix = VMN_SETUP_ULA_PREFIX;

static uint16_t setup_panId = VMN_SETUP_PAN_ID;

static uint8_t setup_channel = VMN_SETUP_CHANNEL;

static EmberNodeType setup_node_type = EMBER_ROUTER;

static uint8_t setup_radio_power = VMN_SETUP_RADIO_TX_POWER;

//static EmberKeyData setup_masterkey = {
//  { 0x56, 0x4D, 0x4E, 0x5F, 0x53, 0x45, 0x54, 0x55,
//	0x50, 0x4E, 0x45, 0x54, 0x57, 0x4F, 0x52, 0x4B, }
//};

static EmberKeyData setup_masterkey = VMN_SETUP_MASTER_KEY;


uint8_t joinKey[EMBER_JOIN_KEY_MAX_SIZE + 1] = { 0 };


static uint8_t setup_extended_panid[] = VMN_SETUP_EXTENDED_PAN_ID;

///********************************************************************************************/
///********************************SECURE NETWORK DATA*****************************************/


static EmberNetworkParameters secure_parameters = {{0}};

static uint8_t secure_networkId[EMBER_NETWORK_ID_SIZE];

static EmberIpv6Prefix secure_static_prefix;

static uint16_t secure_panId;

static uint8_t secure_channel;

static EmberNodeType secure_node_type = EMBER_END_DEVICE;

static uint8_t secure_radio_power = VMN_SETUP_RADIO_TX_POWER;

//static EmberKeyData setup_masterkey = {#define ALIAS(x) x##Alias
//  { 0x56, 0x4D, 0x4E, 0x5F, 0x53, 0x45, 0x54, 0x55,
//	0x50, 0x4E, 0x45, 0x54, 0x57, 0x4F, 0x52, 0x4B, }
//};

//static EmberKeyData secure_masterkey = {
//		{ 0 }
//};
static EmberKeyData secure_masterkey = {
		{ 0x46, 0xe1, 0xaa, 0x95, 0x4a, 0xd1, 0x6d, 0x20, 0xf9, 0xfb, 0xfe, 0x08, 0xf0, 0x4c, 0x95, 0x00 }
};

static uint8_t secure_extended_panid[] = VMN_SETUP_EXTENDED_PAN_ID;

///********************************************************************************************/


void emberAfNetworkStatusCallback(EmberNetworkStatus newNetworkStatus,
                                  EmberNetworkStatus oldNetworkStatus,
                                  EmberJoinFailureReason reason)
{
  // This callback is called whenever the network status changes, like when
  // we finish joining to a network or when we lose connectivity.  If we have
  // no network, we try joining to one.  If we have a saved network, we try to
  // resume operations on that network.  When we are joined and attached to the
  // network, we wait for an advertisement and then begin reporting.

  emberEventControlSetInactive(stateEventControl);

  switch (newNetworkStatus) {
  emberAfAppPrintln("Bryce");
    case EMBER_NO_NETWORK:
      if (oldNetworkStatus == EMBER_JOINING_NETWORK) {
        emberAfCorePrintln("ERR: Joining failed: 0x%x", reason);
      } else if(VMN_join_mode > 0){
    	  setNextState(COMMISSION);
//    	  emberAfCorePrintln("Commissioning...");
//    	  emberCommissionNetwork(setup_channel,
//    	                         0,                 // fallback channel mask - ignored
//    	                         setup_networkId,
//    	                         sizeof(setup_networkId),
//    	                         setup_panId,
//    	                         setup_static_prefix.bytes,
//    	                         setup_extended_panid,
//    	                         &setup_masterkey,
//    	                         0);
//    	  halCommonDelayMilliseconds(2000);
//    	  emberJoinCommissioned(setup_radio_power,
//    			  	  	  	  	  setup_node_type,
//								  0);
//    	  halCommonDelayMilliseconds(2000);
//    	  emberAttachToNetwork();
//    	  setNextState(WAIT_FOR_SERVER_ADVERTISEMENT);
      }
      break;
    case EMBER_SAVED_NETWORK:
      setNextState(RESUME_NETWORK);
      break;
    case EMBER_JOINING_NETWORK:
      // Wait for either the "attaching" or "no network" state.
      break;
    case EMBER_JOINED_NETWORK_ATTACHING:
      // Wait for the "attached" state.
      if (oldNetworkStatus == EMBER_JOINED_NETWORK_ATTACHED) {
        emberAfCorePrintln("Trying to re-connect...");
      }
      break;
    case EMBER_JOINED_NETWORK_ATTACHED:
      emberAfCorePrintln("%s network \"%s\"",
                         (state == RESUME_NETWORK
                          ? "Resumed operation on"
                          : (state == JOIN_NETWORK
                             ? "Joined"
                             : "Rejoined")),
                         printableNetworkId());
      // TODO: For a brief interruption in connectivity, the client could attempt
      // to continue reporting to its previous server, rather than wait for a new
      // server.
      setNextState(WAIT_FOR_SERVER_ADVERTISEMENT);
      break;
    case EMBER_JOINED_NETWORK_NO_PARENT:
      // We always join as a router, so we should never end up in the "no parent"
      // state.
//      assert(false);
    	setNextState(WAIT_FOR_SERVER_ADVERTISEMENT);
      break;
    default:
      assert(false);
      break;
  }
}

static void resumeNetwork(void)
{
  assert(state == RESUME_NETWORK);

  emberAfCorePrintln("Resuming operation on network \"%s\"", printableNetworkId());

  emberResumeNetwork();
}

void emberResumeNetworkReturn(EmberStatus status)
{
  // This return indicates whether the stack is going to attempt to resume.  If
  // so, the result is reported later as a network status change.  If we cannot
  // even attempt to resume, we just give up and try joining instead.

  assert(state == RESUME_NETWORK);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("ERR: Unable to resume: 0x%x", status);
    setNextState(JOIN_NETWORK);
  }
}

static void joinNetwork(void)
{
  // When joining a network, we look for one specifically with our network id.
  // The commissioner must have our join key for this to succeed.

  EmberNetworkParameters parameters = { { 0 } };
  uint8_t joinKey[EMBER_JOIN_KEY_MAX_SIZE + 1] = { 0 };

  assert(state == JOIN_NETWORK);

  MEMCOPY(parameters.networkId, networkId, sizeof(networkId));
  parameters.nodeType = EMBER_ROUTER;
  parameters.radioTxPower = 17;
  getJoinKey(joinKey, &parameters.joinKeyLength);
  MEMCOPY(parameters.joinKey, joinKey, parameters.joinKeyLength);

  emberAfCorePrint("Joining network \"%s\" with EUI64 ", networkId);
  emberAfCoreDebugExec(emberAfPrintBigEndianEui64(emberEui64()));
  emberAfCorePrintln(" and join key \"%s\"", joinKey);

  emberJoinNetwork(&parameters,
                   (EMBER_NETWORK_ID_OPTION
                    | EMBER_NODE_TYPE_OPTION
                    | EMBER_TX_POWER_OPTION
                    | EMBER_JOIN_KEY_OPTION),
                   EMBER_ALL_802_15_4_CHANNELS_MASK);
}

// join
void joinCommand(void)
{
  // If we are not in a join state, we will attempt to join a network.
  // This is functionally the same as pushing the JOIN_BUTTON.

  if (emberNetworkStatus() == EMBER_NO_NETWORK) {
    setNextState(JOIN_NETWORK);
  }
}

static void getJoinKey(uint8_t *joinKey, uint8_t *joinKeyLength)
{
  tokTypeMfgThreadJoinKey token;
  halCommonGetMfgToken(&token, TOKEN_MFG_THREAD_JOIN_KEY);
  if (token.joinKeyLength == 0xFFFF) {
    createRandomJoinKey(joinKey, joinKeyLength);
    emberAfCorePrintln("WARNING: Join key not set");
    token.joinKeyLength = *joinKeyLength;
    MEMCOPY(token.joinKey, joinKey, token.joinKeyLength);
    halCommonSetMfgToken(TOKEN_MFG_THREAD_JOIN_KEY, &token);
  } else {
    *joinKeyLength = token.joinKeyLength;
    MEMCOPY(joinKey, token.joinKey, *joinKeyLength);
    joinKey[*joinKeyLength] = '\0';
  }
}

static void createRandomJoinKey(uint8_t *joinKey, uint8_t *joinKeyLength)
{
  // If the manufacturing token containing the join key was not set, a random
  // join key is generated.  The Thread specification disallows the characters
  // I, O, Q, and Z, for readability.

  const char characters[] = "ABCDEFGHJKLMNPRSTUVWXY1234567890";

  for (*joinKeyLength = 0; *joinKeyLength < JOIN_KEY_SIZE; (*joinKeyLength)++ ) {
    uint16_t key = halCommonGetRandom() % (int) (sizeof(characters) - 1);
    joinKey[*joinKeyLength] = characters[key];
  }
  joinKey[*joinKeyLength] = '\0';
}

// get-join-key
void getJoinKeyCommand(void)
{
  // This function gets the join key and then prints it.  If the join key has
  // not been created yet, it will be created in the getJoinKey function and
  // then printed.

  uint8_t joinKey[EMBER_JOIN_KEY_MAX_SIZE + 1];
  uint8_t joinKeyLength;

  getJoinKey(joinKey, &joinKeyLength);
  emberAfCorePrintln("Join key: \"%s\"", joinKey);
}

void emberJoinNetworkReturn(EmberStatus status)
{
  // This return indicates whether the stack is going to attempt to join.  If
  // so, the result is reported later as a network status change.  Otherwise,
  // we just try again.

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("ERR: Unable to join: 0x%x", status);
    repeatState();
  }
}


static void waitForServerAdvertisement(void)
{
  // Once on the network, we wait for a server to advertise itself.  We
  // periodically print a message while waiting to prove we are alive.

  assert(state == WAIT_FOR_SERVER_ADVERTISEMENT);

  emberAfCorePrintln("Waiting for an advertisement from a server");

  if(VMN_join_mode > 0 && network_backoff_count > 2 && secure_networkId[0] != NULL){ //secure masterkey content check
	  if(VMN_join_mode == 1){
		  VMN_join_mode = 2;
		  network_backoff_count = 0;
		  resetNetworkState();
	  } else if(VMN_join_mode == 2){
		  VMN_join_mode = 1;
		  network_backoff_count = 0;
		  resetNetworkState();
	  }
  }
  network_backoff_count++;
  emberAfAppPrintln("Backoff count = %d", network_backoff_count);
  repeatStateWithDelay(WAIT_PERIOD_MS);
}
//static void waitForServerAdvertisement(void)
//{
//  // Once on the network, we wait for a server to advertise itself.  We
//  // periodically print a message while waiting to prove we are alive.
//
//  assert(state == WAIT_FOR_SERVER_ADVERTISEMENT);
//
//  emberAfCorePrintln("Waiting for an advertisement from a server");
//
//  if(VMN_join_mode > 0 && network_backoff_count > 2){
//	  if(VMN_join_mode == 1){
//		  VMN_join_mode = 2;
//		  network_backoff_count = 0;
//		  resetNetworkState();
//	  } else if(VMN_join_mode == 2){
//		  VMN_join_mode = 1;
//		  network_backoff_count = 0;
//		  resetNetworkState();
//	  }
//  }
//  network_backoff_count++;
//  emberAfAppPrintln("Backoff count = %d", network_backoff_count);
//  repeatStateWithDelay(WAIT_PERIOD_MS);
//}

void serverAdvertiseHandler(EmberCoapCode code,
                            uint8_t *uri,
                            EmberCoapReadOptions *options,
                            const uint8_t *payload,
                            uint16_t payloadLength,
                            const EmberCoapRequestInfo *info)
{
  // Advertisements from servers are sent as CoAP POST requests to the
  // "server/advertise" URI.  When we receive an advertisement, we attach to
  // the that sent it and beginning sending reports.

  EmberCoapCode responseCode;

  if (state != WAIT_FOR_SERVER_ADVERTISEMENT) {
    responseCode = EMBER_COAP_CODE_503_SERVICE_UNAVAILABLE;
  } else {
    attachToServer(&info->remoteAddress);
    responseCode = EMBER_COAP_CODE_204_CHANGED;
  }

  if (emberCoapIsSuccessResponse(responseCode)
      || info->localAddress.bytes[0] != 0xFF) { // not multicast
    emberCoapRespondWithCode(info, responseCode);
  }
}

static void attachToServer(const EmberIpv6Address *newServer)
{
  // We attach to a server in response to an advertisement (or a CLI command).
  // Once we have a server, we begin reporting periodically.  We start from a
  // clean state with regard to failed reports.

  assert(state == WAIT_FOR_SERVER_ADVERTISEMENT);

  MEMCOPY(&server, newServer, sizeof(EmberIpv6Address));
  failedReports = 0;

  // puts server address in global variable for VMN data protocol functions to use.
  MEMCOPY(&global_server, newServer, sizeof(EmberIpv6Address));

  emberAfCorePrint("Attached to server at ");
  emberAfCoreDebugExec(emberAfPrintIpv6Address(newServer));
  emberAfCorePrintln("");

  VMN_euidQuery();

  if(VMN_join_mode == 2 && !aqmIsActive()){
	  airQualityMasterStart();
  }

  /*
   * Disables Native RTC replaces it with RTCC Calendar Mode
   */
//  rtccSetup();

//  if(VMN_join_mode == 2 && currentNodeType() != 3){
//	  changeNodeType(secure_node_type);
//  }

//  //******************************AIDAN**************************
//  // sends euid and ip to server once connected
//
//	uint8_t EUI[EUI64_SIZE];
//
//	// copy EUI64 into EUI placeholder array
//	memcpy(EUI, emberEui64()->bytes, EUI64_SIZE);
//
//	VMN_payloadPackager(&server, EUI, sizeof(EUI), 0x02, 0x0002);
//
////*******************************AIDAN****************************


  setNextState(REPORT_DATA_TO_SERVER);
}

// attach <server>
void attachCommand(void)
{
  // If we are waiting for a server, we can manually attach to one using a CLI
  // command.

  if (state == WAIT_FOR_SERVER_ADVERTISEMENT) {
    EmberIpv6Address newServer = { { 0 } };
    if (emberGetIpv6AddressArgument(0, &newServer)) {
      attachToServer(&newServer);
    } else {
      emberAfCorePrintln("ERR: Invalid ip");
    }
  }
}

static void processServerDataAck(EmberCoapStatus status,
                                 EmberCoapCode code,
                                 EmberCoapReadOptions *options,
                                 uint8_t *payload,
                                 uint16_t payloadLength,
                                 EmberCoapResponseInfo *info)
{
  // We track the success or failure of reports so that we can determine when
  // we have lost the server.  A series of consecutive failures is the trigger
  // to detach from the current server and find a new one.  Any successfully-
  // transmitted report clears past failures.

  if (state == WAIT_FOR_DATA_CONFIRMATION) {
    if (status == EMBER_COAP_MESSAGE_ACKED
        || status == EMBER_COAP_MESSAGE_RESPONSE) {
      failedReports = 0;
    } else {
      failedReports++;
      emberAfCorePrintln("ERR: Report timed out - failure %u of %u",
                         failedReports,
                         REPORT_FAILURE_LIMIT);
    }
    if (failedReports < REPORT_FAILURE_LIMIT) {
      setNextStateWithDelay(REPORT_DATA_TO_SERVER, heartbeat_interval);
    } else {
      detachFromServer();
    }
  }
}

static void reportDataToServer(void)
{
  // We peridocally send our euid and a flag to show if we have buffered data to send. The success or failure of the reports
  // is tracked so we can determine if the server has disappeared and we should
  // find a new one.
  EmberCoapSendInfo info = { 0 }; // use defaults
  EmberStatus status;

  uint8_t EUI[EUI64_SIZE+1+6];
//  	uint8_t heartbeat_payload[EUI64_SIZE + 1 + 16];
//  	EmberIpv6Address local_address;
//  	uint8_t ip_type = 0; // 0 = mesh local, 1 = link local , >1 =  global unicast address

//  	emberGetLocalIpAddress(ip_type, &local_address);

//copy EUI64 into heartbeat_payload array
  memcpy(EUI, emberEui64()->bytes, EUI64_SIZE);
//  	memcpy(heartbeat_payload, emberEui64()->bytes, EUI64_SIZE);

//Flag to let server know there is data in buffer
  EUI[8] = (queueIsEmpty() == 1) ? 0 : 1 ;
//    heartbeat_payload[8] = (queueIsEmpty() == 1) ? 0 : 1 ;

//copy IP address into heartbeat_payload
//    memcpy(heartbeat_payload + 9, local_address.bytes, sizeof(local_address.bytes));

  //Firmware Image Version = EUI 9 + 10 + 11 + 12
  EUI[9]  = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_VERSION >> 24;
  EUI[10] = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_VERSION >> 16;
  EUI[11] = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_VERSION >> 8;
  EUI[12] = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_VERSION;

  //Firmware Image Type = EUI location 13 + 14

  EUI[13] = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_TYPE >> 8;
  EUI[14] = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_TYPE;

  assert(state == REPORT_DATA_TO_SERVER);

  emberAfCorePrint("Reporting to server at ");
  emberAfCoreDebugExec(emberAfPrintIpv6Address(&server));
  emberAfCorePrintln("");

//  emberAfCorePrint("heartbeat_payload =  ");
//  for(uint8_t i = 0; i < sizeof(heartbeat_payload); i++){
//	  emberAfCorePrint("%X",heartbeat_payload[i]);
//  }
//  emberAfCorePrintln("");


  status = emberCoapPost(&server,
                         clientReportUri,
                         (const uint8_t *)&EUI,
                         sizeof(EUI),
                         processServerDataAck,
                         &info);
  if (status == EMBER_SUCCESS) {
    setNextState(WAIT_FOR_DATA_CONFIRMATION);
  } else {
    emberAfCorePrintln("ERR: Reporting failed: 0x%x", status);
    repeatStateWithDelay(heartbeat_interval);
  }
}

#ifdef CORTEXM3_EFM32_MICRO
static int32_t getTemp_mC(void)
{
  return TEMPDRV_GetTemp() * 1000;
}
#else
static int32_t getTemp_mC(void)
{
  uint16_t value;
  int16_t volts;
  halStartAdcConversion(ADC_USER_APP,
                        ADC_REF_INT,
                        TEMP_SENSOR_ADC_CHANNEL,
                        ADC_CONVERSION_TIME_US_256);
  halReadAdcBlocking(ADC_USER_APP, &value);
  volts = halConvertValueToVolts(value / TEMP_SENSOR_SCALE_FACTOR);
  return (1591887L - (171 * (int32_t)volts)) / 10;
}
#endif

// report
void reportCommand(void)
{
  // If we have a server and are reporting data, we can manually send a new
  // report using a CLI command.

  if (state == REPORT_DATA_TO_SERVER) {
    reportDataToServer();
  }
}

static void detachFromServer(void)
{
  // We detach from a server in response to failed reports (or a CLI command).
  // Once we detach, we wait for a new server to advertise itself.

  assert(state == REPORT_DATA_TO_SERVER
         || state == WAIT_FOR_DATA_CONFIRMATION);

  emberAfCorePrint("Detached from server at ");
  emberAfCoreDebugExec(emberAfPrintIpv6Address(&server));
  emberAfCorePrintln("");

  setNextState(WAIT_FOR_SERVER_ADVERTISEMENT);
}

// detach
void detachCommand(void)
{
  // If we have a server and are reporting data, we can manually detach and try
  // to find a new server by using a CLI command.

  if (state == REPORT_DATA_TO_SERVER
      || state == WAIT_FOR_DATA_CONFIRMATION) {
    detachFromServer();
  }
}

static void resetNetworkState(void)
{
  emberAfCorePrintln("Resetting network state");
  emberResetNetworkState();
}

void emberResetNetworkStateReturn(EmberStatus status)
{
  // If we ever leave the network, we go right back to joining again.  This
  // could be triggered by an external CLI command.

  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("Reset network state");
  }
}

//void emberUdpHandler(const uint8_t *destination,
//                     const uint8_t *source,
//                     uint16_t localPort,
//                     uint16_t remotePort,
//                     const uint8_t *payload,
//                     uint16_t payloadLength)
//{
//  ALIAS(emberUdpHandler)(destination,
//                         source,
//                         localPort,
//                         remotePort,
//                         payload,
//                         payloadLength);
//}

bool emberAfPluginIdleSleepOkToSleepCallback(uint32_t durationMs)
{
  // Once we join to a network, we will automatically stay awake, because we
  // are a router.  Before we join, we would could sleep, but we prevent that
  // by returning false here.

  return false;
}

//void halButtonIsr(uint8_t button, uint8_t buttonState)
//{
//  // When the user pushes JOIN_BUTTON, the client will attempt to join a
//  // network.
//
//  if (buttonState == BUTTON_PRESSED && button == JOIN_BUTTON) {
//    if (emberNetworkStatus() == EMBER_NO_NETWORK) {
//      setNextState(JOIN_NETWORK);
//    }
//  }
//}

void stateEventHandler(void)
{
  emberEventControlSetInactive(stateEventControl);

  switch (state) {
  case COMMISSION:
      	emberAfCorePrintln("Commissioning...");
      	network_backoff_count = 0;
      	if(VMN_join_mode == 1){
      		emberAfAppPrint("MASTER KEY = ");
      		for(uint8_t i = 0; i < 16; i++){
      			emberAfAppPrint("%x ",setup_masterkey.contents[i]);
      		}
      		emberAfAppPrintln("");
			emberCommissionNetwork(setup_channel,
								 0,                 // fallback channel mask - ignored
								 setup_networkId,
								 sizeof(setup_networkId),
								 setup_panId,
								 setup_static_prefix.bytes,
								 setup_extended_panid,
								 &setup_masterkey,
								 0);
      	} else if(VMN_join_mode == 2){
      		emberAfAppPrint("MASTER KEY = ");
      		for(uint8_t i = 0; i < 16; i++){
      			emberAfAppPrint("%x ",secure_masterkey.contents[i]);
      		}
      		emberAfAppPrintln("");
			emberCommissionNetwork(secure_channel,
								 0,                 // fallback channel mask - ignored
								 secure_networkId,
								 sizeof(secure_networkId),
								 secure_panId,
								 secure_static_prefix.bytes,
								 secure_extended_panid,
								 &secure_masterkey,
								 0);
      	}

      	break;

      case JOIN_COMMISSIONED:

    	  if(VMN_join_mode == 1){
			  emberJoinCommissioned(setup_radio_power,
									  setup_node_type,
									  true);
    	  } else if(VMN_join_mode == 2){
			  emberJoinCommissioned(secure_radio_power,
					  	  	  	  	  setup_node_type,
									  true);
    	  }

  	  setNextStateWithDelay(ATTACH_TO_NETWORK, 5000);
      break;

      case ATTACH_TO_NETWORK:
      	emberAttachToNetwork();
      break;
    case RESUME_NETWORK:
      resumeNetwork();
      break;
    case JOIN_NETWORK:
      joinNetwork();
      break;
    case WAIT_FOR_SERVER_ADVERTISEMENT:
      waitForServerAdvertisement();
      break;
    case REPORT_DATA_TO_SERVER:
      reportDataToServer();
      break;
    case WAIT_FOR_DATA_CONFIRMATION:
      break;
    case RESET_NETWORK_STATE:
      resetNetworkState();
      break;
    default:
      assert(false);
      break;
  }
}

static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs)
{
  state = nextState;
  emberEventControlSetDelayMS(stateEventControl, delayMs);
}

static const uint8_t *printableNetworkId(void)
{
  EmberNetworkParameters parameters = { { 0 } };
  static uint8_t networkId[EMBER_NETWORK_ID_SIZE + 1] = { 0 };
  emberGetNetworkParameters(&parameters);
  MEMCOPY(networkId, parameters.networkId, EMBER_NETWORK_ID_SIZE);
  return networkId;
}

void VMN_euidQuery(void){
	//******************************AIDAN**************************
	// sends euid and ip to server once connected

	uint8_t EUI[EUI64_SIZE];

	// copy EUI64 into EUI placeholder array
	memcpy(EUI, emberEui64()->bytes, EUI64_SIZE);

	VMN_payloadPackager(&server, EUI, sizeof(EUI), 0x02, 0x0002);

	//*******************************AIDAN****************************
}


void setHeartbeatInterval(long interval_in){
	heartbeat_interval = interval_in;
	getHeartbeatInterval();
}

void getHeartbeatInterval(void){
	uint8_t p_load[] = {(heartbeat_interval >> 24),
						(heartbeat_interval >> 16),
						(heartbeat_interval >> 8),
						 heartbeat_interval,
	};
	VMN_payloadPackager(&server, p_load, sizeof(p_load), DAT, 0x0022);
}

void changeNodeType(uint8_t node_type){
	network_backoff_count = 0;
	emberChangeNodeType(node_type);

}

void getNodeType(void){
	EmberNetworkParameters current_parameters = { { 0 } };
	uint8_t node_type;

	emberGetNetworkParameters(&current_parameters);
	node_type = current_parameters.nodeType;

	uint8_t p_load[] = {node_type};

	VMN_payloadPackager(&server, p_load, sizeof(p_load), DAT, 0x0023);
}

uint8_t currentNodeType(void){
	EmberNetworkParameters current_parameters = { { 0 } };
	uint8_t node_type;

	emberGetNetworkParameters(&current_parameters);
	node_type = current_parameters.nodeType;

	return node_type;
}

void waitForServer(void){
	network_backoff_count = 0;
	setNextState(WAIT_FOR_SERVER_ADVERTISEMENT);
}

void joinCommissionedState(void){
	setNextState(JOIN_COMMISSIONED);
}

void VMN_setJoinMode(uint8_t mode_in){
	VMN_join_mode = mode_in;
	detachCommand();
	resetNetworkState();
}

void VMN_setNetworkId(uint8_t *net_id){
	uint8_t new_net_id[EMBER_NETWORK_ID_SIZE];
	MEMCOPY(secure_networkId, net_id, EMBER_NETWORK_ID_SIZE);
	emberAfAppPrintln("Network name = %s", secure_networkId);
}

void VMN_setULA(uint8_t *ula_in){
	uint8_t ula[8];
	MEMCOPY(ula,ula_in,8);
	emberAfAppPrint("New ULA =");
	for(uint8_t i = 0; i < 8; i++){
		secure_static_prefix.bytes[i] = ula[i];
		emberAfAppPrint("%x", secure_static_prefix.bytes[i]);
		if(i == 1 || i == 3 || i == 5){
			emberAfAppPrint(":");
		}
	}
	emberAfAppPrintln("");
}

void VMN_setPanId(uint16_t pan_id_in){
	secure_panId = pan_id_in;
	emberAfAppPrintln("NEW Pan ID = %2x", secure_panId);
}

void VMN_setChannel(uint8_t channel_in){
	secure_channel = channel_in;
	emberAfAppPrintln("NEW Channel = %d", channel_in);
}

void VMN_setNodeType(uint8_t type_in){
	secure_node_type = type_in;
	emberAfAppPrintln("NEW type = %d", secure_node_type);
}

void VMN_setRadioPower(int8_t power_in){
	secure_radio_power = power_in;
	emberAfAppPrintln("NEW power = %d", secure_radio_power);
}

void VMN_setMasterKey(uint8_t *master_key_in){
//	uint8_t master_in[16];
	MEMCOPY(secure_masterkey.contents, master_key_in, 16);
	emberAfAppPrint("NEW MasterKey =");
	for(uint8_t i = 0; i < 16; i++){
		emberAfAppPrint("%x ", secure_masterkey.contents[i]);
	}
	emberAfAppPrintln("");

}
