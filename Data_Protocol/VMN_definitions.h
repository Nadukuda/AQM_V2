/*
 * VMN_payload_map.h
 *
 *  Created on: 29 Nov 2018
 *      Author: Aidan
 */

#ifndef DATA_PROTOCOL_VMN_PAYLOAD_MAP_H_
#define DATA_PROTOCOL_VMN_PAYLOAD_MAP_H_

#endif /* DATA_PROTOCOL_VMN_PAYLOAD_MAP_H_ */


//*********************************************************************************
//*********************************************************************************
/**Aidan*
 *
 * UnComment the the node type that is being used for this hardware configuration.
 */
#define NODE_TYPE_SENSOR
//#define NODE_TYPE_SERVER
//#define NODE_TYPE_ROUTER


/**Aidan*
 * Uncomment required sensors
 */

#define SI7021
#define NOVA_AQM
//#define SPS30
//#define LIDAR
#define ALPHA_GAS_SENSOR
#define AIR_QUALITY_MASTER

// add more sensors as required
// 1 = NO2/SO2....etc
// 2 = NO2/O3
#define SENSOR_SELECT_FLAG			2

/* Added newly for Version2 */

#define SOLENOID_OPEN				1
#define SOLENOID_CLOSE				0
#define SOLENOID1					0
#define SOLENOID2					1
#define SOLENOID3					2

#define ADC0						0
#define ADC1						1
#define SENSOR1						1
#define SENSOR2						2
#define SENSOR3						3
#define SENSOR4						4
#define NO_SENSOR_CONNECTED			0

// O3 = 001 SO2 = 002 NO2 = 003 NO = 100 CO = 101 H2S = 110 NH3 = 111
#define O3							1
#define SO2							2
#define NO2							3
#define NO							4
#define CO							5
#define H2S							6
#define NH3							7



/*
			LEFT					RIGHT
 	 	   ---------			  ---------
 	 	 |			|			|			|
 	 	 |	  S1	|			|	 S3		|
 	 	 |			|			|			|
 	 	 | -------- |			| --------  |
 	 	 |			|			|			|
 	 	 |	  S2  	|			|	 S4		|
 	 	 |			|			|			|
 	 	   ---------			  ---------

*/

//Old format message structure works only when CONNECTED_S1 = NO2 and CONNECTED_S3 = O3
// if YES CONNECTED_S1 = NO2 and CONNECTED_S3 = O3 then live data will send through old format+new format and new format data will only be buffered
//if NO measured data sent through new format and data will also be buffered in new format

#define CONNECTED_S1			NO2
#define CONNECTED_S2			O3
#define CONNECTED_S3			O3
#define CONNECTED_S4			CO

//Avaiable gains 1x 2x 4x 8x
/*#define SENSOR1_DEFAULTGAIN		4
#define SENSOR2_DEFAULTGAIN		2
#define SENSOR3_DEFAULTGAIN		2
#define SENSOR4_DEFAULTGAIN		4*/

#define GAINx1					1
#define GAINx2					2
#define GAINx4					4
#define GAINx8					8

static uint8_t sensors_defaultgain[7] = {2,2,2,8,2,1,8};  //Default gains for O3,SO2,NO2,NO,CO,H2S,NH3 respectively

//*********************************************************************************
//*********************************************************************************


enum CLASSES{
	ACK				= 0x01,
	SET				= 0x02,
	CMD				= 0x03,
	DAT				= 0x04,
	MON				= 0x05,
	INF				= 0x06,
	UPD				= 0x07,
};

enum HEADERS{
	SYNC1			= 0,
	SYNC2			= 1,
	CLASS			= 2,
	ID_MSB			= 3,
	ID_LSB			= 4,
	PAYLOAD_LENGTH  = 5,

};

enum INTERVALS{
	INTERVAL_BYTE1		= 6,
	INTERVAL_BYTE2		= 7,
	INTERVAL_BYTE3		= 8,
	INTERVAL_BYTE4		= 9,
};


enum SDS011{
	PM25_LSB		= 6,
	PM25_MSB		= 7,
	PM10_LSB		= 8,
	PM10_MSB		= 9,
	DATA_BIT		= 6,
//	INTERVAL_BYTE1		= 6,
//	INTERVAL_BYTE2		= 7,
//	INTERVAL_BYTE3		= 8,
//	INTERVAL_BYTE4		= 9,
};
int pm2_5;
int pm10;
uint8_t sensor_select_bit;
uint8_t data_bit;



enum SDS198{
	PM100_LSB		= 6,
	PM100_MSB		= 7,
//	INTERVAL_BYTE1		= 6,
//	INTERVAL_BYTE2		= 7,
//	INTERVAL_BYTE3		= 8,
//	INTERVAL_BYTE4		= 9,
};
int pm100;




/**Aidan*
 *
 * LiDAR data
 *
 */
enum LIDAR{
	STATE			= 6,
	DISTANCE_MSB	= 7,
	DISTANCE_LSB	= 8,
	VELOCITY		= 9,
};

