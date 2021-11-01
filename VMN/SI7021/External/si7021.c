/*
 * si7021.c
 *
 *  Created on: 11 Dec 2018
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


static boolean init = false;
static uint8_t WRITE_ADDRESS = 0x80;
static uint8_t READ_ADDRESS = 0x81;

static uint8_t MEASURE_HUMIDITY = 0xF5;
static uint8_t MEASURE_TEMP = 0xF3;

uint8_t si7021_humidity_data[2];
uint8_t si7021_temperature_data[2];

void si7021Setup(void);
//void si7021RequestHumidity(void);
uint8_t si7021RequestHumidity(void);
void si7021ReadHumidity(void);
void si7021RequestTemperature(void);
void si7021ReadTemperature(void);

uint16_t si7021getHumidityData(void);
uint16_t si7021getTemperatureData(void);

void si7021Setup(void){
	if(!init){
		//halI2cInitialize();
		halI2C_Si7021Initialize();
		init = true;
	}
}

//void si7021RequestHumidity(void)
uint8_t si7021RequestHumidity(void){
	uint8_t result = halI2cWriteBytes(WRITE_ADDRESS,&MEASURE_HUMIDITY,1);
//	if(result != 0)
//	{
	return result;
			//	  emberAfAppPrintln("RESULT = %d ",result);// TEMP/HUM SENSOR Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");

}

void si7021ReadHumidity(void){
	si7021_humidity_data[0] = 0;
	si7021_humidity_data[1] = 0;
	halI2cReadBytes(READ_ADDRESS, &si7021_humidity_data,2);
}

void si7021RequestTemperature(void){
	halI2cWriteBytes(WRITE_ADDRESS,&MEASURE_TEMP,1);
}

void si7021ReadTemperature(void){
	si7021_temperature_data[0] = 0;
	si7021_temperature_data[1] = 0;
	halI2cReadBytes(READ_ADDRESS, &si7021_temperature_data,2);
}

uint16_t si7021getHumidityData(void){
	uint16_t humidity = (si7021_humidity_data[0] << 8) | (si7021_humidity_data[1] & 0xFE);
	return humidity;
}

uint16_t si7021getTemperatureData(void){
	uint16_t temperature = (si7021_temperature_data[0] << 8) | (si7021_temperature_data[1] & 0xFC);
	return temperature;
}
