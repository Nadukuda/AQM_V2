/*
 * VMN_COAP.c
 *
 *  Created on: 31 Mar 2020
 *      Author: AidanFyans
 */


#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#include EMBER_AF_API_HAL
#include EMBER_AF_API_COMMAND_INTERPRETER2
#include "VMN_definitions.h"

static uint8_t MESSAGE_URI[3] = {'v','m','n'};

void transmitPayload(uint8_t *payload, uint8_t payload_length, uint8_t *destination);

static void processVMNDataAck(EmberCoapStatus status,
						EmberCoapCode code,
						EmberCoapReadOptions *options,
						uint8_t *payload,
						uint16_t payloadLength,
						EmberCoapResponseInfo *info);

void transmitPayload(uint8_t *payload, uint8_t payload_length, uint8_t *destination)
{

  EmberCoapSendInfo info = { 0 }; // use defaults
  EmberStatus status;

  status = emberCoapPost(destination,
                         MESSAGE_URI,
                         (const uint8_t *)payload, //&
                         payload_length,
                         processVMNDataAck,
                         &info);
  if (status == EMBER_SUCCESS) {
	  return;
  } else {
    emberAfCorePrintln("ERR: Transmission failed: 0x%x", status);
  }
}



static void processVMNDataAck(EmberCoapStatus status,
						EmberCoapCode code,
						EmberCoapReadOptions *options,
						uint8_t *payload,
						uint16_t payloadLength,
						EmberCoapResponseInfo *info){

	if (status == EMBER_COAP_MESSAGE_ACKED
		|| status == EMBER_COAP_MESSAGE_RESPONSE) {
		emberAfAppPrintln("VMN ACK Received - %x:%x%x", payload[2], payload[3], payload[4]);
		if(payload[2] == 0x04){
			switch(payload[3] << 8 | payload[4]){
			case 0x001A:
				setTransmissionFlag(true);
			break;

			case 0x0029: case 0x0032: //0x0029 for O3, 0x0032 for SO2
				if(getTransmissionFlag() == true){
					dequeueRear();
					setTransmissionFlag(false);
				}
			break;

			case 0x0025:
				emberAfAppPrintln("BUFFER - PM Data ACK");
				remoteBufferStateControl(3);
			break;
			//added this on 21/09/2021 to send buffered data through new commands 0x0031 for O3 & 0x0033 for SO2. 0x0026 is the previous command used to send converted temp/hum values
			case 0x0026: case 0x0031: case 0x0033:
				emberAfAppPrintln("BUFFER - GAS Data ACK");
				remoteBufferStateControl(4);
			break;
			default:
			break;

			}
		}


	} else {
		emberAfCorePrintln("ERR: No Response From Device");
	}
}
