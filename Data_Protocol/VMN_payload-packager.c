/*
 * VMN.c
 *
 *  Created on: 27 Nov 2018
 *      Author: Aidan
 */


#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#include EMBER_AF_API_HAL
#include EMBER_AF_API_COMMAND_INTERPRETER2
#include "VMN_definitions.h"

static const uint8_t MESSAGE_URI[] = "VMN/";

void VMN_payloadPackager(uint8_t *destination,
						 uint8_t *payload_in,
						 int payload_length,
						 uint8_t message_class,
						 uint16_t message_ID);

uint16_t checksumGenerator(uint8_t *input, int seq_length);

//static void transmitPayload(uint8_t *payload, uint8_t payload_length, uint8_t *destination);
//
//static void processVMNDataAck(EmberCoapStatus status,
//						EmberCoapCode code,
//						EmberCoapReadOptions *options,
//						uint8_t *payload,
//						uint16_t payloadLength,
//						EmberCoapResponseInfo *info);


/**Aidan*
 *
 * Takes a payload input, uses it to create checksums before rolling it
 * up into a single payload ready for transmission.
 *
 */
void VMN_payloadPackager(uint8_t *destination,
						 uint8_t *payload_in,
						 int payload_length,
						 uint8_t message_class,
						 uint16_t message_ID){

	uint8_t package[payload_length + 6]; //payload + class byte + id byte(msb) + id byte(lsb) + payload_length
	uint16_t checksum;

	uint8_t payload_out[payload_length + 8];  // package + checksum (LSB and MSB)

	/**Aidan*
	 *  Puts message class and message id at the front of the payload package
	 */
	package[SYNC1] = 0x56; // V
	package[SYNC2] = 0xD7; // x
	package[CLASS] = message_class;
	package[ID_MSB] = (uint8_t)(message_ID >> 8);
	package[ID_LSB] = (uint8_t)message_ID;
	package[PAYLOAD_LENGTH] = (uint8_t)payload_length;


	// adds payload after Class and ID
	for(int i = 0; i < payload_length; i++){
		package[i+6] = payload_in[/*(payload_length-1) - */i];
	}

	// Send entire payload for checksum generation
	checksum = checksumGenerator(package, sizeof(package));

	// package payload into new array with space for checksums
	memcpy(payload_out, package, sizeof(package));

	// add checksums to end of payload
	payload_out[sizeof(payload_out)-2] = (uint8_t)(checksum >> 8);
	payload_out[sizeof(payload_out)-1] = (uint8_t)(checksum);

//	emberAfAppPrintln("START OF TRANSMITTED MESSAGE");
//
//	  for(int i = 0; i < sizeof(payload_out)+1; i++){
//		  emberAfAppPrint("%X",payload_out[i]);
//	  }
//	  emberAfAppPrintln("payload size = %d", sizeof(payload_out));
//	  emberAfAppPrintln("END");

	// transmit payload to destination
//	emberSendUdp(destination, 0,0, &payload_out, sizeof(payload_out));
	transmitPayload(payload_out, payload_length+8, destination);


}
//
//static void transmitPayload(uint8_t *payload, uint8_t payload_length, uint8_t *destination)
//{
//
//  EmberCoapSendInfo info = { 0 }; // use defaults
//  EmberStatus status;
//
//  for(int i = 0; i < sizeof(payload)+1; i++){
//	  emberAfAppPrint("%X ",payload[i]);
//  }
//  emberAfAppPrintln("payload size - %X", sizeof(payload));
//  emberAfAppPrintln("");
//
//  status = emberCoapPost(destination,
//                         MESSAGE_URI,
//                         (const uint8_t *)payload, //&
//                         payload_length,
//                         processVMNDataAck,
//                         &info);
//  if (status == EMBER_SUCCESS) {
//	  return;
//  } else {
//    emberAfCorePrintln("ERR: Transmission failed: 0x%x", status);
//  }
//}
//
//
//static void processVMNDataAck(EmberCoapStatus status,
//						EmberCoapCode code,
//						EmberCoapReadOptions *options,
//						uint8_t *payload,
//						uint16_t payloadLength,
//						EmberCoapResponseInfo *info){
//
//	    if (status == EMBER_COAP_MESSAGE_ACKED
//	        || status == EMBER_COAP_MESSAGE_RESPONSE) {
//	    	ackCheck(payload);
//	    } else {
//	    	emberAfCorePrintln("ERR: No Ack");
//	    }
//
//}
//
//






