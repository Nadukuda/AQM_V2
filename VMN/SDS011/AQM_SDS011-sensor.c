/*
 * AQM_SDS011-sensor.c
 *
 *
 *  Created on: 14 Nov 2018
 *      Author: Aidan
 *
 *
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
//#include "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.5/platform/base/hal/plugin/serial/com_config.h"
#include "serial/com_config.h"

#define NOVA_AQM // global AQM
#define SDS011 // PM2.5 & PM10

enum {
	INACTIVE								=   0,
	ACTIVE								=   1,
	OPEN_PORT							=   2,
	ACQUIRE_MEASUREMENT					=	3,
	ANALYSE_DATA						=   4,
	TRANSMIT_MEASUREMENT				=	5,
	REPEAT								=   6,
};

// flag to select which sensor is active (0 == main sensor, !0 == backup sensor)
uint8_t sensor_flag = 0;
// variable for controlling port number to be initialised.
uint8_t uart_port_number = COM_USART1;

// COM port initialisation structure.
COM_Init_t initData;

// latch for making sensor 2 the default unit ( if 1 sensor 2 = default )
static uint8_t latch = 0;

// counts number of measurements taken since last purge (sensor 0).
//uint8_t cycle_count_channel_0 = 0;
static uint8_t cycle_count[] = {0,0};

// counts number of measurements taken since last purge (sensor 1).
//uint8_t cycle_count_channel_1 = 0;


// number of measurements allowed between scheduled purges.
static uint8_t purge_limit = 2;
// number of readings taken before a verification measurement is taken
static uint8_t verification_limit = 2;//100;

//number of readings since last verification measurement
static uint8_t verification_count = 0;

// interval between scheduled AQM measurements in milliseconds. this is modified on 04/10/2021 by shravanthi ti test the timestamp between each interval.
// Complete aqm measurement process taking approx 41 to 48 seconds and if we want to schedule the changing this
static long measurement_interval = 259000;// to make it run for 4min 19sec to cover the process time as well within schedule time300000; // 5 min

uint32_t port_watchdog_timestamp; // port watchdog for ensuring system cannot get stuck looking for measurement on usart

#define PORT_WATCHDOG_MS 15000

static uint8_t aqmState = INACTIVE;

// USART RX buffer
const uint8_t BUFFER_SIZE = 10;
uint8_t uart_RX[10] = { 0 };


EmberEventControl aqmEventControl;

uint8_t count = 0;

uint8_t pm_data[20];

static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs);
#define setNextState(nextState)			setNextStateWithDelay((nextState), 0)
#define repeatState()					setNextStateWithDelay(aqmState, 0)
#define repeatStateWithDelay(delayMs)	setNextStateWithDelay(aqmState, (delayMs))
void serialFetch(void);
//void setAqmMeasurementInterval (long interval_in);
void aqmStart(void);
void aqmStop(void);
void aqmContinue(void);
void aqmLatch(uint8_t);
void setAqmPurgeLimit(uint8_t);
void setAqmVerificationLimit(uint8_t);
uint8_t getAqmPurgeLimit(void);
void getAqmVerificationLimit(void);
void getAqmInterval(void);
void aqmDataLog(void);
void aqmTransmit(void);

void aqmEventHandler(void){
	emberEventControlSetInactive(aqmEventControl);

	switch (aqmState) {

		case INACTIVE:
			emberAfAppPrintln("INACTIVE");
			aqmPowerOff();
			setFanSpeed(0);
			break;

		case ACTIVE:
			emberAfAppPrintln("ACTIVE");
			initData = (COM_Init_t) COM_USART1_DEFAULT;
			uart_port_number = COM_USART1;
			aqmPowerOn(0);
			setFanSpeed(0);
			// short delay to allow the aqm to power up and initialise.
			setNextStateWithDelay(OPEN_PORT, 5000);
			break;

		case OPEN_PORT:
//			emberAfAppPrintln("OPEN PORT");
			port_watchdog_timestamp = halCommonGetInt32uMillisecondTick();
			USART_Enable(USART1, usartEnable);
			COM_Init(uart_port_number, &initData);
//			USART_IntEnable(USART1, USART_IEN_RXDATAV);
//			NVIC_EnableIRQ(USART1_RX_IRQn);
//			USART_IntSet(USART1, USART_IFS_TXC);
			setNextStateWithDelay(ACQUIRE_MEASUREMENT, 200);
			break;

		case ACQUIRE_MEASUREMENT:
//			emberAfAppPrintln("MEASUREMENT");
			if((halCommonGetInt32uMillisecondTick() - port_watchdog_timestamp) > PORT_WATCHDOG_MS){

				// notify server that a coms failure has occured
				VMN_payloadPackager(&global_server, 0x00, 1, DAT, 0x0006);
			//	aqmPowerOff();
				setNextStateWithDelay(ACTIVE, 1000);
			} else {
				serialFetch();
			}
			break;

		case REPEAT:
			emberAfAppPrintln("REPEAT");
			serialFetch();
			break;

		case ANALYSE_DATA:

			if(uart_RX[0] ==  0xAA && uart_RX[9] == 0xAB){
				setNextState(TRANSMIT_MEASUREMENT);
			} else {
				emberAfAppPrintln("checksum failure");
				setNextState(ACQUIRE_MEASUREMENT);
			}
			break;

		case TRANSMIT_MEASUREMENT:
			aqmDataLog();
			if(count < 5){
				count++;
				setNextState(OPEN_PORT);
			} else {
				count = 0;
				COM_DeInit(uart_port_number);
				USART_Enable(USART1, usartDisable);
				aqmPowerOff();
//				cycle_count[sensor_flag]++;
//
//				if(cycle_count[sensor_flag] >= purge_limit){
//					cycle_count[sensor_flag] = 0;
//					if(purge_limit > 0){
//						vortexAqmPurge(sensor_flag);
//					}
				pmDataLog(pm_data);
//				aqmTransmit();
				printAqmData();
				remoteGasStart();
				setNextState(INACTIVE);
			}
			setFanSpeed(0);

//			setNextStateWithDelay(ACTIVE, measurement_interval);

			break;
	}
}

void aqmDataLog(void){
	pm_data[count*4+0] = uart_RX[2];
	pm_data[count*4+1] = uart_RX[3];
	pm_data[count*4+2] = uart_RX[4];
	pm_data[count*4+3] = uart_RX[5];
//	emberAfAppPrintln("pm2.5 = %x%x,pm10 = %x%x", uart_RX[3],uart_RX[2],uart_RX[5],uart_RX[4]);
}

void aqmTransmit(void){

//	uint8_t p_load[] = {uart_RX[2], uart_RX[3], uart_RX[4], uart_RX[5], sensor_flag};
//	emberAfAppPrintln("pm2.5 = %x%x   pm10 = %x%x", pm_data[1],pm_data[0],pm_data[3],pm_data[2]);
	printAqmData();
	VMN_payloadPackager(&global_server, pm_data, sizeof(pm_data), DAT, 0x001A);
}

void printAqmData(void){
	emberAfAppPrint("DAT-SDS011,");
	uint8_t sensor_number = 0;
	emberAfAppPrint("SENS=%d,", sensor_number);
	for(uint8_t i = 0; i < 20; i = i + 4){
	pm2_5 = (((pm_data[i+1]*256) + pm_data[i])/10);
	pm10  = (((pm_data[i+3]*256) + pm_data[i+2])/10);
	emberAfAppPrint("ORDER=%d,PM2.5=%d,PM10=%d,",i/4, pm2_5, pm10);
	}
	emberAfAppPrintln("");
}

void aqmStart(void){
	setNextState(ACTIVE);
}

void aqmStop (void){
	setNextState(INACTIVE);
}

void aqmContinue(void){

	setNextStateWithDelay(ACTIVE, measurement_interval);
}

//void aqmLatch(uint8_t latch_in){
//	if(latch_in == 0){
//		latch = 0;
//		sensor_flag = 0;
//	} else {
//		latch = 1;
//		sensor_flag = 1;
//	}
//}

void setAqmMeasurementInterval(long interval_in){
	//measurement_interval = (long)emberSignedCommandArgument(0);
	measurement_interval = interval_in;
	emberAfAppPrintln("INTERVAL SET!!! = %6d", measurement_interval);
}

//void setAqmPurgeLimit(uint8_t purge_limit_in){
//	purge_limit = purge_limit_in;
//}
////void setAqmVerificationLimit(uint8_t verification_limit_in){
////	verification_limit = verification_limit_in;
////}
//
//void getAqmPurgeLimit(void){
//	uint8_t p_load[] = {purge_limit};
//	VMN_payloadPackager(&global_server, p_load, sizeof(p_load), DAT, 0x0007);
//}

//void getAqmVerificationLimit(void){
//	uint8_t p_load[] = {verification_limit};
//	VMN_payloadPackager(&global_server, p_load, sizeof(p_load), DAT, 0x0008);
//}

void getAqmInterval(void){
	uint8_t p_load[] = {(measurement_interval >> 24),
						(measurement_interval >> 16),
						(measurement_interval >> 8),
						 measurement_interval,
	};
	VMN_payloadPackager(&global_server, p_load, sizeof(p_load), DAT, 0x0009);

}

void serialFetch(void){
  if(COM_ReadAvailable(uart_port_number) > 8){
	  //emberAfAppPrintln("available");
	  COM_ReadData(uart_port_number,&uart_RX, BUFFER_SIZE, NULL);

//	  emberAfAppPrintln("%X%X%X%X%X%X%X%X%X%X",
//			  uart_RX[0],
//			  uart_RX[1],
//			  uart_RX[2],
//			  uart_RX[3],
//			  uart_RX[4],
//			  uart_RX[5],
//			  uart_RX[6],
//			  uart_RX[7],
//			  uart_RX[8],
//			  uart_RX[9]
//	  );
	  COM_FlushRx(uart_port_number);
	  setNextState(ANALYSE_DATA);
  } else {
	  setNextState(ACQUIRE_MEASUREMENT);
  }

}


static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs)
{
  aqmState = nextState;
  emberEventControlSetDelayMS(aqmEventControl, delayMs);
}
