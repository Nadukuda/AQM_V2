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
	all_open		=	0x2B,
	all_close		=	0x14,
	aqm1_only		=	0x01,	//MOSFET
	aqm2_only		=	0x02,  //MOSFET
};

uint16_t solenoid_delay_time = 100000;

//uint8_t solenoid_address = 0x40;
uint8_t aqm_control_address = 0x42;
uint8_t solenoid_init[] = {0x00, 0x00};
uint8_t mosfet_init[] = {0x01, 0x00};
uint8_t open[] = {0x09, 0x2A};
uint8_t close[] = {0x09, 0x15};
//uint8_t off[] = {0x09, 0x00};
uint8_t solenoids_off[] = {0x12, 0x02}; //changed 0x12 0x00 to 0x12 0x02 to keep sol3 always on
uint8_t mosfets_off[] = {0x13, 0x00};
uint8_t pullup_on[] = {0x06, 0xff};


uint8_t valve_position[] = {0x12, 0x00};
uint8_t aqm_power_state[] = {0x13, 0x00};

boolean sol_initialised = false;

void aqmPowerOn(uint8_t);
void aqmPowerOff(void);
extern void Solenoid_init(void);
extern void errormessagetimestamp(void);
extern uint8_t errormessage_payload[9];
void aqmPowerOn(uint8_t sensor_select){
	emberAfAppPrintln("AQM ON");
	if(!sol_initialised){
	//	emberAfAppPrintln("i2c0 init");
		halI2cInitialize();
		uint8_t error =  halI2cWriteBytes(aqm_control_address, &solenoid_init, 2);
		if(error != 0) {
			errormessage_payload[0]=17; // If Si7021/HTU21D is not responding
			errormessagetimestamp();
			VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
			emberAfAppPrint(" IO EXPANDER is Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");
		} else {
			halI2cWriteBytes(aqm_control_address, &mosfet_init, 2);
			sol_initialised = true;
			if(low_power_mode){
				// open selected solenoid
				if(sensor_select == 0){
				//	emberAfAppPrintln("sol1--");  //test purpose
					valve_position[1] = sol1_open | sol3_open;
				} else {
				//	emberAfAppPrintln("sol2");
					valve_position[1] = sol2_open | sol3_open;
				}
				halI2cWriteBytes(aqm_control_address, &valve_position,2);

				halCommonDelayMicroseconds(solenoid_delay_time);


				//de energise solenoid

				halI2cWriteBytes(aqm_control_address, &solenoids_off,2);

			}
		}
	}
	if(sol_initialised == true) {
		if(!low_power_mode){
			// open selected solenoid
			if(sensor_select == 0){
			//	emberAfAppPrintln("sol1");
				valve_position[1] = sol1_open  | sol3_open;
			} else {
			//	emberAfAppPrintln("sol2");
				valve_position[1] = sol2_open  | sol3_open;
			}
			halI2cWriteBytes(aqm_control_address, &valve_position,2);

			halCommonDelayMicroseconds(solenoid_delay_time);


			//de energise solenoid

			halI2cWriteBytes(aqm_control_address, &solenoids_off,2);

		}

		//power on selected sensor

		if(!sensor_select){
		//	emberAfAppPrintln("aqm1_only");
			aqm_power_state[1] = aqm1_only;
		} else {
		//	emberAfAppPrintln("aqm2_only");
			aqm_power_state[1] = aqm2_only;
		}

		halI2cWriteBytes(aqm_control_address, &aqm_power_state,2);
	}
}

void aqmPowerOff(){

	if(!sol_initialised){
		halI2cInitialize();
		halI2cWriteBytes(aqm_control_address, &solenoid_init, 2);
		halI2cWriteBytes(aqm_control_address, &mosfet_init, 2);
		sol_initialised = true;
	}

	if(!low_power_mode){
		// close solenoid
		valve_position[1] = (all_close | sol3_open);//0x16;//changed to trigger sol3 to open all the time  instead of all_close  changed sol1_close just to close sol1
		halI2cWriteBytes(aqm_control_address, &valve_position,2);
		halCommonDelayMicroseconds(solenoid_delay_time);

		// power down aqm and de-energise solenoid
		halI2cWriteBytes(aqm_control_address, &solenoids_off,2); //I think not needed but check it out after sort out the sol3 open control
	}

	halI2cWriteBytes(aqm_control_address, &mosfets_off,2);

}

