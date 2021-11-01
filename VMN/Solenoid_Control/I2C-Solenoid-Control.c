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
#include <Data_Protocol/VMN_globals.h>
#include <Data_Protocol/VMN_definitions.h>

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
static uint8_t off[] = {0x12, 0x02};


static uint8_t valve_position[] = {0x12, 0x00};

static boolean sol_initialised = false;

void Solenoid(uint8_t valve_number, uint8_t valve_state);

extern void errormessagetimestamp(void);
extern uint8_t errormessage_payload[9];
// I2C is returning different error codeds when it can't talk to the external devices have a look for error codes
/*
void Solenoid_init(void)
{
	if(!sol_initialised){
		emberAfAppPrintln("i2c0 init");
		halI2cInitialize();
		halI2cWriteBytes(aqm_control_address, &solenoid_init, 2);
		halI2cWriteBytes(aqm_control_address, &mosfet_init, 2);
		sol_initialised = true;
	}
}*/
void Solenoid(uint8_t valve_number, uint8_t valve_state){

	if(!sol_initialised){
	//	emberAfAppPrintln("solenoid_init");
		halI2cInitialize();
		uint8_t error = halI2cWriteBytes(solenoid_address, &solenoid_init, 2);
		if(error != 0) {
			errormessage_payload[0]=17; // If Si7021/HTU21D is not responding
			errormessagetimestamp();
			VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
			emberAfAppPrint(" EXPANDER is Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");
		}
		else
		sol_initialised = true;
	}
	switch (valve_number){

	case SOLENOID1:

		switch (valve_state){
		//emberAfAppPrintln("SOLENOID1");
		case SOLENOID_OPEN :
		//	emberAfAppPrintln("SOLENOID1 open");
			valve_position[1] = sol1_open | sol3_open;
			halI2cWriteBytes(solenoid_address, &valve_position, 2);
			break;

		case SOLENOID_CLOSE :
		//	emberAfAppPrintln("SOLENOID1 close");
			valve_position[1] = sol1_close| sol3_open;
			halI2cWriteBytes(solenoid_address, &valve_position, 2);
			break;
		}

	break;

	case SOLENOID2:
	//	emberAfAppPrintln("SOLENOID2");
		switch (valve_state){
		case SOLENOID_OPEN :
		//	emberAfAppPrintln("SOLENOID2 open");
			valve_position[1] = sol2_open | sol3_open;
			halI2cWriteBytes(solenoid_address, &valve_position, 2);
			break;

		case SOLENOID_CLOSE :
		//	emberAfAppPrintln("SOLENOID2 close");
			valve_position[1] = sol2_close | sol3_open;
			halI2cWriteBytes(solenoid_address, &valve_position, 2);
			break;
		}
	break;
		case SOLENOID3:
		//	emberAfAppPrintln("SOLENOID3");
				switch (valve_state){
				case SOLENOID_OPEN :
			//		emberAfAppPrintln("SOLENOID3 open");
					valve_position[1] = sol3_open;
					halI2cWriteBytes(solenoid_address, &valve_position, 2);
					break;

				case SOLENOID_CLOSE :
			//		emberAfAppPrintln("SOLENOID3 close");
					valve_position[1] = sol3_close;
					halI2cWriteBytes(solenoid_address, &valve_position, 2);
					break;
				}
			break;
	}
	halCommonDelayMicroseconds(solenoid_delay_time);
//	halI2cWriteBytes(solenoid_address, &off, 2);
}


