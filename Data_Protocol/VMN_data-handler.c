/*
 * VMN_Data_Handler.c
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

static uint8_t net_id[EMBER_NETWORK_ID_SIZE];
static uint8_t ula[8];
static uint8_t master_key_in[16];
static uint16_t pan_id;

// EUID placeholder array
uint8_t EUI[EUI64_SIZE];

uint8_t fw_version[6];


void nullResponse(const EmberCoapRequestInfo *info, EmberCoapCode responseCode);
void payloadResponse(const EmberCoapRequestInfo *info, EmberCoapCode responseCode, uint8_t *payload, uint8_t payload_length);

void emberUdpHandler(const uint8_t *destination,
                     const uint8_t *source,
                     uint16_t localPort,
                     uint16_t remotePort,
                     const uint8_t *payload,
                     uint16_t payloadLength) {
//if(localPort == 5683){
		uint8_t message_class = payload[2];
		uint8_t message_id_MSB = payload[3];
		uint8_t message_id_LSB = payload[4];

		emberAfAppPrintln("class = %X", message_class);
		emberAfAppPrintln("ID = %X%X", message_id_MSB, message_id_LSB);
		emberAfAppPrintln("Payload Length = %X",payload[5]);
		emberAfAppPrint("Payload = ");
		for(uint8_t i = 6; i < sizeof(payload-2); i++){
			emberAfAppPrint("%X", payload[i]);
		}
		emberAfAppPrintln("");
		uint16_t RX_checksum = (payload[payloadLength-2] << 8) | payload[payloadLength-1];

		uint16_t calc_checksum = checksumGenerator(payload, payloadLength-2);

		emberAfAppPrintln("rx checksum = %2x : calc checksum = %2x", RX_checksum, calc_checksum);

		if(RX_checksum == calc_checksum){

			emberAfAppPrint("Source=");
			emberAfPrintIpv6Address(source);
			emberAfAppPrint(",");

			switch(message_class) {

				case 0x01: // ACK - Acknowledgement messages
					if(payload[ID_LSB] == 0x01){
						emberAfAppPrintln("ACK Received - %x:%2x", payload[6], (payload[7]<<8 | payload[8]));
						ackCheck(payload);
					} else {
						emberAfAppPrintln("Message Failure");
					}
					break;

				case 0x02: // SET - Network, device or configuration messages
					if(SET_commandCheck(payload, source)){
						sendAck(source, payload);
					} else {
						sendNak(source, payload);
					}
					break;

				case 0x03: // CMD - Operational Commands
#ifdef NODE_TYPE_SENSOR
					if(CMD_commandCheck(payload)){
						sendAck(source, payload);
					} else {
						sendNak(source, payload);
					}
#endif
					break;

				case 0x04: // DAT - Sensor Data
#ifndef NODE_TYPE_SENSOR
					DAT_decoder(payload);
#endif
					break;

				case 0x05: // MON - Device and Network Monitoring Data
					VMN_startNetworkScan();
					break;

				case 0x06: // INF - Information Messages

					break;

				case 0x07: // UPD - Updates

					break;
			}
		} else {
			emberAfAppPrintln("CHECKSUM ERROR");
		}
//	}
}

//	void settingsHandler()

void VMNMessageHandler(EmberCoapCode code,
                         uint8_t *uri,
                         EmberCoapReadOptions *options,
                         const uint8_t *payload,
                         uint16_t payloadLength,
                         const EmberCoapRequestInfo *info)
{
  EmberCoapCode responseCode;
  uint8_t message_class = payload[2];
  uint8_t id_msb = payload[3];
  uint8_t id_lsb = payload[4];
  uint8_t interval_payload[4];
  uint8_t response_payload[] = {0x56, 0xD7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  uint32_t temp;
  uint32_t return_temp;
//  EmberIpv6Address source = info->remoteAddress;
  emberAfAppPrintln("COAP - message class RX = %X : %X%X : Length = %d", message_class, id_msb, id_lsb, payloadLength);

  response_payload[2] = 0x04;
//  response_payload[3] = id_msb;
//  response_payload[4] = id_lsb;
  switch(message_class){



  case 0x02: // SET - Settings
	  switch(id_msb<<8|id_lsb){

		case 0x0004: // SET-SOFT-RESET
			halReboot();
		break;

		case 0x0005: // EUID-QUERY

			// copy EUI64 into EUI placeholder array
			memcpy(EUI, emberEui64()->bytes, EUI64_SIZE);

			responseCode = EMBER_COAP_CODE_205_CONTENT;
			payloadResponse(info, responseCode, EUI, 8);

		break;

		case 0x0006: // TX power adjustment

			emberSetRadioPower(payload[6]);
			responseCode = EMBER_COAP_CODE_204_CHANGED;
			nullResponse(info, responseCode);
			emberGetRadioPower();
		break;

		case 0x000A: //set network node type
			changeNodeType(payload[DATA_BIT]);
			responseCode = EMBER_COAP_CODE_204_CHANGED;
			nullResponse(info, responseCode);
		break;

		case 0x000B: //get network node type
			getNodeType();
			responseCode = EMBER_COAP_CODE_203_VALID;
			nullResponse(info, responseCode);
		break;

		//********************************************************************

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

			responseCode = EMBER_COAP_CODE_204_CHANGED;
			nullResponse(info, responseCode);

			break;

		case 0x000D: // SET-MASTER-KEY
			/*Master Key - 16 bytes ( payload 6 - 22)*/
			for(uint8_t i = 0; i < 16; i++){
				master_key_in[i] = payload[i+6];
			}
			VMN_setMasterKey(&master_key_in);
			responseCode = EMBER_COAP_CODE_204_CHANGED;
			nullResponse(info, responseCode);
			break;

		case 0x000E: // SET-NET-SWAP
			responseCode = EMBER_COAP_CODE_204_CHANGED;
			nullResponse(info, responseCode);
			VMN_setJoinMode(payload[DATA_BIT]);
			break;

		case 0x000F: // SET-AQM-POWER-MODE
			setAQMPowerMode(payload[DATA_BIT]);
			responseCode = EMBER_COAP_CODE_204_CHANGED;
			payloadResponse(info, responseCode, getAQMPowerMode(), 1);
			break;

		case 0x0010: // GET-AQM-POWER-MODE
			responseCode = EMBER_COAP_CODE_204_CHANGED;
			payloadResponse(info, responseCode, getAQMPowerMode(), 1);
			break;

		case 0x0011: // SET-RTCC
			responseCode = EMBER_COAP_CODE_204_CHANGED;
			nullResponse(info, responseCode);
			VMN_rtccSetDateTime(payload);
			break;

		case 0x0012: // GET-RTCC
			responseCode = EMBER_COAP_CODE_204_CHANGED;
			nullResponse(info, responseCode);
			VMN_rtccGetDateTime();
			break;

		case 0x0013: // FORCE-OTA
			responseCode = EMBER_COAP_CODE_203_VALID;
			nullResponse(info, responseCode);
			otaBootloadClientForceUpdate();
			break;

		case 0x0014: // GET-FW-VERSION
			//Firmware Image Version (MSB-> LSB)
			fw_version[0]  = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_VERSION >> 24;
			fw_version[1] = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_VERSION >> 16;
			fw_version[2] = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_VERSION >> 8;
			fw_version[3] = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_VERSION;

			//Firmware Image Type (MSB -> LSB)

			fw_version[4] = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_TYPE >> 8;
			fw_version[5] = EMBER_AF_PLUGIN_OTA_BOOTLOAD_CLIENT_POLICY_CURRENT_IMAGE_TYPE;
			responseCode = EMBER_COAP_CODE_205_CONTENT;
			payloadResponse(info, responseCode, fw_version, 6);
			break;

	  }
