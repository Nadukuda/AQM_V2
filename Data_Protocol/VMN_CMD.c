/*
 * VMN_CMD.c
 *
 *  Created on: 29 Nov 2018
 *      Author: Aidan
 */


#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#include EMBER_AF_API_HAL
#include EMBER_AF_API_COMMAND_INTERPRETER2
#include "VMN_definitions.h"

static long interval;

int CMD_commandCheck(uint8_t *payload);
void LiDAR(uint8_t *payload);
void novaAQM(uint8_t *payload);
void si7021(uint8_t *payload);
void sps30(uint8_t *payload);
void alphaGasSensor(uint8_t *payload);
uint32_t airQualityMaster(uint8_t *payload);

int CMD_commandCheck(uint8_t *payload){
	uint8_t message_id_msb = payload[ID_MSB];
	switch(message_id_msb){
	emberAfAppPrintln("COMMAND CHECK RX");
	#ifdef LIDAR
		case 0x00:
			LiDAR(payload);
		break;
	#endif

	#ifdef NOVA_AQM
		case 0x01:
			novaAQM(payload);
		break;
	#endif

	#ifdef SI7021
		case 0x02:
//			emberAfAppPrintln("SI7021");
			si7021(payload);
			break;
	#endif

	#ifdef SPS30
		case 0x03:
			sps30(payload);
		break;
	#endif

	#ifdef ALPHA_GAS_SENSOR
		case 0x04:
			alphaGasSensor(payload);
		break;
	#endif

	#ifdef AIR_QUALITY_MASTER
		case 0x05:
			airQualityMaster(payload);
		break;
	#endif
	}
	return 1;
}

#ifdef LIDAR
void LiDAR(uint8_t *payload){
	switch (payload[ID_LSB]){

	/**Aidan*
	 * LiDAR start polling for data
	 */
	case 0x00:
		i2cEventLoopStart();
	break;

	/**Aidan*
	 * LiDAR stop polling for data
	 */
	case 0x01:
//		i2cEventLoopStop();
	break;
	}

}
#endif

#ifdef NOVA_AQM
void novaAQM(uint8_t *payload){

	switch(payload[ID_LSB]){

	case 0x00: // CMD-AQM-STARTStart Measurements
		aqmStart();
	break;

	case 0x01: // CMD-AQM-STOP Stop Measurements
		aqmStop();
	break;

	case 0x02: // CMD-AQM-INTVL Set measurement interval

		emberAfAppPrintln("byte 1 = %x", payload[INTERVAL_BYTE1]);
		emberAfAppPrintln("byte 2 = %x", payload[INTERVAL_BYTE2]);
		emberAfAppPrintln("byte 3 = %x", payload[INTERVAL_BYTE3]);
		emberAfAppPrintln("byte 4 = %x", payload[INTERVAL_BYTE4]);

		interval =  ((payload[INTERVAL_BYTE1] << 24) |
					 (payload[INTERVAL_BYTE2] << 16) |
					 (payload[INTERVAL_BYTE3] <<  8) |
					  payload[INTERVAL_BYTE4]);
		/**Aidan*
		 *
		 * old version for taking direct CLI input
		 *
		 * interval =  ( payload[INTERVAL_BYTE1] - '0') +
		 *				((payload[INTERVAL_BYTE2] - '0') * 10) +
		 *				((payload[INTERVAL_BYTE3] - '0') * 100) +
		 *				((payload[INTERVAL_BYTE4] - '0') * 1000);
		 *
		 */
		emberAfAppPrintln("new_val = %d", interval);
		setAqmMeasurementInterval(interval);
	break;

	case 0x03: // CMD-AQM-PURGE
		sensor_select_bit = payload[DATA_BIT];
		emberAfAppPrintln("sensor selection bit RX = %d", sensor_select_bit);
		vortexStandalonePurge(sensor_select_bit);
	break;

	/**Aidan*
	 *
	 * commented out functions are not required for single internal sensor deployment
	 *
	 */
//	case 0x04: // CMD-AQM-LATCH
//		data_bit = payload[DATA_BIT];
//		emberAfAppPrintln("latch bit RX = %d", data_bit);
//		aqmLatch(data_bit);
//	break;

//	case 0x05: // CMD-AQM-VER-LMT
//		data_bit = payload[DATA_BIT];
//		emberAfAppPrintln("verification limit RX = %d", data_bit);
//		setAqmVerificationLimit(data_bit);
//	break;

	case 0x06: // CMD-AQM-PURGE-LMT
		data_bit = payload[DATA_BIT];
		emberAfAppPrintln("purge limit RX = %d", data_bit);
		setAqmPurgeLimit(data_bit);
	break;

	case 0x07: // CMD-GET-PURGE-LMT
		getAqmPurgeLimit();
	break;

//	case 0x08: // CMD-GET-VERIFICATION-LMT
//		getAqmVerificationLimit();
//	break;

	case 0x09: // CMD-GET-AQM-INTVL
		getAqmInterval();
	break;
	}
}
#endif

