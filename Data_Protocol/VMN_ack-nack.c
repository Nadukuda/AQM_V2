/*
 * VMN_ack-nack.c
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
#include "VMN/Data_Queue/Queue.h"

void sendAck(uint8_t *source, uint8_t *ack_payload, EmberCoapRequestInfo *info, EmberCoapCode responseCode);
void sendNak(uint8_t *source, uint8_t *nak_payload);
void ackCheck(uint8_t *payload);

static uint8_t class;
static uint16_t ID;

void sendAck(uint8_t *source, uint8_t *ack_payload, EmberCoapRequestInfo *info, EmberCoapCode responseCode){

	uint8_t ack_temp[] = {ack_payload[CLASS], ack_payload[ID_MSB], ack_payload[ID_LSB]};

//	VMN_payloadPackager(source,ack_temp,sizeof(ack_temp),ACK, 0x0001);
	emberCoapRespondWithPayload(info,responseCode, ack_temp, sizeof(ack_temp));
}

void sendNak(uint8_t *source, uint8_t *nak_payload){
	uint8_t nak_temp[] = {nak_payload[CLASS], nak_payload[ID_MSB], nak_payload[ID_LSB]};

	VMN_payloadPackager(source,nak_temp,sizeof(nak_temp),ACK,0x0000);
}

void ackCheck(uint8_t *payload){
	class = payload[6];
	ID = (payload[7]<<8 | payload[8]);
	switch(class){
		case 4: // DAT
			switch(ID){
				case 0x001A:
					emberAfAppPrintln("PM Data ACK");
//					remoteTransmitGasStart();
					setTransmissionFlag(true);
				break;

				case 0x0029: case 0x0032: //0x0029 for NO2/O3 gas data ACK and 0x0032 for NO2/O3 gas data ACK
					emberAfAppPrintln("Gas Data ACK");
					if(getTransmissionFlag() == true){
						dequeueRear();
						setTransmissionFlag(false);
						emberAfAppPrintln("DEQUEUE REAR!!!!");
					} else {
						emberAfAppPrintln("DEQUEUE ERROR");
					}

				break;

				case 0x0025:
					emberAfAppPrintln("BUFFER - PM Data ACK");
					remoteBufferStateControl(3);
				break;

				case 0x0026:  case 0x0031: case 0x0033: //This will send the buffered data through 0x0031 (temp/hum raw measurement), 0x0026( converted temp/hum data), 0x0033 ( SO2 temp/hum raw measurement)
					emberAfAppPrintln("BUFFER - GAS Data ACK");
					remoteBufferStateControl(4);
				break;

				default:
				break;
			}
		break;

		default:
		break;
	}
}



