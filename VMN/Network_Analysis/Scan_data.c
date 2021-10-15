/*
 * Scan_data.c
 *
 *  Created on: 6 Jun 2019
 *      Author: Aidan
 *
 *    Takes data gathered during an active network scan.
 *    Transmits data to the server on completion of scan.
 *
 */



#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#ifdef EMBER_AF_API_DEBUG_PRINT
  #include EMBER_AF_API_DEBUG_PRINT
#endif

#include <Data_Protocol/VMN_definitions.h>
#include <Data_Protocol/VMN_globals.h>


static uint8_t count = 0;
static uint8_t transmit_count = 0;
static uint8_t lqi[36];
static uint8_t rssi[36];
static uint8_t mac_id[36][8];

void VMN_startNetworkScan(void);
void VMN_scanDataPackager(const EmberMacBeaconData * beaconData);
void VMN_scanDataTransmit(void);
uint8_t VMN_getScanCount(void);

void VMN_startNetworkScan(void){
	emberAfAppPrintln("SCAN START");
	count = 0;
	transmit_count = 0;

	EmberNetworkScanType scanType =  EMBER_ACTIVE_SCAN;

	uint8_t channel = (emberCommandArgumentCount() > 0
	                     ? (uint8_t)emberUnsignedCommandArgument(0)
	                     : 0);
	uint32_t channelMask = (channel == 0
	                          ? EMBER_ALL_802_15_4_CHANNELS_MASK
	                          : BIT32(channel));

	uint8_t duration = (emberCommandArgumentCount() > 1
	                      ? (uint8_t)emberUnsignedCommandArgument(1)
	                      : DEFAULT_SCAN_DURATION);

	emberStartScan(scanType, channelMask, duration);
//	networkManagementScanCommand();

}

void VMN_scanDataPackager(const EmberMacBeaconData * beaconData){
	lqi[count] 		= beaconData->lqi;
	rssi[count]		= beaconData->rssi;
	MEMCOPY(mac_id[count], beaconData->macExtendedId, 8);

	count ++;
}

void VMN_scanDataTransmit(void){
	uint8_t payload[10];

	payload[0] = lqi[transmit_count];
	payload[1] = rssi[transmit_count];
	for(uint8_t i = 0; i < 8; i++){
		payload[i+2] = mac_id[transmit_count][i];
	}

	emberAfAppPrintln("lqi = %u",payload[0]);
	emberAfAppPrintln("rssi = %d",(int8_t)payload[1]);
//	emberAfCoreDebugExec(emberAfPrintExtendedPanId(beaconData->extendedPanId));


	VMN_payloadPackager(&global_server, payload, 10, DAT, 0x001B);
	transmit_count++;

	if(transmit_count < count){
		VMN_scanDataTransmit();
	}
}

uint8_t VMN_getScanCount(void){
	return count;
}

