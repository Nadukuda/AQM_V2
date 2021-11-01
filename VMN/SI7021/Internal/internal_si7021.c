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

uint8_t CORE_si7021_humidity_data[2];
uint8_t CORE_si7021_temperature_data[2];

void CORE_si7021Setup(void);
//void CORE_si7021RequestHumidity(void);
void CORE_si7021ReadHumidity(void);
void CORE_si7021RequestTemperature(void);
void CORE_si7021ReadTemperature(void);
uint8_t CORE_si7021RequestHumidity(void);
uint16_t CORE_si7021getHumidityData(void);
uint16_t CORE_si7021getTemperatureData(void);

void CORE_si7021Setup(void){
	if(!init){
		halI2c1Initialize();
		init = true;
	}
}

//void CORE_si7021RequestHumidity(void)
uint8_t CORE_si7021RequestHumidity(void){
	return halI2c1WriteBytes(WRITE_ADDRESS,&MEASURE_HUMIDITY,1);
}

void CORE_si7021ReadHumidity(void){
	CORE_si7021_humidity_data[0] = 0;
	CORE_si7021_humidity_data[1] = 0;
	halI2c1ReadBytes(READ_ADDRESS, &CORE_si7021_humidity_data,2);
}

void CORE_si7021RequestTemperature(void){
	halI2c1WriteBytes(WRITE_ADDRESS,&MEASURE_TEMP,1);
}

void CORE_si7021ReadTemperature(void){
	CORE_si7021_temperature_data[0] = 0;
	CORE_si7021_temperature_data[1] = 0;
	halI2c1ReadBytes(READ_ADDRESS, &CORE_si7021_temperature_data,2);
}

uint16_t CORE_si7021getHumidityData(void){
	uint16_t humidity = (CORE_si7021_humidity_data[0] << 8) | (CORE_si7021_humidity_data[1] & 0xFE);
	return humidity;
}

uint16_t CORE_si7021getTemperatureData(void){
	uint16_t temperature = (CORE_si7021_temperature_data[0] << 8) | (CORE_si7021_temperature_data[1] & 0xFC);
	return temperature;
}
