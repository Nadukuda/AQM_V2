/*
 * Air-Quality-Control.c
 *
 *  Created on: 9 Apr 2019
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

#define NOVA_AQM // global AQM
#define SDS011 // PM2.5 & PM10

enum {
	INACTIVE			=   0,
	ACTIVE				=   1,
	AQM					=   2,
	GAS					=	3,
	PURGE				=	4,
	TRANSMIT_PM			=	5,
	TRANSMIT_GAS		= 	6,
	ENQUEUE_DATA		=	7,
};


// interval between scheduled AQM measurements in milliseconds.
static long master_interval = 259000;// was 300000; // 5 min this was changed to get the timestamp as 5min instead of 5m 41s 29/09/2021

static boolean repeat = false;
static uint8_t purge_limit = 20;
static uint8_t measurement_count = 0;
static uint64_t date_time;
EmberEventControl aqmMasterEventControl;

uint8_t aqmMasterState = INACTIVE;

AirQualityData air_quality_temp;
uint32_t measurement_index;
boolean transmission_flag = false;


//void airQualityMasterStart(void);
void airQualityMasterStop(void);
boolean airQualityMasterIsRunning(void);
uint32_t getAirQualityMasterInterval(void);
//void setAirQualityMasterInterval(long interval_in);
void remoteGasStart(void);
void remoteTransmitPmStart(void);
void remoteTransmitGasStart(void);
void remoteEnqueueStart(void);
void pmDataLog(uint8_t *pm_data_in);
void pmDataLog(uint8_t *gas_data_in);
void setTransmissionFlag(boolean flag_in);
boolean getTransmissionFlag(void);
boolean aqmIsActive(void);

static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs);
#define setNextState(nextState)			setNextStateWithDelay((nextState), 0)
#define repeatState()					setNextStateWithDelay(aqmMasterState, 0)
#define repeatStateWithDelay(delayMs)	setNextStateWithDelay(aqmMasterState, (delayMs))

uint8_t PM_datasend_control=0;
uint8_t sensorcount = 1;
extern void ADCDataPackaging(uint8_t sensorcount);

uint8_t sensor_notconnected(uint8_t sensornum)
{
	switch(sensornum)
	{
		case O3: gas_payload[74] = 	((O3 << 4) | 	( 0x01 << 2) ); break;
		case SO2: gas_payload[74] = ((SO2 << 4) | ( 0x01 << 2) ); break;
		case NO2: gas_payload[74] = ((NO2 << 4) | ( 0x01 << 2) ); break;
		case NO: gas_payload[74] = ((NO << 4) | 	( 0x01 << 2) ); break;
		case CO: gas_payload[74] = ((CO << 4) | 	( 0x01 << 2) ); break;
		case H2S: gas_payload[74] = ((H2S << 4) | ( 0x01 << 2) ); break;
		case NH3: gas_payload[74] = ((NH3 << 4) | ( 0x01 << 2) ); break;
	}
}

uint8_t sensor_connected(uint8_t sensornum)
{
	switch(sensornum)
	{
		case 1: gas_payload[74] = ( (O3 << 4) | (0x10 << 2) ); break;
		case 2: gas_payload[74] =  ((SO2 << 4) |  (0x10 << 2) ); break;
		case 3: gas_payload[74] = ( (NO2 << 4) |  (0x10 << 2) ); break;
		case 4: gas_payload[74] = ( (NO << 4) |  (0x10 << 2) ); break;
		case 5: gas_payload[74] = ( (CO << 4) |  (0x10 << 2) ); break;
		case 6: gas_payload[74] = ( (H2S << 4) |  (0x10 << 2) ); break;
		case 7: gas_payload[74] = ( (NH3 << 4) | ( 0x10 << 2) ); break;
	}
}

void aqmMasterEventHandler(void){
	emberEventControlSetInactive(aqmMasterEventControl);

	switch (aqmMasterState) {

		case INACTIVE:
			emberAfAppPrintln("INACTIVE");

		break;

		case ACTIVE:
			setFanSpeed(0);
			setNextState(AQM);
		break;

		case AQM:
			setTransmissionFlag(false);
			aqmStart();
			measurement_count ++;
//			setNextStateWithDelay(GAS, 30000);
		break;

		case GAS:
			gasSensorSingle();  //here i have 4 payloads each one is from each sensor
		break;

		case ENQUEUE_DATA:
//			air_quality_temp.index = measurement_index++;
			if(PM_datasend_control == 0) {
				VMN_rtccUpdate();
	//			date_time = VMN_getDateTime();
				air_quality_temp.time = VMN_rtccGetTime();
				air_quality_temp.date = VMN_rtccGetDate();
			}
				Enqueue(air_quality_temp);
		break;

		case TRANSMIT_PM:
			if( PM_datasend_control == 0) {
				memcpy(pm_payload,air_quality_temp.pm_data,20);
				pm_payload[20] = air_quality_temp.time >> 24;
				pm_payload[21] = air_quality_temp.time >> 16;	//hours
				pm_payload[22] = air_quality_temp.time >> 8;	//min
				pm_payload[23] = air_quality_temp.time;			//seconds

				pm_payload[24] = air_quality_temp.date >> 24;
				pm_payload[25] = air_quality_temp.date >> 16;	//year
				pm_payload[26] = air_quality_temp.date >> 8;	//month
				pm_payload[27] = air_quality_temp.date;			//day

	//			VMN_payloadPackager(&global_server, buffer_output.pm_data, 24, DAT, 0x0025);
				//PM DATA
				VMN_payloadPackager(&global_server, pm_payload, 28, DAT, 0x001A);
				PM_datasend_control = 1;
				setNextStateWithDelay(TRANSMIT_GAS, 2000);
			} else
				setNextState(TRANSMIT_GAS);

		break;

		case TRANSMIT_GAS:
			memcpy(gas_payload, air_quality_temp.gas_data, 66);
			gas_payload[66] = air_quality_temp.time >> 24;
			gas_payload[67] = air_quality_temp.time >> 16;
			gas_payload[68] = air_quality_temp.time >> 8;
			gas_payload[69] = air_quality_temp.time;

			gas_payload[70] = air_quality_temp.date >> 24;
			gas_payload[71] = air_quality_temp.date >> 16;
			gas_payload[72] = air_quality_temp.date >> 8;
			gas_payload[73] = air_quality_temp.date;
			if(sensorcount <=  firstpopulatedsensor ) {
				sensorcount = firstpopulatedsensor;
				switch(sensorcount) {
					case 1:
						gas_payload[74] = (sensor_connected(CONNECTED_S1) | 0x00); break;
					case 2:
						gas_payload[74] = (sensor_connected(CONNECTED_S2) | 0x01); break;
					case 3:
						gas_payload[74] = (sensor_connected(CONNECTED_S3) | 0x02); break;
					case 4:
						gas_payload[74] = (sensor_connected(CONNECTED_S4) | 0x03); break;
			   }
				VMN_payloadPackager(&global_server, gas_payload, 75, DAT, 0x0034);
				emberAfAppPrintln("Sensor ID byte %d", gas_payload[74]);
				sensorcount++;
			}
			if(sensorcount <= 4) {
				uint8_t sensor_type = sensor_countbuffer[sensorcount - 1];
				switch(sensor_type) {
			/*	case 0: //not connected  15/10/2021 Tim told me that if No sensor then No need to send the payload
					switch(sensorcount + 1) {
						 case 1: gas_payload[74] = (sensor_notconnected(CONNECTED_S1) | 0x00); break;
						 case 2: gas_payload[74] = (sensor_notconnected(CONNECTED_S2) | 0x01); break;
						 case 3: gas_payload[74] = (sensor_notconnected(CONNECTED_S3) | 0x02); break;
						 case 4: gas_payload[74] = (sensor_notconnected(CONNECTED_S4) | 0x03); break;
						 default: //has to there something as a default
					}
				break;*/
				case 1:  //connected
					switch(sensorcount) {
					/*	case 1:
							ADCDataPackaging(sensorcount);
							setNextState(ENQUEUE_DATA);
							gas_payload[74] = (sensor_connected(CONNECTED_S1) | 0x00);
							VMN_payloadPackager(&global_server, gas_payload, 75, DAT, 0x0034);
							break;*/
						case 2:
							ADCDataPackaging(sensorcount);
							setNextState(ENQUEUE_DATA);
							gas_payload[74] = (sensor_connected(CONNECTED_S2) | 0x01);
							VMN_payloadPackager(&global_server, gas_payload, 75, DAT, 0x0034);
							break;
						case 3:
							ADCDataPackaging(sensorcount);
							setNextState(ENQUEUE_DATA);
							gas_payload[74] = (sensor_connected(CONNECTED_S3) | 0x02);
							VMN_payloadPackager(&global_server, gas_payload, 75, DAT, 0x0034);
							break;
						case 4:
							ADCDataPackaging(sensorcount);
							setNextState(ENQUEUE_DATA);
							gas_payload[74] = (sensor_connected(CONNECTED_S4) | 0x03);
							VMN_payloadPackager(&global_server, gas_payload, 75, DAT, 0x0034);
							break;
					}

				break;
			  }
				sensorcount++;
				emberAfAppPrintln("Sensor ID byte %d", gas_payload[74]);
			}
			if(sensorcount > 4) {
				PM_datasend_control = 1;
				sensorcount = 1;
			}
			//Live GAS DATA send through
		/*	switch(SENSOR_SELECT_FLAG) {
			case 1: //added on 20/09/2021  - NO2 / SO2 version sends raw temp/hum measurements, works on si7021 or htu21d
				VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0032);
				break;
			case 2: //added on 20/09/2021  - NO2 / O3 standard version sends raw temp/hum measurements, works on si7021 or htu21d
				VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0029);
				break;
			default: //default device set to NO2/O3 added on 20/09/2021  - to sends raw temp/hum measurements, works on si7021 or htu21d of NO2/O3
				VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0029);
				break;

			}*/
		//	VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0016); si7021 version
	/*		if(SENSOR_SELECT_FLAG == 1) // set this flag in VMN_Definition.h
				VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0032); //added on 20/09/2021  - NO2 / SO2 version sends raw temp / hum measurements, works on si7021 or htu21d
			else
			VMN_payloadPackager(&global_server, gas_payload, 74, DAT, 0x0029); //added on 20/09/2021  - NO2 / O3 standard version sends raw temp / hum measurements, works on si7021 or htu21d
*/
			setNextState(PURGE);
		break;

		case PURGE:
			if(!low_power_mode && purge_limit > 0){
				if(measurement_count >= purge_limit){
					vortexAqmPurge(0);
					measurement_count = 0;
				}
			}

			if(repeat){
				setNextStateWithDelay(ACTIVE, master_interval);
			} else {
				setNextState(INACTIVE);
			}
		break;
	}
}

