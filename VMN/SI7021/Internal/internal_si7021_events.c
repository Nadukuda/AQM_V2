/*
 * si7021_events.c
 *
 *  Created on: 30 Oct 2018
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

#include <Data_Protocol/VMN_definitions.h>
#include <Data_Protocol/VMN_globals.h>
#include "em_rtcc.h"
#include "em_cmu.h"

#define SI7021

//void si7021EventHandler(void);
//extern EmberEventControl si7021EventControl;
uint32_t CORE_si7021setInterval(long);



enum {
	INITIAL					= 0,
	REQUEST_MEASUREMENT		= 1,
	CHECK_READY				= 2,
	FETCH_MEASUREMENT		= 3,
	TRANSMIT				= 4,
	SIZE_OF_WINDOW			= 5,
};

static uint8_t CORE_si7021State = INITIAL;
static boolean repeat = false;
static long interval = 30000;
static boolean CORE_gas_measurement;


EmberEventControl CORE_si7021EventControl;
static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs);
#define setNextState(nextState)			setNextStateWithDelay((nextState), 0)
#define repeatState()					setNextStateWithDelay(CORE_si7021State, 0)
#define repeatStateWithDelay(delayMs)	setNextStateWithDelay(CORE_si7021State, (delayMs))

void CORE_si7021Start(void);

uint8_t payload[4];
#define ONE_MS_TICKS 33 //ticks
extern uint32_t  RTCDRV_TicksToMsec(uint64_t ticks);
extern void delay(uint32_t ms);

void CORE_si7021EventHandler(void){
	emberEventControlSetInactive(CORE_si7021EventControl);

	switch(CORE_si7021State) {
	  case INITIAL:
		setNextState(REQUEST_MEASUREMENT);
		break;

	  case REQUEST_MEASUREMENT:
		//  emberAfAppPrintln("REQUEST");
		  CORE_si7021Setup();
		  CORE_si7021RequestHumidity();

		  setNextStateWithDelay(FETCH_MEASUREMENT, 20);
	  break;

	  case FETCH_MEASUREMENT:
		 // emberAfAppPrintln("FETCH");
		  	  CORE_si7021ReadHumidity();
		  	  int8_t calculated_humidity = ( (125 * CORE_si7021getHumidityData() ) / 65536)-6;
			  emberAfAppPrint("RH Data = %d 		", calculated_humidity);
			  emberAfAppPrintln("RAW Data = %d", CORE_si7021getHumidityData());
			  CORE_si7021RequestTemperature();
			  delay(50); // This is added on 05/08/2021 To work with HTU21D(F) RH/T SENSOR - 50ms delay
			  CORE_si7021ReadTemperature();
			  float CONST1 = 175.72;
			  float CONST2 = 46.85;
			  int8_t calculated_temperature = ((CONST1*CORE_si7021getTemperatureData() ) / 65536)-CONST2;
			  emberAfAppPrint("Temp Data = %d 		", calculated_temperature);
			  emberAfAppPrintln("RAW Data = %d", CORE_si7021getTemperatureData());
			  setNextState(TRANSMIT);

		  break;

	  case TRANSMIT:
		  payload[0] = CORE_si7021getHumidityData() >> 8;
		  payload[1] = CORE_si7021getHumidityData();
		  payload[2] = CORE_si7021getTemperatureData() >> 8;
		  payload[3] = CORE_si7021getTemperatureData();
		  VMN_payloadPackager(&global_server, payload, 4, DAT, 0x0030); //shravanthi added this on 20/09/2021 this message ID is to send temp/RH redings for both SO2 and O3

		  if(repeat){
			  setNextStateWithDelay(INITIAL,interval);
		  }
		  break;
	}
}
//added on 16/09/2021 by sharavanthi
/*void CORE_si7021_measurementfetch(void)
{
	  CORE_si7021Setup();
	  CORE_si7021RequestHumidity();
	  delay(20);	//added this to test buffered data 16/09/2021
	  CORE_si7021ReadHumidity();
	  int8_t calculated_humidity = ( (125 * CORE_si7021getHumidityData() ) / 65536)-6;
	  emberAfAppPrint("RH Data = %d 		", calculated_humidity);
	  emberAfAppPrintln("RAW Data = %d", CORE_si7021getHumidityData());
	  CORE_si7021RequestTemperature();
	  delay(50); // This is added on 05/08/2021 To work with HTU21D(F) RH/T SENSOR - 50ms delay
	  CORE_si7021ReadTemperature();
	  float CONST1 = 175.72;
	  float CONST2 = 46.85;
	  int8_t calculated_temperature = ((CONST1*CORE_si7021getTemperatureData() ) / 65536)-CONST2;
	  emberAfAppPrint("Temp Data = %d 		", calculated_temperature);
	  emberAfAppPrintln("RAW Data = %d", CORE_si7021getTemperatureData());
	  CORE_gas_measurement = !CORE_gas_measurement;
}*/

void CORE_si7021Start(void){
	repeat = true;
	setNextState(INITIAL);
}

uint32_t CORE_si7021setInterval(long new_interval){
	interval = new_interval;
	return interval;
}

void CORE_si7021singleMeasurement(void){
	repeat = false;
	setNextState(INITIAL);

}

void CORE_si7021Stop(void){
	repeat = false;
}

static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs)
{
	CORE_si7021State = nextState;
  emberEventControlSetDelayMS(CORE_si7021EventControl, delayMs);
}
