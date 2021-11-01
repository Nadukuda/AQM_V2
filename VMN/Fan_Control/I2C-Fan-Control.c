/*
 * I2C-Fan-Control.c
 *
 *  Created on: 24 Jan 2019
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
uint8_t fan_write = 0x3E;
uint8_t fan_read = 0x3F;

uint8_t full_off[] = {0x02, 0x10};
uint8_t closed_loop[] = {0x02, 0x22};

uint8_t set_speed[] = {0x00, 0x00};

boolean fan_init = false;

//uint8_t upper_bound = 57;
//uint8_t lower_bound = 6;
extern void errormessagetimestamp(void);
extern uint8_t errormessage_payload[9];
void setFanSpeed(uint8_t speed);
void fanInitialise(void);


void fanInitialise(void){
	if(!fan_init){
		halI2cInitialize();
		fan_init = true;
	}
	halI2cWriteBytes(fan_write, &full_off, 2); // initialise
}

void setFanSpeed(uint8_t speed){
	if(!fan_init){
		halI2cInitialize();
		fan_init = true;
	}
	if(speed == 0){
		fanInitialise();
	} else {
		uint8_t temp = abs(speed-100);
		uint8_t scaled_speed = (temp/2) + 6;
		uint8_t error = halI2cWriteBytes(fan_write, &closed_loop, 2); // set closed loop mode
		if(error != 0){
			errormessage_payload[0]=16; // If Si7021/HTU21D is not responding
			errormessagetimestamp();
			VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
			emberAfAppPrint(" FAN Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");
		}else{
			set_speed[1] = scaled_speed;
			halI2cWriteBytes(fan_write, &set_speed, 2);
		}
	}
}






