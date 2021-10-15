/*
 * vortex-aqm-events.c
 *
 *  Created on: 6 Feb 2019
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

#include "Data_Protocol/VMN_definitions.h"
#include "Data_Protocol/VMN_globals.h"

enum {
	INITIAL 						= 0,
	OPEN							= 1,
	PURGE							= 2,
	CLOSE							= 3,
	REPORT							= 4,
};

uint8_t vortexAqmState = INITIAL;
boolean flag = false;
uint8_t valve_id = 0;

void vortexAqmPurge(uint8_t);
void vortexStandalonePurge(uint8_t);

EmberEventControl vortexAqmEventControl;
static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs);
#define setNextState(nextState)			setNextStateWithDelay((nextState), 0)
#define repeatState()					setNextStateWithDelay(vortexAqmState, 0)
#define repeatStateWithDelay(delayMs)	setNextStateWithDelay(vortexAqmState, (delayMs))

void vortexAqmPurge(uint8_t valve_id_in){
	valve_id = valve_id_in;
	setNextState(OPEN);
}

void vortexStandalonePurge(uint8_t valve_id_in){
	valve_id = valve_id_in;
	flag = true;
	setNextState(OPEN);
}

void vortexAqmEventHandler(void){
	emberEventControlSetInactive(vortexAqmEventControl);

	switch (vortexAqmState){

	case INITIAL:
		emberAfAppPrintln("INITIAL");
		setFanSpeed(0);
		break;

	case OPEN:
		Solenoid(valve_id ,1);
		setNextState(PURGE);
		break;

	case PURGE:
		assert(vortexAqmState == PURGE);
		setFanSpeed(100);
		setNextStateWithDelay(CLOSE, 10000);
		break;

	case CLOSE:
		assert(vortexAqmState == CLOSE);
		setFanSpeed(0);
		Solenoid(valve_id,0);
		setNextState(REPORT);
		break;

	case REPORT:
		assert(vortexAqmState == REPORT);
		emberAfAppPrintln("PURGE COMPLETE!");

		// send purge complete message
		VMN_payloadPackager(&global_server, 0x00, 1, DAT, 0x0005);
		if(flag){
			flag = false;
			setNextState(INITIAL);
			break;
		}
		aqmPowerOff();
//		aqmContinue();
		setNextState(INITIAL);
		break;

	break;
	}


}


static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs)
{
  vortexAqmState = nextState;
  emberEventControlSetDelayMS(vortexAqmEventControl, delayMs);
}