void remotePurgeStart(void){
	setNextState(PURGE);
}

void remoteEnqueueStart(void){
	setNextState(ENQUEUE_DATA);
}

void remoteTransmitPmStart(void){
	setNextState(TRANSMIT_PM);
}

void remoteTransmitGasStart(void){
	setNextState(TRANSMIT_GAS);
}

void remoteGasStart(void){
	setNextState(GAS);
}

void airQualityMasterStart(void){
	repeat = true;
	setNextState(ACTIVE);
}

void airQualityMasterStop(void){
	repeat = false;
	setNextState(INACTIVE);
}

boolean aqmIsActive(void){
	return repeat;
}

boolean airQualityMasterIsRunning(void){
//	uint8_t p_load[] = {repeat};
	return repeat;
//	VMN_payloadPackager(&global_server, p_load, sizeof(p_load), DAT, 0x0013);
}

uint32_t getAirQualityMasterInterval(void){
	uint8_t p_load[] = {(master_interval >> 24),
						(master_interval >> 16),
						(master_interval >> 8),
						 master_interval,
	};
//	emberAfAppPrintln("Sending master interval");
	return master_interval;
//	VMN_payloadPackager(&global_server, p_load, sizeof(p_load), DAT, 0x0012);
}

void setAirQualityMasterInterval(long interval_in){
	master_interval = interval_in;
	getAirQualityMasterInterval();
}

