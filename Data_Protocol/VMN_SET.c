/*
 * VMN_SET.c
 *
 *  Created on: 28 Nov 2018
 *      Author: Aidan
 */


#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#include EMBER_AF_API_HAL
#include EMBER_AF_API_COMMAND_INTERPRETER2
#include "VMN_definitions.h"

enum {
	INITIAL			= 0,
	RESET			= 1,
	REFORM			= 2,
};

static uint8_t net_id[EMBER_NETWORK_ID_SIZE];
static uint8_t ula[8];
static uint8_t master_key_in[16];
static uint16_t pan_id;

static uint8_t resetState = INITIAL;

EmberEventControl resetEventControl;
static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs);
#define setNextState(nextState)			setNextStateWithDelay((nextState), 0)
#define repeatState()					setNextStateWithDelay(i2cState, 0)
#define repeatStateWithDelay(delayMs)	setNextStateWithDelay(i2cState, (delayMs))

#define RESET_WAIT_PERIOD_MS   (30 * MILLISECOND_TICKS_PER_SECOND)

int8_t power_in;
static long interval;

int SET_commandCheck(uint8_t *payload, uint8_t *source);

int SET_commandCheck(uint8_t *payload, uint8_t *source){
	uint16_t message_id = payload[ID_MSB]<<8 | payload[ID_LSB];

	switch(message_id){

		case 0x0002: // SET-EUI64

			emberAfAppPrint("ConnectionSuccessful, ");
			emberAfAppPrint("nodeEUI=");
//			for(int i = 6; i < 14; i++ ){
//				emberAfAppPrint("%x", payload[i]);
//			}

			for(int i = 13; i > 5; i-- ){
				emberAfAppPrint("%x", payload[i]);
			}
			emberAfAppPrint(",");
			emberAfAppPrint("nodeIP=");
			emberAfPrintIpv6Address(source);
			emberAfAppPrintln("");

			break;

		case 0x0003: // SET-NET-RESET
			code_in = ((payload[6] << 8 ) | payload[7]);
			if(code_in == CODE){
				emberAfAppPrintln("RESET CALLED");
				setNextState(RESET);
			}
			emberAfAppPrintln("WRONG CODE = %2x", code_in);
			emberAfAppPrintln(" msb = %x", payload[6]);
			emberAfAppPrintln(" lsb = %x", payload[7]);
			break;

		case 0x0004: // SET-SOFT-RESET
			emberAfAppPrintln("SOFT RESET CALLED");
			halReboot();
		break;

		case 0x0005: // SET-EUID-QUERY
			VMN_euidQuery();
		break;

		case 0x0006: // SET-RADIO-POWER
			power_in = payload[6];
			emberSetRadioPower(power_in);
			emberGetRadioPower();
		break;

		case 0x0007: // SET-HEARTBEAT-INTVL
			interval = (payload[INTERVAL_BYTE1] << 24 |
						payload[INTERVAL_BYTE2] << 16 |
						payload[INTERVAL_BYTE3] << 8  |
						payload[INTERVAL_BYTE4]);
			setHeartbeatInterval(interval);
		break;

		case 0x0008: // GET-TXPOWER-LEVEL
			emberGetRadioPower();
			break;

		case 0x0009: // GET-HEARTBEAT-INTVL
			getHeartbeatInterval();
			break;

		case 0x000A: //SET-NODE-TYPE
			changeNodeType(payload[DATA_BIT]);
			break;

		case 0x000B: // GET-NODE-TYPE
			getNodeType();
			break;

		case 0x000C: // SET-NET-PARAM

			/* Network ID - 16 bytes (net_payload 0 - 15)*/
			for(uint8_t i = 0; i < EMBER_NETWORK_ID_SIZE; i++){
				net_id[i] = payload[i+6];
			}
			VMN_setNetworkId(&net_id);

			/*ULA prefix - 8 bytes (net_payload 16 - 23)*/
			for(uint8_t i = 0; i < 8; i++){
				ula[i] = payload[i+6+16];
//				emberAfAppPrintln("%x", payload[i+6]);
			}
			VMN_setULA(&ula);

			/*Pan ID - 2 bytes ( net_payload 24 - 25)*/

			pan_id = (payload[6+24] <<8) | payload[6+25];
			emberAfAppPrintln("payload pan = %2x",pan_id);
			VMN_setPanId(pan_id);


			/*Channel - 1 byte ( net_payload 26)*/
			VMN_setChannel(payload[6+26]);







			/*Node Type - 1 byte ( net_payload 27)*/
//			VMN_setNodeType(payload[6+27]);

			/*Radio Power - 1 byte ( net_payload 28)*/
//			VMN_setRadioPower((int8_t)payload[6+28]);

			/*Master Key - 16 bytes ( net_payload 29 - 45)*/
//			for(uint8_t i = 0; i < 16; i++){
//				net_id[i] = payload[i+6+29];
//			}
//			VMN_setMasterKey(&net_id);

			break;

		case 0x000D: // SET-MASTER-KEY
			/*Master Key - 16 bytes ( payload 6 - 22)*/
			for(uint8_t i = 0; i < 16; i++){
				master_key_in[i] = payload[i+6];
			}
			VMN_setMasterKey(&master_key_in);
			break;

		case 0x000E: // SET-NET-SWAP
			VMN_setJoinMode(payload[DATA_BIT]);
			break;

		case 0x000F: // SET-AQM-POWER-MODE
			setAQMPowerMode(payload[DATA_BIT]);
			break;

		case 0x0010: // GET-AQM-POWER-MODE
			getAQMPowerMode();
			break;

		case 0x0011: // GET-AQM-POWER-MODE
			VMN_rtccSetDateTime(payload);
			break;

		case 0x0012: // GET-AQM-POWER-MODE
			VMN_rtccGetDateTime();
			break;
	}
	return 1;
}

void resetEventHandler(void){
	emberEventControlSetInactive(resetEventControl);

	switch(resetState) {
		case RESET :
			emberResetNetworkState();
			emberAfAppPrintln("Reset");
			setNextStateWithDelay(REFORM, 5000);
			break;

		case REFORM :
			emberAfAppPrintln("Reform");
			halReboot();
			setNextState(INITIAL);
			break;
	}
}


static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs)
{
  resetState = nextState;
  emberEventControlSetDelayMS(resetEventControl, delayMs);
}
