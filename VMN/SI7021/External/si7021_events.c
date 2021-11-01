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



enum {
	INITIAL					= 0,
	REQUEST_MEASUREMENT		= 1,
	CHECK_READY				= 2,
	FETCH_MEASUREMENT		= 3,
	TRANSMIT				= 4,
	SIZE_OF_WINDOW			= 5,
};

static uint8_t si7021State = INITIAL;
static boolean repeat = false;
static boolean gas_measurement;
static long interval = 30000;


EmberEventControl si7021EventControl;
static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs);
#define setNextState(nextState)			setNextStateWithDelay((nextState), 0)
#define repeatState()					setNextStateWithDelay(si7021State, 0)
#define repeatStateWithDelay(delayMs)	setNextStateWithDelay(si7021State, (delayMs))
#define TIMEGAP(a,b)	((a) - (b))
void si7021Start(void);
#define ONE_MS_TICKS 33 //ticks
uint8_t payload[4];
extern uint32_t  RTCDRV_TicksToMsec(uint64_t ticks);
extern void errormessagetimestamp(void);
extern uint8_t errormessage_payload[9];
void delay(uint32_t ms)
{
	uint32_t ticks = 33*ms;
	  uint32_t startTime;
	  volatile uint32_t now;

	  if ( ticks ) {
	    startTime = RTCC_CounterGet();
	    do {
	      now = RTCC_CounterGet();
	    } while ( TIMEGAP(now, startTime) < ticks );
	  }
}
void delay_read(uint32_t delaycnt)
{
	RTCDRV_TicksToMsec(delaycnt);
}


void si7021EventHandler(void){
	emberEventControlSetInactive(si7021EventControl);

	switch(si7021State) {
	  case INITIAL:
		setNextState(REQUEST_MEASUREMENT);
		break;

	  case REQUEST_MEASUREMENT:
		//  emberAfAppPrintln("1");
//		  emberAfAppPrintln("REQUEST");
		  si7021Setup();
		  uint8_t error = si7021RequestHumidity();
		  if(error != 0 ){
			  errormessage_payload[0]=15; // If Si7021/HTU21D is not responding
			  errormessagetimestamp();
			  VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
			  emberAfAppPrint(" TEMP/HUM SENSOR Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");
			  setNextState(TRANSMIT);
		  }
		  else
		 // delay(20);	//added this to test buffered data 16/09/2021
		  setNextStateWithDelay(FETCH_MEASUREMENT, 20);  //This was 50 changed to 20 on 05/08/2021
	  break;

	  case FETCH_MEASUREMENT:
		//  emberAfAppPrintln("2");
//		  emberAfAppPrintln("FETCH");
		  si7021ReadHumidity();
		  int8_t calculated_humidity = ( (125 * si7021getHumidityData() ) / 65536)-6;
		  emberAfAppPrint("HUM = %d 		", calculated_humidity);
		  emberAfAppPrintln("RH RAW Data = %d", si7021getHumidityData());
		//  delay(1); // This is added on 05/08/2021 To work with HTU21D(F) RH/T SENSOR - 1ms delay
		  si7021RequestTemperature();
		  delay(50); // This is added on 05/08/2021 To work with HTU21D(F) RH/T SENSOR - 50ms delay
		  si7021ReadTemperature();
		  float CONST1 = 175.72;
		  float CONST2 = 46.85;
		  int8_t calculated_temperature = ((CONST1*si7021getTemperatureData() ) / 65536)-CONST2;
		  emberAfAppPrint("TEMP = %d 		", calculated_temperature);
		  emberAfAppPrintln("Temp RAW Data = %d", si7021getTemperatureData());
		 // temp_reading_flag  = 1;
		  if(gas_measurement){

			  //here these are previous reading but I have to make a change before packaging for 0x0016 command
			  setAirHumidity(si7021getHumidityData()); //sending raw data
			  setAirTemp(si7021getTemperatureData()); //sending raw data
			  gas_measurement = !gas_measurement;
		  } else {
			  setNextState(TRANSMIT);
		  }
		  break;

	  case TRANSMIT:
		  payload[0] = si7021getHumidityData() >> 8;
		  payload[1] = si7021getHumidityData();
		  payload[2] = si7021getTemperatureData() >> 8;
		  payload[3] = si7021getTemperatureData();

		  VMN_payloadPackager(&global_server, payload, 4, DAT, 0x0003);

		  if(repeat){
			  setNextStateWithDelay(INITIAL,interval);
		  }
		  break;
	}
}

//This function reads the temperature/humidity measurements from the measurement registers for both si7021/HTU21D sensors
void temp_humidity_measurementfetch(void)
{
	uint8_t ret;
	  si7021Setup();
	  ret = si7021RequestHumidity();
	  if(ret == 0) { //when transfer is done
		  delay(20);	//added this to test buffered data 16/09/2021
		  si7021ReadHumidity();
		  humidity = si7021getHumidityData();
		  int8_t calculated_humidity = ( (125 * humidity ) / 65536)-6;
		  emberAfAppPrint("HUM = %d 		", calculated_humidity);
		  emberAfAppPrintln("RH RAW Data = %d", humidity);
		  si7021RequestTemperature();
		  delay(50); // This is added on 05/08/2021 To work with HTU21D(F) RH/T SENSOR - 50ms delay
		  si7021ReadTemperature();
		  temperature = si7021getTemperatureData();
		  float CONST1 = 175.72;
		  float CONST2 = 46.85;

		  int8_t calculated_temperature = ((CONST1*temperature ) / 65536)-CONST2;
		  emberAfAppPrint("TEMP = %d 		", calculated_temperature);
		  emberAfAppPrintln("Temp RAW Data = %d", temperature);
		  //here these are previous reading but I have to make a change before packaging for 0x0016 command
		  setAirHumidity(si7021getHumidityData()); //sending raw data
		  setAirTemp(si7021getTemperatureData()); //sending raw data
		/*  if((humidity == 0) && (temperature == 0)) {
			  errormessage_payload[0]=0x0F; // If Si7021/HTU21D is not responding
			  errormessagetimestamp();
			  VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
			  emberAfAppPrint(" TEMP/HUM SENSOR Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");}*/
	} else {
		errormessage_payload[0]=15; // If Si7021/HTU21D is not responding
		errormessagetimestamp();
		VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
		emberAfAppPrint(" TEMP/HUM SENSOR Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");
	}
	  gas_measurement = !gas_measurement;
}

void si7021Start(void){
	repeat = true;
	setNextState(INITIAL);
}

void si7021Gas(void){
	repeat = false;
	gas_measurement = true;
//	emberAfAppPrintln("si7021Gas");
	temp_humidity_measurementfetch();
//	setNextState(INITIAL);
}

void si7021setInterval(long new_interval){
	interval = new_interval;
}

void si7021singleMeasurement(void){
	repeat = false;
	setNextState(INITIAL);

}

void si7021Stop(void){
	repeat = false;
}

static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs)
{
  si7021State = nextState;
  emberEventControlSetDelayMS(si7021EventControl, delayMs);
}