int8_t lidar_velocity;
uint16_t lidar_distance;


enum SI7021{
	HUMIDITY_MSB	= 6,
	HUMIDITY_LSB	= 7,
	TEMPERATURE_MSB	= 8,
	TEMPERATURE_LSB	= 9,
};
float CONST1;
float CONST2;
int temperature;
int humidity;


/**Aidan*
 *
 * Sensirion SPS30 particulate matter sensor
 *
 */

enum SPS30{

	MASS_PM1_U1					= 6,
	MASS_PM1_U2					= 7,
	MASS_PM1_L1					= 8,
	MASS_PM1_L2					= 9,

	MASS_PM25_U1				= 10,
	MASS_PM25_U2				= 11,
	MASS_PM25_L1				= 12,
	MASS_PM25_L2				= 13,

	MASS_PM4_U1					= 14,
	MASS_PM4_U2					= 15,
	MASS_PM4_L1					= 16,
	MASS_PM4_L2					= 17,

	MASS_PM10_U1				= 18,
	MASS_PM10_U2				= 19,
	MASS_PM10_L1				= 20,
	MASS_PM10_L2				= 21,

	NUMBER_PM05_U1				= 22,
	NUMBER_PM05_U2				= 23,
	NUMBER_PM05_L1				= 24,
	NUMBER_PM05_L2				= 25,

	NUMBER_PM1_U1				= 26,
	NUMBER_PM1_U2				= 27,
	NUMBER_PM1_L1				= 28,
	NUMBER_PM1_L2				= 29,

	NUMBER_PM25_U1				= 30,
	NUMBER_PM25_U2				= 31,
	NUMBER_PM25_L1				= 32,
	NUMBER_PM25_L2				= 33,

	NUMBER_PM4_U1				= 34,
	NUMBER_PM4_U2				= 35,
	NUMBER_PM4_L1				= 36,
	NUMBER_PM4_L2				= 37,

	NUMBER_PM10_U1				= 38,
	NUMBER_PM10_U2				= 39,
	NUMBER_PM10_L1				= 40,
	NUMBER_PM10_L2				= 41,

	TYPICAL_SIZE_U1				= 42,
	TYPICAL_SIZE_U2				= 43,
	TYPICAL_SIZE_L1				= 44,
	TYPICAL_SIZE_L2				= 45,

//	MASS_PM1_U1					= 0,
//	MASS_PM1_U2					= 1,
//	MASS_PM1_L1					= 3,
//	MASS_PM1_L2					= 4,
//
//	MASS_PM25_U1				= 6,
//	MASS_PM25_U2				= 7,
//	MASS_PM25_L1				= 9,
//	MASS_PM25_L2				= 10,
//
//	MASS_PM4_U1					= 12,
//	MASS_PM4_U2					= 13,
//	MASS_PM4_L1					= 15,
//	MASS_PM4_L2					= 16,
//
//	MASS_PM10_U1				= 18,
//	MASS_PM10_U2				= 19,
//	MASS_PM10_L1				= 21,
//	MASS_PM10_L2				= 22,
//
//	NUMBER_PM05_U1				= 24,
//	NUMBER_PM05_U2				= 25,
//	NUMBER_PM05_L1				= 27,
//	NUMBER_PM05_L2				= 28,
//
//	NUMBER_PM1_U1				= 30,
//	NUMBER_PM1_U2				= 31,
//	NUMBER_PM1_L1				= 33,
//	NUMBER_PM1_L2				= 34,
//
//	NUMBER_PM25_U1				= 36,
//	NUMBER_PM25_U2				= 37,
//	NUMBER_PM25_L1				= 39,
//	NUMBER_PM25_L2				= 40,
//
//	NUMBER_PM4_U1				= 42,
//	NUMBER_PM4_U2				= 43,
//	NUMBER_PM4_L1				= 45,
//	NUMBER_PM4_L2				= 46,
//
//	NUMBER_PM10_U1				= 48,
//	NUMBER_PM10_U2				= 49,
//	NUMBER_PM10_L1				= 51,
//	NUMBER_PM10_L2				= 52,
//
//	TYPICAL_SIZE_U1				= 54,
//	TYPICAL_SIZE_U2				= 55,
//	TYPICAL_SIZE_L1				= 57,
//	TYPICAL_SIZE_L2				= 58,

};

float float_temp;
uint32_t data;

int sps30_pm1_mass;
int sps30_pm25_mass;
int sps30_pm4_mass;
int sps30_pm10_mass;

int sps30_pm05_num;
int sps30_pm1_num;
int sps30_pm25_num;
int sps30_pm4_num;
int sps30_pm10_num;

int sps30_typical_size;

/**Aidan*
 *
 * RESET VERIFICATION
 *
 */

const uint16_t CODE;
#define CODE  10102
uint16_t code_in;

typedef struct{

} VMNResponse;