//************************************************************
	  break;


  case 0x03:  // CMD - Operational Commands
	  emberAfAppPrintln("CLASS 3");
#ifdef NODE_TYPE_SENSOR
	  switch(id_msb){

	  case 0x01: // Particulate control
		  switch(id_lsb){

		  case 0x06: // SET-PURGE-LIMIT
			  setAqmPurgeLimit(payload[DATA_BIT]);
		  break;

		  case 0x07: // GET-PURGE-LIMIT
				responseCode = EMBER_COAP_CODE_204_CHANGED;
				payloadResponse(info, responseCode, getAqmPurgeLimit(), 1);
		  break;

		  default:
		  break;
		  }
	  break; // Particulate Break

	  case 0x02: //si7021 CONTROL

		  switch(id_lsb){

		  case 0x00: // si7021 START
				CORE_si7021Start();
				responseCode = EMBER_COAP_CODE_204_CHANGED;
				nullResponse(info, responseCode);
			  break;

		  case 0x01: // si7021 STOP
			  	CORE_si7021Stop();
				responseCode = EMBER_COAP_CODE_204_CHANGED;
				nullResponse(info, responseCode);
			  break;

		  case 0x02: //si7021 single
			    CORE_si7021singleMeasurement();
				responseCode = EMBER_COAP_CODE_204_CHANGED;
				nullResponse(info, responseCode);
			  break;

		  case 0x03: //si7021 set interval
				temp =  ((payload[INTERVAL_BYTE1] << 24) |
							 (payload[INTERVAL_BYTE2] << 16) |
							 (payload[INTERVAL_BYTE3] <<  8) |
							  payload[INTERVAL_BYTE4]);
				return_temp = CORE_si7021setInterval(temp);

				interval_payload[0] = return_temp >> 24;
				interval_payload[1] = return_temp >> 16;
				interval_payload[2] = return_temp >> 8;
				interval_payload[3] = return_temp;
				responseCode = EMBER_COAP_CODE_204_CHANGED;
				payloadResponse(info, responseCode, interval_payload, 4);
			  break;

		  default:
			  break;

		  }

		  break;

	  case 0x04: // GAS CONTROL
		  switch(id_lsb){

		  case 0x05: //ADC GET GAIN
//			  response_payload[3] = 0x00;
//			  response_payload[4] = 0x14;
//			  response_payload[5] = 1;
//			  response_payload[6] = getADCGain();
//			  responseCode = EMBER_COAP_CODE_205_CONTENT;
//			  payloadResponse(info, responseCode, response_payload , 12);
			  responseCode = EMBER_COAP_CODE_205_CONTENT;
			  payloadResponse(info, responseCode, getADCGain(), 1);
			  break;

		  case 0x06: //ADC SET GAIN
			  setADCGain(payload[6],payload[7], 2);

//			  response_payload[3] = 0x00;
//			  response_payload[4] = 0x14;
//			  response_payload[5] = 1;
//			  response_payload[6] = getADCGain();

			  responseCode = EMBER_COAP_CODE_204_CHANGED;
			  payloadResponse(info, responseCode, getADCGain(), 1);
			  break;

		  case 0x07: //SET PURGE FAN SPEED
			  data_bit = payload[DATA_BIT];
			  response_payload[3] = 0x00;
			  response_payload[4] = 0x17;
			  response_payload[5] = 1;
			  response_payload[6] = setGasPurgeFanSpeed(data_bit);
			  responseCode = EMBER_COAP_CODE_204_CHANGED;
			  payloadResponse(info, responseCode,response_payload , 12);
			  break;

		  case 0x08: //SET PURGE FAN DURATION
				temp =  ((payload[INTERVAL_BYTE1] << 24) |
							 (payload[INTERVAL_BYTE2] << 16) |
							 (payload[INTERVAL_BYTE3] <<  8) |
							  payload[INTERVAL_BYTE4]);
				return_temp = setGasPurgeDuration(temp);

				response_payload[3] = 0x00;
				response_payload[4] = 0x18;
				response_payload[5] = 4;
				response_payload[6] = return_temp >> 24;
				response_payload[7] = return_temp >> 16;
				response_payload[8] = return_temp >> 8;
				response_payload[9] = return_temp;

				responseCode = EMBER_COAP_CODE_204_CHANGED;
				payloadResponse(info, responseCode, response_payload, 4);
			  break;

		  case 0x09: //SET POST PURGE DELAY
				temp =  ((payload[INTERVAL_BYTE1] << 24) |
							 (payload[INTERVAL_BYTE2] << 16) |
							 (payload[INTERVAL_BYTE3] <<  8) |
							  payload[INTERVAL_BYTE4]);

			    return_temp = setGasPostPurgeDelay(temp);

			    interval_payload[0] = return_temp >> 24;
			  	interval_payload[1] = return_temp >> 16;
			  	interval_payload[2] = return_temp >> 8;
			  	interval_payload[3] = return_temp;
			  	responseCode = EMBER_COAP_CODE_204_CHANGED;
			  	payloadResponse(info, responseCode, interval_payload, 4);
			  break;

		  default:
			  break;
		  }
	  break;//GAS CONTROL


		case 0x05: // AQM MASTER
			switch(id_lsb){
			case 0x00: // Master Start
				airQualityMasterStart();
				responseCode = EMBER_COAP_CODE_204_CHANGED;
				nullResponse(info, responseCode);
			break;

			case 0x01: // Master Stop
				airQualityMasterStop();
				responseCode = EMBER_COAP_CODE_204_CHANGED;
				nullResponse(info, responseCode);
			break;

			case 0x02: // Is running
				responseCode = EMBER_COAP_CODE_205_CONTENT;
				payloadResponse(info, responseCode, airQualityMasterIsRunning(), 1);
			break;

			case 0x03: // Get Master Interval
				temp = getAirQualityMasterInterval();
				interval_payload[0] = temp >> 24;
				interval_payload[1] = temp >> 16;
				interval_payload[2] = temp >> 8;
				interval_payload[3] = temp;

				responseCode = EMBER_COAP_CODE_205_CONTENT;
				payloadResponse(info, responseCode, interval_payload, 4);
			break;

			case 0x04: // Set Master Interval
				temp =  ((payload[INTERVAL_BYTE1] << 24) |
							 (payload[INTERVAL_BYTE2] << 16) |
							 (payload[INTERVAL_BYTE3] <<  8) |
							  payload[INTERVAL_BYTE4]);
				setAirQualityMasterInterval(temp);

				temp = getAirQualityMasterInterval();
				interval_payload[0] = temp >> 24;
				interval_payload[1] = temp >> 16;
				interval_payload[2] = temp >> 8;
				interval_payload[3] = temp;

				responseCode = EMBER_COAP_CODE_204_CHANGED;
				payloadResponse(info, responseCode, interval_payload, 4);
			break;

			case 0x05:
				bufferQuery();
			break;

			default:
				break;
			}
		break; // AQM MASTER
			default:
				break;
	  }
