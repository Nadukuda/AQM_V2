/*
 * VMN_DAT.c
 *
 *  Created on: 29 Nov 2018
 *      Author: Aidan
 *
 *      Handles the decoding of DAT
 *
 */


#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#include EMBER_AF_API_HAL
#include EMBER_AF_API_COMMAND_INTERPRETER2
#include "Data_Protocol/VMN_definitions.h"

#ifndef NODE_TYPE_SENSOR

int DAT_decoder(uint8_t *payload);


int DAT_decoder(uint8_t *payload){

	uint16_t message_id = payload[3]<<8 | payload[4];

	switch(message_id){

		case 0x0000: // DAT-LDPVT
			lidar_distance = (payload[DISTANCE_MSB] << 8) | payload[DISTANCE_LSB];
			lidar_velocity = (int8_t)(payload[VELOCITY]);
			emberAfAppPrint("DAT-LDPVT,");
			emberAfAppPrint("State=");
			switch(payload[STATE]) {
				case 0x01:
					emberAfAppPrint("Obstruction");
					break;

				case 0x02:
					emberAfAppPrint("Object L");
					break;

				case 0x03:
					emberAfAppPrint("Object R");
					break;

				case 0x04:
					emberAfAppPrint("Incoming L");
					break;

				case 0x05:
					emberAfAppPrint("Incoming R");
					break;

				case 0x06:
					emberAfAppPrint("Sensor Blocked");
					break;

				case 0x0F:
					emberAfAppPrint("ALL Clear");
					break;
			}
			emberAfAppPrint(",Distance=%d,Velocity=%d", lidar_distance, lidar_velocity);
			emberAfAppPrintln("");

		break;

		case 0x0001: // DAT-SDS011
			pm2_5 = (((payload[PM25_MSB]*256) + payload[PM25_LSB])/10);
			pm10  = (((payload[PM10_MSB]*256) + payload[PM10_LSB])/10);
			emberAfAppPrint("DAT-SDS011,");
			emberAfAppPrint("PM2.5=%d,PM10=%d",pm2_5, pm10);
			emberAfAppPrintln("");
		break;

		case 0x0002: // DAT-SDS198
			pm100  = (((payload[PM100_MSB]*256) + payload[PM100_LSB])/10);
			emberAfAppPrint("DAT-SDS198,");
			emberAfAppPrint("PM100=%d",pm100);
			emberAfAppPrintln("");
		break;

		case 0x0003: // DAT-SI7021
			CONST1 = 175.72;
			CONST2 = 46.85;
			humidity  = ( (125 * ((payload[HUMIDITY_MSB] << 8) | (payload[HUMIDITY_LSB] & 0xFE))) / 65536)-6;
			temperature = ((CONST1*((payload[TEMPERATURE_MSB] << 8) | (payload[TEMPERATURE_LSB] & 0xFC)) ) / 65536)-CONST2;
			emberAfAppPrint("DAT-SI7021,");
			emberAfAppPrint("HUM=%d,",humidity);
			emberAfAppPrint("TEMP=%d,",temperature);
			emberAfAppPrintln("");
		break;

		case 0x0004: // DAT-SPS30

			//***************************************************************
			//*****************PARTICAL MASS*********************************
			//***************************************************************

			//PM1
			data = (payload[MASS_PM1_U1] << 24 |
							 payload[MASS_PM1_U2] << 16 |
							 payload[MASS_PM1_L1] << 8  |
							 payload[MASS_PM1_L2]);

			float_temp = *((float*)&data);
			sps30_pm1_mass = (int)(float_temp*100);

			//***************************************************************
			//PM2.5
			data = (payload[MASS_PM25_U1] << 24 |
					payload[MASS_PM25_U2] << 16 |
					payload[MASS_PM25_L1] << 8  |
					payload[MASS_PM25_L2]);

			float_temp = *((float*)&data);
			sps30_pm25_mass = (int)(float_temp*100);

			//***************************************************************
			//PM4
			data = (payload[MASS_PM4_U1] << 24 |
					payload[MASS_PM4_U2] << 16 |
					payload[MASS_PM4_L1] << 8  |
					payload[MASS_PM4_L2]);

			float_temp = *((float*)&data);
			sps30_pm4_mass = (int)(float_temp*100);

			//***************************************************************
			//PM10
			data = (payload[MASS_PM10_U1] << 24 |
					payload[MASS_PM10_U2] << 16 |
					payload[MASS_PM10_L1] << 8  |
					payload[MASS_PM10_L2]);

			float_temp = *((float*)&data);
			sps30_pm10_mass = (int)(float_temp*100);

			//***************************************************************
			sps30_pm10_mass = sps30_pm10_mass - sps30_pm4_mass;
			sps30_pm4_mass  = sps30_pm4_mass  - sps30_pm25_mass;
			sps30_pm25_mass = sps30_pm25_mass - sps30_pm1_mass;


			//***************************************************************
			//*****************TYPICAL SIZE**********************************
			//***************************************************************



			data = (payload[TYPICAL_SIZE_U1] << 24 |
					payload[TYPICAL_SIZE_U2] << 16 |
					payload[TYPICAL_SIZE_L1] << 8  |
					payload[TYPICAL_SIZE_L2]);

			float_temp = *((float*)&data);
			sps30_typical_size = (int)(float_temp*100);



			//***************************************************************
			//*****************PARTICAL NUMBER*******************************
			//***************************************************************

			//PM0.5
			emberAfAppPrintln("PARTICAL NUMBER");
			data = (payload[NUMBER_PM05_U1] << 24 |
					payload[NUMBER_PM05_U2] << 16 |
					payload[NUMBER_PM05_L1] << 8  |
					payload[NUMBER_PM05_L2]);

			float_temp = *((float*)&data);
			sps30_pm05_num = (int)(float_temp*100);

			//***************************************************************
			//PM1

			data = (payload[NUMBER_PM1_U1] << 24 |
					payload[NUMBER_PM1_U2] << 16 |
					payload[NUMBER_PM1_L1] << 8  |
					payload[NUMBER_PM1_L2]);

			float_temp = *((float*)&data);
			sps30_pm1_num = (int)(float_temp*100);

			//***************************************************************
			//PM2.5

			data = (payload[NUMBER_PM25_U1] << 24 |
					payload[NUMBER_PM25_U2] << 16 |
					payload[NUMBER_PM25_L1] << 8  |
					payload[NUMBER_PM25_L2]);

			float_temp = *((float*)&data);
			sps30_pm25_num = (int)(float_temp*100);

			//***************************************************************
			//PM4

			data = (payload[NUMBER_PM4_U1] << 24 |
					payload[NUMBER_PM4_U2] << 16 |
					payload[NUMBER_PM4_L1] << 8  |
					payload[NUMBER_PM4_L2]);

			float_temp = *((float*)&data);
			sps30_pm4_num = (int)(float_temp*100);

			//***************************************************************
			//PM10

			data = (payload[NUMBER_PM10_U1] << 24 |
					payload[NUMBER_PM10_U2] << 16 |
					payload[NUMBER_PM10_L1] << 8  |
					payload[NUMBER_PM10_L2]);

			float_temp = *((float*)&data);
			sps30_pm10_num = (int)(float_temp*100);

			//***************************************************************
			sps30_pm10_num = sps30_pm10_num - sps30_pm4_num;
			sps30_pm4_num  = sps30_pm4_num  - sps30_pm25_num;
			sps30_pm25_num = sps30_pm25_num - sps30_pm1_num;
			sps30_pm1_num = sps30_pm1_num - sps30_pm05_num;

			emberAfAppPrint("DAT-SPS30,");
			emberAfAppPrint("MASS-PM1=%d,",sps30_pm1_mass);
			emberAfAppPrint("MASS-PM2.5=%d,",sps30_pm25_mass);
			emberAfAppPrint("MASS-PM4=%d,",sps30_pm4_mass);
			emberAfAppPrint("MASS-PM10=%d,",sps30_pm10_mass);
			emberAfAppPrint("NUM-PM0.5=%d,",sps30_pm05_num);
			emberAfAppPrint("NUM-PM1=%d,",sps30_pm1_num);
			emberAfAppPrint("NUM-PM2.5=%d,",sps30_pm25_num);
			emberAfAppPrint("NUM-PM4=%d,",sps30_pm4_num);
			emberAfAppPrint("NUM-PM10=%d,",sps30_pm10_num);
			emberAfAppPrint("TYP=%d,",sps30_typical_size);
			emberAfAppPrintln("");
		break;

	}
	return 1;
}
#endif