#ifdef SI7021

void si7021(uint8_t *payload){
	switch (payload[ID_LSB]){

	/**Aidan*
	 * si7021 start measurement cycle
	 */
	case 0x00:
		emberAfAppPrintln("Start");
		CORE_si7021Start();
	break;

	/**Aidan*
	 * si7021 stop measurement cycle
	 */
	case 0x01:
		CORE_si7021Stop();
		break;
	/**Aidan*
	 * si7021 take single measurement
	 */
	case 0x02:
		CORE_si7021singleMeasurement();
		break;

	case 0x03:
		interval =  ((payload[INTERVAL_BYTE1] << 24) |
					 (payload[INTERVAL_BYTE2] << 16) |
					 (payload[INTERVAL_BYTE3] <<  8) |
					  payload[INTERVAL_BYTE4]);
		CORE_si7021setInterval(interval);
	}
}
#endif

#ifdef SPS30
void sps30(uint8_t *payload){
	switch (payload[ID_LSB]){

	/*Aidan*
	 *
	 * Sensirion SPS30 Start Measurement Cycle
	 *
	 */
	case 0x00:
		SPS30StartSequence();
	break;

	/*Aidan*
	 *
	 * Sensirion SPS30 Stop Measurement Cycle
	 *
	 */
	case 0x01:
		SPS30StopSequence();
	break;

	}
}
#endif

#ifdef ALPHA_GAS_SENSOR
void alphaGasSensor(uint8_t *payload){
	switch (payload[ID_LSB]){

				case 0x00:
					gasSensorStart();
				break;

				case 0x01:
					gasSensorStop();
				break;

				case 0x02:
					gasSensorSingle();
				break;

				case 0x03:
					getGasSensorInterval();
				break;

				case 0x04:
					interval =  ((payload[INTERVAL_BYTE1] << 24) |
								 (payload[INTERVAL_BYTE2] << 16) |
								 (payload[INTERVAL_BYTE3] <<  8) |
								  payload[INTERVAL_BYTE4]);
					setGasSensorInterval(interval);
				break;

				case 0x05:
					getADCGain();
				break;

				case 0x06:
					emberAfAppPrintln("adc gain = %d", payload[6], payload[7]);
					setADCGain(payload[6],payload[7], 2);
//					if(sizeof(payload) == 9){
//						data_bit = payload[DATA_BIT];
//						emberAfAppPrintln("adc gain = %d", data_bit);
//						setADCGain(data_bit,0, 1);
//					} else if(sizeof(payload) > 9){
//						setADCGain(payload[6],payload[7], 2);
//					}

				break;

				case 0x07:
					data_bit = payload[DATA_BIT];
					setGasPurgeFanSpeed(data_bit);
				break;

				case 0x08:
					interval =  ((payload[INTERVAL_BYTE1] << 24) |
								 (payload[INTERVAL_BYTE2] << 16) |
								 (payload[INTERVAL_BYTE3] <<  8) |
								  payload[INTERVAL_BYTE4]);
					setGasPurgeDuration(interval);
				break;

				case 0x09:
					interval =  ((payload[INTERVAL_BYTE1] << 24) |
								 (payload[INTERVAL_BYTE2] << 16) |
								 (payload[INTERVAL_BYTE3] <<  8) |
								  payload[INTERVAL_BYTE4]);
					setGasPostPurgeDelay(interval);
				break;
	}
}
#endif

#ifdef AIR_QUALITY_MASTER
uint32_t airQualityMaster(uint8_t *payload){
	switch (payload[ID_LSB]){
//	emberAfAppPrintln("AQM CMD REC");
				case 0x00:
					airQualityMasterStart();
					return NULL;
				break;

				case 0x01:
					airQualityMasterStop();
					return NULL;
				break;

				case 0x02:
					airQualityMasterIsRunning();
					return NULL;
				break;

				case 0x03:
					getAirQualityMasterInterval();
					return NULL;
				break;

				case 0x04:
					interval =  ((payload[INTERVAL_BYTE1] << 24) |
								 (payload[INTERVAL_BYTE2] << 16) |
								 (payload[INTERVAL_BYTE3] <<  8) |
								  payload[INTERVAL_BYTE4]);
					setAirQualityMasterInterval(interval);
				break;

				case 0x05:
					bufferQuery();
				break;
	}
}
#endif