#endif

	  break; // 0x03 CMD


  case 0x04:
	  emberAfAppPrintln("CLASS 4");
	  break;

  default:
	  responseCode = EMBER_COAP_CODE_400_BAD_REQUEST;
  }
//
//
//  emberAfAppPrintln("VMN-COAP RX - p0 = %X : p1 = %X", payload[0], payload[1]);
//  if (payload[0] != 0x56 && payload[1] != 0xD7) {
//    responseCode = EMBER_COAP_CODE_400_BAD_REQUEST;
//  } else {

//	emberAfAppPrint("Source=");
//	emberAfPrintIpv6Address(source);
//	emberAfAppPrint(",");
//
//
//	  //*****************************************************
//
////	  for(int i = 0; i < payloadLength; i++){
////		  emberAfAppPrint("%X",payload[i]);
////	  }
////	  emberAfAppPrintln("");
//
//    emberAfCorePrint("Received message: class %X : id %X%X from -", payload[2], payload[3], payload[4]);
//    emberAfCoreDebugExec(emberAfPrintIpv6Address(&info->remoteAddress));
//    emberAfCorePrintln("");
//    responseCode = EMBER_COAP_CODE_204_CHANGED;
//  }
//
//  if (emberCoapIsSuccessResponse(responseCode)
//      || info->localAddress.bytes[0] != 0xFF) { // not multicast
//	    emberCoapRespondWithCode(info, responseCode);
//  }
//
//	switch(message_class) {
//
//		case 0x01: // ACK - Acknowledgement messages
//			if(payload[ID_LSB] == 0x01){
//				emberAfAppPrintln("ACK Received - %x:%2x", payload[6], (payload[7]<<8 | payload[8]));
//				ackCheck(payload);
//			} else {
//				emberAfAppPrintln("Message Failure");
//			}
//			break;
//
//		case 0x02: // SET - Network, device or configuration messages
//			if(SET_commandCheck(payload, source)){
//				sendAck(source, payload);
//			} else {
//				sendNak(source, payload);
//			}
//			break;
//
//		case 0x03: // CMD - Operational Commands
//#ifdef NODE_TYPE_SENSOR
//			if(CMD_commandCheck(payload)){
////				sendAck(source, payload, info, responseCode);
//				break;
//			} else {
//				sendNak(source, payload);
//			}
//#endif
//			break;
//
//		case 0x04: // DAT - Sensor Data
//#ifndef NODE_TYPE_SENSOR
//			DAT_decoder(payload);
//#endif
//			break;
//
//		case 0x05: // MON - Device and Network Monitoring Data
//			VMN_startNetworkScan();
//			break;
//
//		case 0x06: // INF - Information Messages
//
//			break;
//
//		case 0x07: // UPD - Updates
//
//			break;
//
//	}

//	uint8_t reply_payload[] = {payload[0],payload[1],payload[2],payload[3],payload[4]};
//    emberCoapRespondWithPayload(info,responseCode, reply_payload, sizeof(reply_payload));

}

void nullResponse(const EmberCoapRequestInfo *info, EmberCoapCode responseCode){
  if (emberCoapIsSuccessResponse(responseCode)
	  || info->localAddress.bytes[0] != 0xFF) { // not multicast
		emberCoapRespondWithCode(info, responseCode);
  }
}

void payloadResponse(const EmberCoapRequestInfo *info, EmberCoapCode responseCode, uint8_t *payload, uint8_t payload_length){
	emberCoapRespondWithPayload(info,responseCode, payload, payload_length);
}
