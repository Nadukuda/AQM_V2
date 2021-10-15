/*
 * I2C-Solenoid-Control.c
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


enum position{
	sol1_open  		= 	0x20,
	sol1_close 		=	0x10,
	sol2_open 		=	0x08,
	sol2_close		=	0x04,
	sol3_open		=	0x02,
	sol3_close		=	0x01,
	all_open		=	0x2A,
	all_close		=	0x15
};

static uint16_t solenoid_delay_time = 50000;

static uint8_t solenoid_address = 0x42;
static uint8_t solenoid_init[] = {0x00, 0x00};
static uint8_t open[] = {0x12, 0x2A};
static uint8_t close[] = {0x12, 0x15};
//uint8_t off[] = {0x09, 0x00};
static uint8_t off[] = {0x12, 0x00};


static uint8_t valve_position[] = {0x12, 0x00};

static boolean sol_initialised = false;

void Solenoid(uint8_t valve_number, uint8_t valve_state);

void Solenoid(uint8_t valve_number, uint8_t valve_state){

	if(!sol_initialised){
		halI2cInitialize();
		halI2cWriteBytes(solenoid_address, &solenoid_init, 2);
		sol_initialised = true;
	}
	switch (valve_number){

	case 0 :
		switch (valve_state){

		case 1 :
			valve_position[1] = sol1_open;
			halI2cWriteBytes(solenoid_address, &valve_position, 2);
			break;

		case 0 :
			valve_position[1] = sol1_close;
			halI2cWriteBytes(solenoid_address, &valve_position, 2);
			break;
		}

	break;

	case 1 :

		switch (valve_state){
		case 1 :
			valve_position[1] = sol2_open;
			halI2cWriteBytes(solenoid_address, &valve_position, 2);
			break;

		case 0 :
			valve_position[1] = sol2_close;
			halI2cWriteBytes(solenoid_address, &valve_position, 2);
			break;
		}
	break;
		case 2 :

				switch (valve_state){
				case 1 :
					valve_position[1] = sol3_open;
					halI2cWriteBytes(solenoid_address, &valve_position, 2);
					break;

				case 0 :
					valve_position[1] = sol3_close;
					halI2cWriteBytes(solenoid_address, &valve_position, 2);
					break;
				}
			break;
	}
	halCommonDelayMicroseconds(solenoid_delay_time);
	halI2cWriteBytes(solenoid_address, &off, 2);
}