void setAqmPurgeLimit(uint8_t purge_limit_in){
	purge_limit = purge_limit_in;
}
//void setAqmVerificationLimit(uint8_t verification_limit_in){
//	verification_limit = verification_limit_in;
//}

uint8_t getAqmPurgeLimit(void){
	return purge_limit;
//	uint8_t p_load[] = {purge_limit};
//	VMN_payloadPackager(&global_server, p_load, sizeof(p_load), DAT, 0x0007);
}

void setAQMPowerMode(uint8_t power_mode_in){

	if(power_mode_in != low_power_mode){
		if(power_mode_in == 1){ 		// low power mode

			Solenoid(0,1);//open solenoids
			Solenoid(1,1);
			//increase heartbeat interval
			setHeartbeatInterval(300000);
			// air quality master interval increase
			setAirQualityMasterInterval(600000);
			//disable USART0
//			USART_Enable(USART0, usartDisable);
			low_power_mode = true;
		} else if(power_mode_in == 0){	// Standard Mode
			Solenoid(0,0);
			Solenoid(1,0);
			setHeartbeatInterval(60000);
//			USART_Enable(USART0, usartEnable);
			low_power_mode = false;
		}
	}
//	getAQMPowerMode();
}

uint8_t getAQMPowerMode(void){
	return low_power_mode;
//	uint8_t p_load[1] = {low_power_mode};
//	VMN_payloadPackager(&global_server, p_load, sizeof(p_load), DAT, 0x0024);
}

void pmDataLog(uint8_t *pm_data_in){
	memcpy(air_quality_temp.pm_data, pm_data_in, 20);
}

void gasDataLog(uint8_t *gas_data_in){
	memcpy(air_quality_temp.gas_data, gas_data_in, 66);
}

void setTransmissionFlag(boolean flag_in){
	transmission_flag = flag_in;
	emberAfAppPrintln("Transmission Flag = %d", transmission_flag);
}

boolean getTransmissionFlag(void){
	return transmission_flag;
}

static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs)
{
  aqmMasterState = nextState;
  emberEventControlSetDelayMS(aqmMasterEventControl, delayMs);
}
