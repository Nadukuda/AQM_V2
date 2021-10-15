/*
 * Buffer_Management.c
 *
 *  Created on: 11 Nov 2019
 *      Author: Aidan
 */


#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#include EMBER_AF_API_HAL
#include EMBER_AF_API_COMMAND_INTERPRETER2
#ifdef EMBER_AF_API_DEBUG_PRINT
  #include EMBER_AF_API_DEBUG_PRINT
#endif
#include "Data_Protocol/VMN_globals.h"
#include "Data_Protocol/VMN_definitions.h"
#include "VMN/Data_Queue/Queue.h"

enum {
	INACTIVE = 			0,
	QUERY	 =			1,
	SEND_PM  =			2,
	SEND_GAS =			3,
	TRANSMIT_SUCCESS =	4,
};


EmberEventControl aqmBufferEventControl;
uint8_t aqmBufferState = INACTIVE;
AirQualityData buffer_output;


void bufferQuery(void);

static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs);
#define setNextState(nextState)			setNextStateWithDelay((nextState), 0)
#define repeatState()					setNextStateWithDelay(aqmBufferState, 0)
#define repeatStateWithDelay(delayMs)	setNextStateWithDelay(aqmBufferState, (delayMs))


void aqmBufferEventHandler(void){
	emberEventControlSetInactive(aqmBufferEventControl);

	switch (aqmBufferState) {
	case INACTIVE:

	break;

	case QUERY:
		if(queueIsEmpty() == 1){
			VMN_payloadPackager(&global_server, 0x00, 1, DAT, 0x0027); // empty buffer message
			setNextState(INACTIVE);
		} else {
			getFront(&buffer_output);
			setNextState(SEND_PM);
		}
	break;

	case SEND_PM:
		memcpy(pm_payload,buffer_output.pm_data,20);
		pm_payload[20] = buffer_output.time >> 24;
		pm_payload[21] = buffer_output.time >> 16;
		pm_payload[22] = buffer_output.time >> 8;
		pm_payload[23] = buffer_output.time;

		pm_payload[24] = buffer_output.date >> 24;
		pm_payload[25] = buffer_output.date >> 16;
		pm_payload[26] = buffer_output.date >> 8;
		pm_payload[27] = buffer_output.date;

		VMN_payloadPackager(&global_server, pm_payload, 28, DAT, 0x0025); // buffered PM data message
	break;

	case SEND_GAS:
		memcpy(gas_payload, buffer_output.gas_data, 66);
		gas_payload[66] = buffer_output.time >> 24;
		gas_payload[67] = buffer_output.time >> 16;
		gas_payload[68] = buffer_output.time >> 8;
		gas_payload[69] = buffer_output.time;

		gas_payload[70] = buffer_output.date >> 24;
		gas_payload[71] = buffer_output.date >> 16;
		gas_payload[72] = buffer_output.date >> 8;
		gas_payload[73] = buffer_output.date;
		//VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0026); // buffered GAS data message
	/*	if(SENSOR_SELECT_FLAG == 1)
			VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0033); // Buffered GAS data of NO2/SO2 sensor will send through this 0x0033 command and added this on 21/09/2021
		if(SENSOR_SELECT_FLAG == 2)
			VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0031); // Buffered GAS data of NO2/O3 sensor will send through this 0x0031 command and added this on 21/09/2021
	*/
		switch(SENSOR_SELECT_FLAG) {
			case 1: // Added this new command on 21/09/2021 to send NO2/SO2 buffered GAS data through 0x0033 message id
				VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0033);
			break;
			case 2: // Added this new command on 21/09/2021 to send NO2/O3 buffered GAS data through 0x0031 message id
				VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0031);
			break;
			default: //default device is built for NO2/O3
				VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0031);
		}
		break;

	case TRANSMIT_SUCCESS:
		Dequeue();
		setNextState(QUERY);
	break;

	default:
	break;
	}
}

void bufferQuery(void){
	setNextState(QUERY);
}

void remoteBufferStateControl(uint8_t state_in){
	if(state_in >= 0 && state_in < 5){
		setNextState(state_in);
	}
}


static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs)
{
  aqmBufferState = nextState;
  emberEventControlSetDelayMS(aqmBufferEventControl, delayMs);
}
