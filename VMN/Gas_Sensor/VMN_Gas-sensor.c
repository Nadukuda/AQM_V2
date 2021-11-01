/*
 * VMN_Gas-sensor.c
 *
 *  Created on: 5 Apr 2019
 *      Author: Aidan
 *
 *      ADC0 = NO2
 *      ADC1 = O3
 *
 *      ADC0 channel 1 = working electrode
 *      ADC0 channel 2 = auxiliary electrode
 *
 *      ADC1 channel 1 = working electrode
 *      ADC1 channel 2 =  auxiliary electrode
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
#include "stdio.h"
#include "stdlib.h"
#include "stack/include/ember.h"

void databuffer_empty(uint8_t buffer[]);
static void ADCInitialise(uint8_t adc_select_in, uint8_t sensor_number);
static uint8_t ADCReadChannelOne(uint8_t adc_select_in);
static uint8_t ADCReadChannelTwo(uint8_t adc_select_in);
static uint8_t ADCReadChannelThree(uint8_t adc_select_in);
static uint8_t ADCReadChannelFour(uint8_t adc_select_in);
static void ADCRequestCh1(uint8_t adc_select_in);
static void ADCRequestCh2(uint8_t adc_select_in);
static void ADCRequestCh3(uint8_t adc_select_in);
static void ADCRequestCh4(uint8_t adc_select_in);
void setAirTemp(uint16_t input);
void setAirHumidity(uint16_t input);
void setGasSensorInterval(long interval_input);
uint8_t getADCGain(void);
void setADCGain(uint8_t gain_in_a, uint8_t gain_in_b, uint8_t inputs);
uint8_t setGasPurgeFanSpeed(uint8_t);
uint32_t setGasPurgeDuration(long);
uint32_t setGasPostPurgeDelay(long);

//Using 2 MCP3424
//Left side address 0x68
//right side address 0x6C 110 1 1000
//ADC0 = right side
//ADC1 = left side 1101 0000 = D0
//110  1 1000 =68
//110 1001 69
//110 1010 6a
//110 1011 6b
//110 1100 6c
//110 1101 6d
//110 1110 6e
//110 1111 6f
enum{
	MCP3424_0_WRITE_ADDRESS			=		0XD0, //D0 68 D2 69 D4 6a D6 D8 Da dc de
	MCP3424_0_READ_ADDRESS			=		0XD1, //D1 D3 D5 D7 D9 db dd df
	MCP3424_1_WRITE_ADDRESS			=		0XD8,
	MCP3424_1_READ_ADDRESS			=		0XD9,
	//sensor 1 and 3 uses these configs  (O3 and SO2)
	CHANNEL_1_CONFIG_REGISTER_G8	=		0x8F, // 8x gain
	CHANNEL_1_CONFIG_REGISTER_G4	=		0x8E, // 4x gain
	CHANNEL_1_CONFIG_REGISTER_G2	=		0x8D, // 2x gain
	CHANNEL_1_CONFIG_REGISTER_G1	=		0x8C, // 1x gain
	CHANNEL_1_CHECK_BYTE_G8			=		0x0F, // 8x gain
	CHANNEL_1_CHECK_BYTE_G4			=		0x0E, // 4x gain
	CHANNEL_1_CHECK_BYTE_G2			=		0x0D, // 2x gain
	CHANNEL_1_CHECK_BYTE_G1			=		0x0C, // 1x gain
	CHANNEL_2_CONFIG_REGISTER_G8	=		0xAF, // 8x gain
	CHANNEL_2_CONFIG_REGISTER_G4	=		0xAE, // 4x gain
	CHANNEL_2_CONFIG_REGISTER_G2	=		0xAD, // 2x gain
	CHANNEL_2_CONFIG_REGISTER_G1	=		0xAC, // 1x gain
	CHANNEL_2_CHECK_BYTE_G8			=		0x2F, // 8x gain
	CHANNEL_2_CHECK_BYTE_G4			=		0x2E, // 4x gain
	CHANNEL_2_CHECK_BYTE_G2			=		0x2D, // 2x gain
	CHANNEL_2_CHECK_BYTE_G1			=		0x2C, // 1x gain
	//sensor 2 and 4 uses these configs  (NO2 and other optional sensor)
	CHANNEL_3_CONFIG_REGISTER_G8	=		0xCF, // 8x gain
	CHANNEL_3_CONFIG_REGISTER_G4	=		0xCE, // 4x gain
	CHANNEL_3_CONFIG_REGISTER_G2	=		0xCD, // 2x gain
	CHANNEL_3_CONFIG_REGISTER_G1	=		0xCC, // 1x gain
	CHANNEL_3_CHECK_BYTE_G8			=		0x4F, // 8x gain
	CHANNEL_3_CHECK_BYTE_G4			=		0x4E, // 4x gain
	CHANNEL_3_CHECK_BYTE_G2			=		0x4D, // 2x gain
	CHANNEL_3_CHECK_BYTE_G1			=		0x4C, // 1x gain
	CHANNEL_4_CONFIG_REGISTER_G8	=		0xEF, // 8x gain
	CHANNEL_4_CONFIG_REGISTER_G4	=		0xEE, // 4x gain
	CHANNEL_4_CONFIG_REGISTER_G2	=		0xED, // 2x gain
	CHANNEL_4_CONFIG_REGISTER_G1	=		0xEC, // 1x gain
	CHANNEL_4_CHECK_BYTE_G8			=		0x6F, // 8x gain
	CHANNEL_4_CHECK_BYTE_G4			=		0x6E, // 4x gain
	CHANNEL_4_CHECK_BYTE_G2			=		0x6D, // 2x gain
	CHANNEL_4_CHECK_BYTE_G1			=		0x6C, // 1x gain

};

uint8_t channel_config_register1 = CHANNEL_1_CONFIG_REGISTER_G4;
uint8_t channel_config_register2 = CHANNEL_2_CONFIG_REGISTER_G4;
uint8_t channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G4;
uint8_t channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G4;
//uint8_t gain = 0x08;
uint8_t sample_rate = 0x12; // decimal 18

static uint8_t count = 0;

uint8_t payload[66];

uint16_t gas_temp;
uint16_t gas_humidity;

uint8_t gas_adc_gain;
uint8_t gas_adc_sample_rate;

uint8_t no2_sign;
uint8_t no2_temp;
int32_t no2_value;

uint8_t o3_sign;
uint8_t o3_temp;
int32_t o3_value;

uint8_t measurement_order;
//static uint8_t adc_1_channel_1_data_buffer[4]; //not using why?
//static uint8_t adc_1_channel_2_data_buffer[4]; //not using why?

//static uint32_t channel_1_data;
//static uint32_t channel_2_data;

static uint16_t air_temp;
static uint16_t air_humidity;

/*******  variables added for V2 by shravanthi ****************/

static uint8_t before_airblow_sensor1_data[2][15]; // col 0 = working electrode : col 1 = auxiliary electrode;
static uint8_t before_airblow_sensor2_data[2][15];  // col 0 = working electrode : col 1 = auxiliary electrode;
static uint8_t before_airblow_sensor3_data[2][15]; // col 0 = working electrode : col 1 = auxiliary electrode;
static uint8_t before_airblow_sensor4_data[2][15];  // col 0 = working electrode : col 1 = auxiliary electrode;

static uint8_t after_airblow_sensor1_data[2][15]; // col 0 = working electrode : col 1 = auxiliary electrode;
static uint8_t after_airblow_sensor2_data[2][15];  // col 0 = working electrode : col 1 = auxiliary electrode;
static uint8_t after_airblow_sensor3_data[2][15]; // col 0 = working electrode : col 1 = auxiliary electrode;
static uint8_t after_airblow_sensor4_data[2][15];  // col 0 = working electrode : col 1 = auxiliary electrode;

uint8_t adc0_ch1_config_register;// = CHANNEL_3_CONFIG_REGISTER_G1;
uint8_t adc0_ch2_config_register;// = CHANNEL_4_CONFIG_REGISTER_G1;
uint8_t adc0_ch3_config_register;// = CHANNEL_3_CONFIG_REGISTER_G1;
uint8_t adc0_ch4_config_register;// = CHANNEL_4_CONFIG_REGISTER_G1;
uint8_t adc1_ch1_config_register;// = CHANNEL_3_CONFIG_REGISTER_G1;
uint8_t adc1_ch2_config_register;// = CHANNEL_4_CONFIG_REGISTER_G1;
uint8_t adc1_ch3_config_register;// = CHANNEL_3_CONFIG_REGISTER_G1;
uint8_t adc1_ch4_config_register;// = CHANNEL_4_CONFIG_REGISTER_G1;

uint8_t adc0_ch1_check_byte;
uint8_t adc0_ch2_check_byte;
uint8_t adc0_ch3_check_byte;
uint8_t adc0_ch4_check_byte;
uint8_t adc1_ch1_check_byte;
uint8_t adc1_ch2_check_byte;
uint8_t adc1_ch3_check_byte;
uint8_t adc1_ch4_check_byte;

//Think whether we can use this buffers for all 4 sensors
static uint8_t adc0_ch1_data_buffer[4]; //bcause 18bit will give 3bytes of data and 1 config reg
static uint8_t adc0_ch2_data_buffer[4];
static uint8_t adc0_ch3_data_buffer[4]; //bcause 18bit will give 3bytes of data and 1 config reg
static uint8_t adc0_ch4_data_buffer[4];
static uint8_t adc1_ch1_data_buffer[4]; //bcause 18bit will give 3bytes of data and 1 config reg
static uint8_t adc1_ch2_data_buffer[4];
static uint8_t adc1_ch3_data_buffer[4]; //bcause 18bit will give 3bytes of data and 1 config reg
static uint8_t adc1_ch4_data_buffer[4];

uint8_t channel_call_flag = 0;
uint8_t adc0_ch1measurementskip = 0; // this flag is set when there is no sensor adc0_measurementskip_sensor2flag
//uint8_t adc0_ch2measurementskip = 0; // this flag is set when there is no sensor
uint8_t adc0_ch3measurementskip = 0; // this flag is set when there is no sensor
//uint8_t adc0_ch4measurementskip = 0; // this flag is set when there is no sensor
uint8_t adc1_ch1measurementskip = 0; // this flag is set when there is no sensor adc0_measurementskip_sensor2flag
//uint8_t adc1_ch2measurementskip = 0; // this flag is set when there is no sensor
uint8_t adc1_ch3measurementskip = 0; // this flag is set when there is no sensor
//uint8_t adc1_ch4measurementskip = 0; // this flag is set when there is no sensor

static uint8_t adc0_sensor_select = 1; //added newly on 06/10/2021 to selecr the sensor
static uint8_t adc1_sensor_select = 3; //added newly on 06/10/2021 to selecr the sensor
static uint8_t gas_measurement_fanspinflag = 1;

uint8_t sensor_countbuffer[4] = {0, 0,0,0};   //please update this buffer based on the sensors connected if connected value as 1 , Not connected = 0
uint8_t  firstpopulatedsensor = 0;
uint8_t sensorcount = 0;
uint8_t adcinit_control = 1;

uint8_t s1measurement_error = 0;
uint8_t s2measurement_error = 0;
uint8_t s3measurement_error = 0;
uint8_t s4measurement_error = 0;
uint8_t I2c_conditiontestflag = 0;

uint8_t errorcode;
uint8_t errormessage_payload[9];
uint32_t errormessagetime;
uint32_t errormessagedate;
uint8_t defaultgain = 0;

uint8_t adc0_repeatstate_control = 0;
uint8_t adc1_repeatstate_control = 0;

uint8_t adc0_connect_statusflag = 0;
uint8_t adc1_connect_statusflag = 0;

void ADCerrorCodes(uint8_t sensorcount);
void errormessagetimestamp(void);
void ADCDataPackaging_oldformat(void);
/**************  END ***************/

enum {
	INACTIVE				= 0,
	INITIAL					= 1,
	REQUEST_MEASUREMENT1	= 2,
	REQUEST_MEASUREMENT2	= 3,
	FETCH_MEASUREMENT1		= 4,
	FETCH_MEASUREMENT2		= 5,
	TRANSMIT				= 6,
	PURGE_CHAMBER			= 7,
	POST_PURGE_DELAY		= 8,

};

static uint8_t gasSensorState = INACTIVE;
static boolean repeat = false;
static long interval = 30000;
//static uint8_t adc1_check;
//static uint8_t adc2_check;
static uint8_t fan_flag = 0;

// changed on 20/09/2021 to merge both SO2 and O3 projects
static uint8_t adc_gain;// = 4;
static uint8_t adc_gain_a;// = 4;
static uint8_t adc_gain_b;// = 4;

//  END
static uint8_t adc_select = 0; //to select 1 or 2
static uint8_t purge_fan_speed = 30;
static long purge_duration = 3000; //before 3000 shraavnthi changed to 4000 for test on 26/10/2021 to fill the chamber with more air to get to the sensors ask tinm
static long post_purge_delay = 5000;

static boolean initial_flag = false;

EmberEventControl gasSensorEventControl;
static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs);
#define setNextState(nextState)			setNextStateWithDelay((nextState), 0)
#define repeatState()					setNextStateWithDelay(gasSensorState, 0)
#define repeatStateWithDelay(delayMs)	setNextStateWithDelay(gasSensorState, (delayMs))



/*void adc0sensorsstate_finder(void) {

	if( CONNECTED_S1 == 0 ) {
		if(CONNECTED_S2 == 0)
			adc0_connect_statusflag = 1;
		else {
			adc0_sensor_select = 2;
			adc0_connect_statusflag = 0;
		}
	} else {
		adc0_sensor_select = 1;
		adc0_connect_statusflag = 0;
	}

}

void adc1sensorsstate_finder(void) {

	if( CONNECTED_S3 == 0 ) {
		if(CONNECTED_S4 == 0)
			adc1_connect_statusflag = 1;
		else {
			adc1_sensor_select = 4;
			adc1_connect_statusflag = 0;
		}
	} else {
		adc1_sensor_select = 3;
		adc1_connect_statusflag = 0;
	}

}*/


void defaultgainselector(uint8_t sensor){
	switch(sensor) {
	case O3: defaultgain = sensors_defaultgain[0]; break;
	case SO2: defaultgain = sensors_defaultgain[1];break;
	case NO2: defaultgain = sensors_defaultgain[2]; break;
	case NO: defaultgain = sensors_defaultgain[3];break;
	case CO: defaultgain = sensors_defaultgain[4];break;
	case H2S: defaultgain = sensors_defaultgain[5];break;
	case NH3: defaultgain = sensors_defaultgain[6];break;
	default:
		defaultgain = 4; //error
	}
}
void gasSensorEventHandler(void){
	emberEventControlSetInactive(gasSensorEventControl);
	uint8_t adc0_presence_check = 5; //some random value
	uint8_t adc1_presence_check = 5; //some random value
	switch(gasSensorState) {

	case INACTIVE:
		break;

	case INITIAL:
	//	ADCInitialise(adc_select,sensor_select); //initially adc _Select=0 and sensor_select=1
		ADCInitialise(0,adc0_sensor_select); //initially adc _Select=0 and sensor_select=1
		ADCInitialise(1,adc1_sensor_select); //initially adc _Select=0 and sensor_select=1
		if(gas_measurement_fanspinflag == 2) {// This if enables the measurements after F
		//	emberAfAppPrintln("after fan spin");
			setFanSpeed(0); //Fan init
			if(!low_power_mode){
				Solenoid(SOLENOID2,SOLENOID_OPEN);  // 1 open 0 close solenoid
			} else if(initial_flag == false){
				Solenoid(SOLENOID2,SOLENOID_OPEN);
				initial_flag = true;
			}
	//		setFanSpeed(100); // spin fan to refresh chamber
	//		fan_flag = 1;
	//		setNextStateWithDelay(PURGE_CHAMBER, 3000);
			if(count == 0) {// && adc_select == 0){
				setNextState(PURGE_CHAMBER);
			//	emberAfAppPrintln("PURGE_CHAMBER");
			} else {
				setNextState(REQUEST_MEASUREMENT1);
			}
			gas_measurement_fanspinflag = 3;
		} else
			setNextStateWithDelay(REQUEST_MEASUREMENT1,50);

		break;


	case PURGE_CHAMBER:
	//	emberAfAppPrintln("PURGEentry");
		setFanSpeed(purge_fan_speed); //running fan for sometime
		setNextStateWithDelay(POST_PURGE_DELAY,purge_duration);
		break;

	case POST_PURGE_DELAY:
		setFanSpeed(0); //fan off
		setNextStateWithDelay(REQUEST_MEASUREMENT1, post_purge_delay);
		break;


	case REQUEST_MEASUREMENT1:
 //here think about how to request for ADC's based on sensor
		//1st sensor and 3rd ch1 & ch2,   2 and 4th sensors ch3, ch4
	//	emberAfAppPrintln("REQUEST_MEASUREMENT1");
		if(channel_call_flag == 1) {
			ADCRequestCh3(ADC0); //requesting channel 3 of ADC0
		//	emberAfAppPrintln("ch3");
			ADCRequestCh3(ADC1);////requesting channel 3 of ADC1
		} else {

			ADCRequestCh1(ADC0); //requesting channel 1 of ADC0
		//	emberAfAppPrintln("ch1");
			ADCRequestCh1(ADC1); //requesting channel 1 of ADC1
		}
		setNextStateWithDelay(FETCH_MEASUREMENT1,500);
		break;


	case FETCH_MEASUREMENT1: //In sensor_countbuffer 1->connected 0->not connected
		//emberAfAppPrintln("FETCH_MEASUREMENT1");
		if(channel_call_flag == 1){ //If sensor is exist read the channel
			adc0_presence_check = ADCReadChannelThree(ADC0);
			adc1_presence_check = ADCReadChannelThree(ADC1); //thsi returns 2
			if((adc0_presence_check == 1) && (adc1_presence_check == 2)) {  //if there is no sensor2 and sensor4
				adc0_ch3measurementskip = adc1_ch3measurementskip = 1; //add some control like wait for few reapeat events before confirming the sensor existance
			} else if( (adc0_presence_check == 1) || (adc1_presence_check == 2)) {
					if(adc0_presence_check == 1)
						adc0_ch3measurementskip = 1; //means there is no sensor2
					if(adc1_presence_check == 2)
						adc1_ch3measurementskip = 1; //means there is no sensor42
			//	sensor_countbuffer[1] = 1;
			} else {
				//		emberAfAppPrintln("rd = ch3");
				adc0_ch3measurementskip = adc1_ch3measurementskip = 0; //if both are present
				sensor_countbuffer[1] = 1;
				sensor_countbuffer[3] = 1;
			}
			setNextState(REQUEST_MEASUREMENT2); //if 2 sensors are available to measure

		} else { // Read the channel, if there is no response just loop back with a delay of 1s until finds the sensor.
			adc0_presence_check = ADCReadChannelOne(ADC0);
			adc1_presence_check = ADCReadChannelOne(ADC1);
			//		emberAfAppPrintln("adc1 ch1_presence_check2 = %d",adc1_presence_check);
			if(( adc0_presence_check == 1) && (adc1_presence_check == 2)){
				adc0_ch1measurementskip = 1;
				adc1_ch1measurementskip = 1;
			//	emberAfAppPrintln("ADC0 & ADC1 Channel 1 Fail");
			//	repeatStateWithDelay(500); //reduced from 1s to 500ms
			} else if( (adc0_presence_check == 1) || (adc1_presence_check == 2)) {
				if((adc0_presence_check == 1))
					adc0_ch1measurementskip = 1; //means there is no sensor2
				if(adc1_presence_check == 2)
					adc1_ch1measurementskip = 1; //means there is no sensor42
		//	sensor_countbuffer[1] = 1;
			} else {
				adc0_ch1measurementskip = adc1_ch1measurementskip = 0;
				sensor_countbuffer[0] = 1;
				sensor_countbuffer[2] = 1;
					//	setNextState(REQUEST_MEASUREMENT2); //if both sensors are populated, go and take measurement for sensor
			}
			setNextState(REQUEST_MEASUREMENT2); //if both sensors are populated, go and take measurement for sensor
		}
		break;
	case REQUEST_MEASUREMENT2:
	//	emberAfAppPrintln("REQUEST_MEASUREMENT2");

		if(channel_call_flag == 1) {
			ADCRequestCh4(ADC0);
		//	emberAfAppPrintln("ch4");
			ADCRequestCh4(ADC1);
		} else {
			ADCRequestCh2(ADC0);
			ADCRequestCh2(ADC1);
		}
		setNextStateWithDelay(FETCH_MEASUREMENT2,500);
		break;

	case FETCH_MEASUREMENT2:

	//	emberAfAppPrintln("FETCH_MEASUREMENT2");
		if(channel_call_flag == 1){  //If sensor is exist read the channel
			adc0_presence_check =	ADCReadChannelFour(ADC0);
			adc1_presence_check =	ADCReadChannelFour(ADC1);
			if((adc0_presence_check == 1) && (adc1_presence_check == 2)) {  //if there is no sensor2 and sensor4
				if(adc0_ch3measurementskip == 1)
					sensor_countbuffer[1] = 0;
				if(adc1_ch3measurementskip == 1)
					sensor_countbuffer[3] = 0;
			//	adc0_sensor_select = SENSOR2; // adc0,adc1 sensor select flags are set to 2 and 4 to skip measurement cycle for sensor 2 and 4
			//	adc1_sensor_select = SENSOR4;
				//	adc0_ch4measurementskip = adc1_ch4measurementskip = 1;
					channel_call_flag = 0;
			} else if( (adc0_presence_check == 1) || (adc1_presence_check == 2)){
					if(adc0_ch3measurementskip == 1) {
						sensor_countbuffer[1] = 0;
						sensor_countbuffer[3] = 1;
					}
					if (adc1_ch3measurementskip == 1){
						sensor_countbuffer[1] = 1;
						sensor_countbuffer[3] = 0;
					//	adc0_ch4measurementskip = 1; //means there is no sensor2
					}
			} else {
				adc0_ch3measurementskip = adc1_ch3measurementskip = 0;
						sensor_countbuffer[1] = 1;
						sensor_countbuffer[3] = 1;
					//	setNextStateWithDelay(TRANSMIT, 500);
			}
			setNextStateWithDelay(TRANSMIT, 500);
		} else { // Read the channel, if there is no response just loop back with a delay of 1s until finds the sensor.
				adc0_presence_check = ADCReadChannelTwo(ADC0);
				adc1_presence_check = ADCReadChannelTwo(ADC1);
				if(( adc0_presence_check == 1) && (adc1_presence_check == 2)){
				//	emberAfAppPrintln("ADC0 & ADC1 Channel 2 Fail"); //test purpose repoeating  but need some control to do other functions
					if(adc0_ch1measurementskip == 1)
						sensor_countbuffer[0] = 0;
					if(adc1_ch1measurementskip == 1)
						sensor_countbuffer[2] = 0;
					//	adc0_sensor_select = SENSOR1; // adc0,adc1 sensor select flags are set to 2 and 4 to skip measurement cycle for sensor 2 and 4
					//	adc1_sensor_select = SENSOR3;
				//	adc0_repeatstate_control++;
				//	repeatStateWithDelay(500);
				}else if( (adc0_presence_check == 1) || (adc1_presence_check == 2)){
					if(adc0_ch1measurementskip == 1) {
						sensor_countbuffer[0] = 0;
						sensor_countbuffer[2] = 1;
					}
					if (adc1_ch1measurementskip == 1){
						sensor_countbuffer[0] = 1;
						sensor_countbuffer[2] = 0;
					//	adc0_ch4measurementskip = 1; //means there is no sensor2
					}
				} else {
					adc0_ch1measurementskip = adc1_ch1measurementskip = 0;
					sensor_countbuffer[0] = 1;
					sensor_countbuffer[2] = 1;
				//	setNextStateWithDelay(TRANSMIT, 500);
			}
			setNextStateWithDelay(TRANSMIT, 500);
		}
		break;

	  case TRANSMIT:
	//	  emberAfAppPrintln("TRANSMIT");
		  switch(adc0_sensor_select) {
			  case SENSOR1:
			//	  emberAfAppPrintln("Hi --s1");
				  if(sensor_countbuffer[0] == 1)
					logData(SENSOR1);
				  break;
			  case SENSOR2:

				  if(sensor_countbuffer[1] == 1) {
				//	  emberAfAppPrintln("Hi--s2");
					  logData(SENSOR2);
				  }

				  break;
		  }
		  switch(adc1_sensor_select) {
		  	case SENSOR3:
		  	//	emberAfAppPrintln("Hi");
		  		if(sensor_countbuffer[2] == 1)
		  			logData(SENSOR3);
		  		break;
		  	case SENSOR4:
		  	//	emberAfAppPrintln("Hi1");
		  		if(sensor_countbuffer[3] == 1)
		  			 logData(SENSOR4);
		  		break;
		  }
		  adc0_ch1measurementskip = 0;
		  adc1_ch1measurementskip = 0;
		  adc0_ch3measurementskip = 0;
		  adc1_ch3measurementskip = 0;
		  if(count < 5 ) {//&& adc_select == 0){   //0 1 2  3 4
		//  	 emberAfAppPrintln("IF 1");
		  	 if((adc0_sensor_select == SENSOR1) || (adc1_sensor_select == SENSOR3)){
		  		adc0_sensor_select = SENSOR2;
		  		adc1_sensor_select = SENSOR4;
		  		channel_call_flag = 1;
		  	 } else {

					adc0_sensor_select = SENSOR1;
					adc1_sensor_select = SENSOR3;
					channel_call_flag = 0;
					count++;  // 1 2 3 4
		  	}
			setNextState(INITIAL);
		  }
		  if((count == 5) && (gas_measurement_fanspinflag == 1)){
		//	  emberAfAppPrintln("zzzzzzzz");
			  count = 0;
			  gas_measurement_fanspinflag = 2;
			//  adc0_sensor_select = SENSOR1;
			//  adc1_sensor_select = SENSOR3;
			//  channel_call_flag = 0;
			//  count = 0;

			  setNextState(INITIAL);
		  }
		  if((count == 5) && (gas_measurement_fanspinflag == 3)){
			  	  // if it comes back from Enqueue data, this Transmit has to be called from AIr-quality-control.c's aqmMasterEventControl event handler.
			 // GasSensor_DataPackaging();
			  count = 0;
			  gas_measurement_fanspinflag = 1;
			  channel_call_flag = 0;
			/*  adc0_ch1measurementskip = 0;
			  adc1_ch1measurementskip = 0;
			  adc0_ch3measurementskip = 0;
			  adc1_ch3measurementskip = 0;*/
			//  adc_select = 0;
			  if(!low_power_mode){
				  Solenoid(SOLENOID2,SOLENOID_CLOSE);
				  halCommonDelayMicroseconds(100);
			//	  Solenoid(SOLENOID1,SOLENOID_CLOSE);  // 1 open 0 close solenoid
				  Solenoid(SOLENOID3,SOLENOID_CLOSE);  // 1 open 0 close solenoid
			  }
			  if(repeat){
				  setNextStateWithDelay(INITIAL, interval);
			  }else{
//				  remotePurgeStart();
				//  for(int8_t i=0;i<4;i++) { //commented on 15/10/2021 to handle the payload for all 4 sensors
				  	si7021Gas();
				  	for(uint8_t i=0;i<4;i++){
				  		emberAfAppPrint("sensor_countbuffer : %d", sensor_countbuffer[i]);
				  	}
				  	emberAfAppPrintln("");
				  	/*
				  	 *  These measurement error count gives the
				  	 */
				  	for(uint8_t i=0;i<4;i++){
				  		if(sensor_countbuffer[i] == 1) {
				  			switch(i+1) {
				  			case SENSOR1:
				  				if(s1measurement_error >= 3) {//just for test checking for count 3 think about the value later
				  				//	s1measurement_error = 0;
				  					sensor_countbuffer[i] = 0;
				  				}
				  				break;
				  			case SENSOR2:
				  				if(s2measurement_error >= 3) {
			  					sensor_countbuffer[i] = 0;
			  				//	s2measurement_error = 0;
				  			}
				  			break;
				  			case SENSOR3:
				  				if(s3measurement_error >= 3){
				  				//	s3measurement_error = 0;
			  					sensor_countbuffer[i] = 0;
				  				}
				  				break;
				  			case SENSOR4:
				  				if(s4measurement_error >= 3) {
				  				//	s4measurement_error = 0;
			  					sensor_countbuffer[i] = 0;
				  				}
				  				break;
				  			}
				  		}
				  	}
				  	if((CONNECTED_S1 == NO2) || (CONNECTED_S3 == O3)) {//assuming that we definitely populates these 2 sensors
				  		ADCDataPackaging_oldformat();
				  	}


				  	for(uint8_t i=0;i<4;i++){
				  		if(sensor_countbuffer[i] == 1) {
				  			firstpopulatedsensor = i+1;
				  			emberAfAppPrintln("firstpopulatedsensor : %d", firstpopulatedsensor);
				  			i = 4;
				  		}
				  	}
				  	for(uint8_t i=1;i<5;i++)
				  		ADCerrorCodes(i);
				  	s1measurement_error = s2measurement_error = s3measurement_error = s4measurement_error = 0;
				 // 	ADCDataPackaging(firstpopulatedsensor);
				  	ADCDataPackaging(firstpopulatedsensor);
				  	remoteEnqueueStart();
				//  } //commented on 15/10/2021 to handle the payload for all 4 sensors
				  setNextState(INACTIVE);
			  }
		}
		  break;
	}
}

void errormessagetimestamp(void){
	VMN_rtccUpdate();
	errormessagetime = VMN_rtccGetTime();
	errormessagedate = VMN_rtccGetDate();
	//emberAfAppPrintln(" rtc %u %u", errormessagetime,errormessagedate);
	errormessage_payload[1] = errormessagetime >> 24 ;
	errormessage_payload[2] = errormessagetime >> 16;	//hours
	errormessage_payload[3] = errormessagetime >> 8;	//min
	errormessage_payload[4] = errormessagetime;			//seconds

	errormessage_payload[5] = errormessagedate >> 24;
	errormessage_payload[6] = errormessagedate >> 16;	//year
	errormessage_payload[7] = errormessagedate >> 8;	//month
	errormessage_payload[8] = errormessagedate;
}

uint8_t adcs1flag=0,adcs2flag=0,adcs3flag=0,adcs4flag=0;
extern void rtctimestamp(void);
void ADCerrorCodes(uint8_t sensorcount)
{
	switch(sensorcount) {
		case SENSOR1:
			if(!(CONNECTED_S1 && sensor_countbuffer[0])) {adcs1flag=1;} //test purpose
			break;
		case SENSOR2:
			if(!(CONNECTED_S2 && sensor_countbuffer[1])) {adcs2flag=1;}
			break;
		case SENSOR3:
			if(!(CONNECTED_S3 && sensor_countbuffer[2])) {adcs3flag=1;}
			break;
		case SENSOR4:
			if(!(CONNECTED_S4 && sensor_countbuffer[3])) {adcs4flag=1;}
			break;
	}
	if(sensorcount == 4){
		if((adcs1flag) || (adcs2flag) || (adcs3flag) || (adcs4flag)) {
			errormessagetimestamp();
			rtctimestamp();
		/*	if((adcs1flag) && (adcs2flag) && (adcs3flag) && (adcs4flag)) { //if both ADC's failed
				errorcode = 14;
				I2c_conditiontestflag = 1;
				VMN_payloadPackager(&global_server, &errorcode, 1, DAT, 0x0035);
			} else */{
				if((adcs1flag) && (adcs2flag)) {
					errormessage_payload[0] = 12;//errorcode = 12;
					VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
					emberAfAppPrint(" ADC1 Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");
				} else{
					if(adcs1flag) {
						errormessage_payload[0] = 8;//errorcode = 8; //only Sensor1 failed
						VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035);
						emberAfAppPrint(" SENSOR1 Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");
					}
					if(adcs2flag) {
						errormessage_payload[0] = 9;//errorcode = 8; //only Sensor1 failed
						VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035);
						emberAfAppPrint(" SENSOR2 Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");
						}
				}
				if((adcs3flag) && (adcs4flag)){ //When ADC2 failed
					errormessage_payload[0] = 13;//errorcode = 13;
					emberAfAppPrint(" ADC2 Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");
				} else{
					if(adcs3flag) {
						errormessage_payload[0] = 10;//errorcode = 10; //only Sensor3 failed
						VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035);
						emberAfAppPrint(" SENSOR3 Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");
					}
					if(adcs4flag) {
						errormessage_payload[0] = 11;//errorcode = 8; //only Sensor1 failed
						VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035);
						emberAfAppPrint(" SENSOR4 Not responding & Error code: %d",errormessage_payload[0]);emberAfAppPrintln("");
					}
				}
			}
		}
		adcs1flag = adcs2flag = adcs3flag= adcs4flag = 0;
	}
}
/*void logData(void){
	if(adc_select == 0){
		for(uint8_t i = 0; i < 3; i++){
			no2_data[0][(count*3)+i] = channel_1_data_buffer[i];
			no2_data[1][(count*3)+i] = channel_2_data_buffer[i];
		}
//		emberAfAppPrintln("Count = %d,NO2 Work = %x%x%x,NO2 AUX = %x%x%x",count, no2_data[0][0],no2_data[0][1],no2_data[0][2], no2_data[1][0], no2_data[1][1], no2_data[1][2]);
	} else {
		for(uint8_t i = 0; i < 3; i++){
			o3_data[0][(count*3)+i] = channel_1_data_buffer[i];
			o3_data[1][(count*3)+i] = channel_2_data_buffer[i];
		}
//		emberAfAppPrintln("Count = %d,O3 Work = %X%X%X,O3 AUX = %X%X%X",count, o3_data[0][0],o3_data[0][1],o3_data[0][2], o3_data[1][0],o3_data[1][1],o3_data[1][2]);
	}
}*/

void logData(uint8_t sensor_number){

	if(gas_measurement_fanspinflag != 1) {
	 switch(sensor_number) {
		case SENSOR1:
			for(uint8_t i = 0; i < 3; i++){  //18bit - 3bytes for each read
				after_airblow_sensor1_data[0][(count*3)+i] = adc0_ch1_data_buffer[i];
				after_airblow_sensor1_data[1][(count*3)+i] = adc0_ch2_data_buffer[i];
				//emberAfAppPrintln(" after airblow %d %d", adc0_ch1_data_buffer[i], adc0_ch2_data_buffer[i]);
			}
			break;
		case SENSOR2:
				for(uint8_t i = 0; i < 3; i++){
					after_airblow_sensor2_data[0][(count*3)+i] = adc0_ch3_data_buffer[i];
					after_airblow_sensor2_data[1][(count*3)+i] = adc0_ch4_data_buffer[i];
					//emberAfAppPrintln(" after airblow %d %d", adc0_ch3_data_buffer[i], adc0_ch4_data_buffer[i]);
				}
		  break;
		case SENSOR3:
				for(uint8_t i = 0; i < 3; i++){
					after_airblow_sensor3_data[0][(count*3)+i] = adc1_ch1_data_buffer[i];
					after_airblow_sensor3_data[1][(count*3)+i] = adc1_ch2_data_buffer[i];
				//	emberAfAppPrintln(" after airblow %d %d", adc1_ch1_data_buffer[i], adc1_ch2_data_buffer[i]);
				}
			break;
		case SENSOR4:
			if(sensor_number == 4)
				for(uint8_t i = 0; i < 3; i++){
					after_airblow_sensor4_data[0][(count*3)+i] = adc1_ch3_data_buffer[i];
					after_airblow_sensor4_data[1][(count*3)+i] = adc1_ch4_data_buffer[i];

				}
			break;
	//		emberAfAppPrintln("Count = %d,O3 Work = %X%X%X,O3 AUX = %X%X%X",count, o3_data[0][0],o3_data[0][1],o3_data[0][2], o3_data[1][0],o3_data[1][1],o3_data[1][2]);
		}
	}else {
		switch(sensor_number)
		{
			case SENSOR1:
				for(uint8_t i = 0; i < 3; i++){  //18bit - 3bytes for each read
					before_airblow_sensor1_data[0][(count*3)+i] = adc0_ch1_data_buffer[i];
					before_airblow_sensor1_data[1][(count*3)+i] = adc0_ch2_data_buffer[i];
				//	emberAfAppPrintln(" before airblow %d %d", adc0_ch1_data_buffer[i], adc0_ch2_data_buffer[i]);
				}
				break;
			case SENSOR2:
				for(uint8_t i = 0; i < 3; i++){
					before_airblow_sensor2_data[0][(count*3)+i] = adc0_ch3_data_buffer[i];
					before_airblow_sensor2_data[1][(count*3)+i] = adc0_ch4_data_buffer[i];
				//	emberAfAppPrintln(" before airblow %d %d", adc0_ch3_data_buffer[i], adc0_ch4_data_buffer[i]);
				}
				break;
			case SENSOR3:
				for(uint8_t i = 0; i < 3; i++){
					before_airblow_sensor3_data[0][(count*3)+i] = adc1_ch1_data_buffer[i];
					before_airblow_sensor3_data[1][(count*3)+i] = adc1_ch2_data_buffer[i];
				//	emberAfAppPrintln(" before airblow %d %d", adc1_ch1_data_buffer[i], adc1_ch2_data_buffer[i]);
				}
				break;
			case SENSOR4:
				for(uint8_t i = 0; i < 3; i++){
					before_airblow_sensor4_data[0][(count*3)+i] = adc1_ch3_data_buffer[i];
					before_airblow_sensor4_data[1][(count*3)+i] = adc1_ch4_data_buffer[i];
				//	emberAfAppPrintln(" before airblow %d %d", adc1_ch3_data_buffer[i], adc1_ch4_data_buffer[i]);
				}
				break;
			//		emberAfAppPrintln("Count = %d,O3 Work = %X%X%X,O3 AUX = %X%X%X",count, o3_data[0][0],o3_data[0][1],o3_data[0][2], o3_data[1][0],o3_data[1][1],o3_data[1][2]);
			}
		}
}
void gasSensorStart(void){
	repeat = true;
	setNextState(INITIAL);
}

void gasSensorStop(void){
	repeat = false;
	setNextState(INACTIVE);
}

void gasSensorSingle(void){
	repeat = false;
	setNextState(INITIAL);
}

void getGasSensorInterval(void){
	uint8_t p_load[] = {(interval >> 24),
						(interval >> 16),
						(interval >> 8),
						 interval,
	};
	VMN_payloadPackager(&global_server, p_load, sizeof(p_load), DAT, 0x0011);
}

void setGasSensorInterval(long interval_in){
	interval = interval_in;
}

uint8_t getADCGain(void){
	uint8_t p_load[] = {(adc_gain_a << 4) | adc_gain_b};
	emberAfAppPrintln("gain = %d,%d", adc_gain_a, adc_gain_b);
	return p_load[0];
//	VMN_payloadPackager(&global_server, p_load, sizeof(p_load), DAT, 0x0014);
}

void setADCGain(uint8_t gain_in_a, uint8_t gain_in_b, uint8_t inputs){

	if(inputs == 1){
		adc_gain_a = gain_in_a;
		adc_gain_b = gain_in_a;
	} else if(inputs == 2) {
		adc_gain_a = gain_in_a;
		adc_gain_b = gain_in_b;
	}
//	getADCGain();
}

/**
 * Initialises i2c communications for both MCP3424 chips and sets up both channels of the MCP3424 as follows:
 *
 *  - 'one shot' conversion mode
 *  - 18 bit sample rate
 *  - PGA gain will be selected based on the populated sensors default as 4x for both Sensor1 and Sensor2 and 2x for both Sensor3 and Sensor4
 *
 */

//This function is fine but need to test with hardware
static void ADCInitialise(uint8_t adc_select_in, uint8_t sensor_number){

	if(adcinit_control < 2) {
		if(adc_select_in == 0)
		 halI2C_ADC0Initialize();
		else {
			halI2C_ADC1Initialize();
			adcinit_control = 2;
		}
//	halI2cInitialize();
//	testbyte++;
	}
	//halI2c1Initialize();
	switch(sensor_number) {  // as for schematic s1- O3 s2 - NO2 s3 - SO2 s4-optional
		case SENSOR1:  // adc_gain for O3
			defaultgainselector(CONNECTED_S1);
			adc_gain = defaultgain;
			adc_gain_a =  defaultgain;
			adc_gain_b  = defaultgain;
		break;
		case SENSOR2: // adc_gain for NO2
			defaultgainselector(CONNECTED_S2);
			adc_gain = defaultgain;
			adc_gain_a =  defaultgain;
			adc_gain_b = defaultgain;
		break;
		case SENSOR3: // default adc_gain is set for SO2
			defaultgainselector(CONNECTED_S3);
			adc_gain = defaultgain;
			adc_gain_a = defaultgain;
			adc_gain_b = defaultgain;
			break;
		case SENSOR4: // Assuming as gain 2
			defaultgainselector(CONNECTED_S4);
			adc_gain = defaultgain;
			adc_gain_a = defaultgain;
			adc_gain_b = defaultgain;
			break;
		default: // default adc_gain is set for SO2
			defaultgainselector(CONNECTED_S1);
			adc_gain = defaultgain;
			adc_gain_a = defaultgain;
			adc_gain_b = defaultgain;
		break;
	}

	if(adc_select_in == 0){
		adc_gain = 2;//adc_gain_a;
	} else {
		adc_gain = 4;//adc_gain_b;
	}
//	emberAfAppPrintln("adc%d gain = %d", adc_select_in, adc_gain);
	switch(sensor_number) {
		case SENSOR1: case SENSOR3://default:
		//	emberAfAppPrintln("sensor %d", sensor_number);
			switch(adc_gain){
				case GAINx1:
					channel_config_register1 = CHANNEL_1_CONFIG_REGISTER_G1;//channel_config_register1_G1;
					channel_config_register2 = CHANNEL_2_CONFIG_REGISTER_G1;
					channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G1;
					channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G1;
					break;

				case GAINx2:
					channel_config_register1 = CHANNEL_1_CONFIG_REGISTER_G2;//channel_config_register1_G2;
					channel_config_register2 = CHANNEL_2_CONFIG_REGISTER_G2;//channel_config_register2_G2;
					channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G2;
					channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G2;
				//	emberAfAppPrintln(" I am from gain2adc1");
					break;

				case GAINx4:
					channel_config_register1 = CHANNEL_1_CONFIG_REGISTER_G4;//channel_config_register1_G4;
					channel_config_register2 = CHANNEL_2_CONFIG_REGISTER_G4;//channel_config_register2_G4;
					channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G4;
					channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G4;
				//	emberAfAppPrintln(" I am from gain4");
					break;

				case GAINx8:
					channel_config_register1 = CHANNEL_1_CONFIG_REGISTER_G8;
					channel_config_register2 = CHANNEL_2_CONFIG_REGISTER_G8;
					channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G8;
					channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G8;
					break;

				default: //gain 1
					channel_config_register1 = CHANNEL_1_CONFIG_REGISTER_G1;//channel_config_register1_G1;
					channel_config_register2 = CHANNEL_2_CONFIG_REGISTER_G1;
					channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G1;
					channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G1;
					break;
			}
			break;
		case SENSOR2: case SENSOR4:
			//	emberAfAppPrintln("sensor %d", sensor_number);
				switch(adc_gain){
					case GAINx1:
						channel_config_register1 = CHANNEL_3_CONFIG_REGISTER_G1;
						channel_config_register2 = CHANNEL_4_CONFIG_REGISTER_G1;
						channel_1_check_byte = CHANNEL_3_CHECK_BYTE_G1;
						channel_2_check_byte = CHANNEL_4_CHECK_BYTE_G1;
						break;

					case GAINx2:
						channel_config_register1 = CHANNEL_3_CONFIG_REGISTER_G2;
						channel_config_register2 = CHANNEL_4_CONFIG_REGISTER_G2;
						channel_1_check_byte = CHANNEL_3_CHECK_BYTE_G2;
						channel_2_check_byte = CHANNEL_4_CHECK_BYTE_G2;
					//	emberAfAppPrintln(" I am from gain2");
						break;

					case GAINx4:
						channel_config_register1 = CHANNEL_3_CONFIG_REGISTER_G4;
						channel_config_register2 = CHANNEL_4_CONFIG_REGISTER_G4;
						channel_1_check_byte = CHANNEL_3_CHECK_BYTE_G4;
						channel_2_check_byte = CHANNEL_4_CHECK_BYTE_G4;
						break;

					case GAINx8:
						channel_config_register1 = CHANNEL_3_CONFIG_REGISTER_G8;
						channel_config_register2 = CHANNEL_4_CONFIG_REGISTER_G8;
						channel_1_check_byte = CHANNEL_3_CHECK_BYTE_G8;
						channel_2_check_byte = CHANNEL_4_CHECK_BYTE_G8;
						break;

					default:  //gain 1
						channel_config_register1 = CHANNEL_3_CONFIG_REGISTER_G1;
						channel_config_register2 = CHANNEL_4_CONFIG_REGISTER_G1;
						channel_1_check_byte = CHANNEL_3_CHECK_BYTE_G1;
						channel_2_check_byte = CHANNEL_4_CHECK_BYTE_G1;
						break;
					}
				break;
		}
		switch(sensor_number) {
			case SENSOR1:
				adc0_ch1_config_register = channel_config_register1;
				adc0_ch2_config_register = channel_config_register2;
				adc0_ch1_check_byte = channel_1_check_byte;
				adc0_ch2_check_byte = channel_2_check_byte;
				break;
			case SENSOR2:
				adc0_ch3_config_register = channel_config_register1;
				adc0_ch4_config_register = channel_config_register2;
				adc0_ch3_check_byte = channel_1_check_byte;
				adc0_ch4_check_byte = channel_2_check_byte;
				break;
			case SENSOR3:
				adc1_ch1_config_register = channel_config_register1;
				adc1_ch2_config_register = channel_config_register2;
				adc1_ch1_check_byte = channel_1_check_byte;
				adc1_ch2_check_byte = channel_2_check_byte;
				break;
			case SENSOR4:
				adc1_ch3_config_register = channel_config_register1;
				adc1_ch4_config_register = channel_config_register2;
				adc1_ch3_check_byte = channel_1_check_byte;
				adc1_ch4_check_byte = channel_2_check_byte;
				break;
		}

		if(adc_select_in == 0){
			halI2cWriteBytes(MCP3424_0_WRITE_ADDRESS, &channel_config_register1, 1);
			halI2cWriteBytes(MCP3424_0_WRITE_ADDRESS, &channel_config_register2, 1);
		} else {
		//	emberAfAppPrintln("ADC1--- %d  %d", channel_config_register1,channel_config_register2);
			halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &channel_config_register1, 1);
			halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &channel_config_register2, 1);
		}
}

static void ADCRequestCh1(uint8_t adc_select_in){

	if(adc_select_in == 0){
	//	emberAfAppPrintln("adc0 ch1: %d", adc0_ch1_config_register);
		halI2cWriteBytes(MCP3424_0_WRITE_ADDRESS, &adc0_ch1_config_register, 1);
	} else {
	//	emberAfAppPrintln("adc1 ch1:%d",adc1_ch1_config_register);
		halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &adc1_ch1_config_register, 1);
	}
}

static void ADCRequestCh2(uint8_t adc_select_in){
	if(adc_select_in == 0){
	//	emberAfAppPrintln("adc0 ch2 %d",adc0_ch2_config_register);
		halI2cWriteBytes(MCP3424_0_WRITE_ADDRESS, &adc0_ch2_config_register, 1);
	} else {
	//	emberAfAppPrintln("adc1 ch2:%d",adc1_ch2_config_register);
		halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &adc1_ch2_config_register, 1);
	}
}

static void ADCRequestCh3(uint8_t adc_select_in){

	if(adc_select_in == 0){
	//	emberAfAppPrintln("adc0 ch3:%d",adc0_ch3_config_register);
		halI2cWriteBytes(MCP3424_0_WRITE_ADDRESS, &adc0_ch3_config_register, 1);
	} else {
	//	emberAfAppPrintln("adc1 ch3:%d",adc1_ch3_config_register);
		halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &adc1_ch3_config_register, 1);
	}
}

static void ADCRequestCh4(uint8_t adc_select_in){
	if(adc_select_in == 0){
	//	emberAfAppPrintln("adc0 ch4:%d",adc0_ch4_config_register);
		halI2cWriteBytes(MCP3424_0_WRITE_ADDRESS, &adc0_ch4_config_register, 1);
	} else {
	//	emberAfAppPrintln("adc1 ch4:%d",adc1_ch4_config_register);
		halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &adc1_ch4_config_register, 1);
	}
}

//this will fill up the buffer with Null characters
void databuffer_empty(uint8_t buffer[4]) {
	for(uint8_t i=0; i< 4;i++)
	{
		buffer[i] = '\0';
	}
}

/*
 *
 * Reads from channel one of the ADC and checks the returned 'Configuration Register' bitfield
 * to ensure that latest reading has been taken using the correct settings as stated above.
 *
 */
static uint8_t ADCReadChannelOne(uint8_t adc_select_in){

	if(adc_select_in == 0){
	//	emberAfAppPrintln("ADCReadChannelOne--");
		databuffer_empty(adc0_ch1_data_buffer);
		halI2cReadBytes(MCP3424_0_READ_ADDRESS, &adc0_ch1_data_buffer, 4);
	/*	emberAfAppPrint("adc0 ch1 payload:   ");
		for(int i=0;i<4;i++)
			{
				emberAfAppPrint(" %d ", adc0_ch1_data_buffer[i]);
			}
		emberAfAppPrintln("");*/
		if(adc0_ch1_data_buffer[3] == adc0_ch1_check_byte){
			if(adc0_ch1_data_buffer[0] != 0xff)
				return 0;
			else {
				s1measurement_error++;
				return 1;
			}

		}else {
			errormessage_payload[0]=0x0E;
			errormessagetimestamp();
			VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
		}

	} else {
		databuffer_empty(adc1_ch1_data_buffer);
		halI2cReadBytes(MCP3424_1_READ_ADDRESS, &adc1_ch1_data_buffer, 4);
	/*	emberAfAppPrint("adc1 ch1 payload:   ");
				for(int i=0;i<4;i++)
					{
						emberAfAppPrint(" %d ", adc1_ch1_data_buffer[i]);
					}
				emberAfAppPrintln("");*/
		if(adc1_ch1_data_buffer[3] == adc1_ch1_check_byte){
			if(adc1_ch1_data_buffer[0] != 0xff)
				return 0;
			else {
				s3measurement_error++;
				return 2;
			}

		}else {
			errormessage_payload[0]=0x0E;
			errormessagetimestamp();
			VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
		}
	}
}

/*
 *
 * Reads from channel one of the ADC and checks the returned 'Configuration Register' bitfield
 * to ensure that latest reading has been taken using the correct settings as stated above.
 *
 */

static uint8_t ADCReadChannelTwo(uint8_t adc_select_in){
//	emberAfAppPrintln("ADCReadChanneltwo");
	if(adc_select_in == 0){
		databuffer_empty(adc0_ch2_data_buffer);
		halI2cReadBytes(MCP3424_0_READ_ADDRESS, &adc0_ch2_data_buffer, 4);
		/*emberAfAppPrint("adc0 ch2 payload:   ");
		for(int i=0;i<4;i++) {
			emberAfAppPrint(" %d ", adc0_ch2_data_buffer[i]);
		}
		emberAfAppPrintln("");*/
		if(adc0_ch2_data_buffer[3] == adc0_ch2_check_byte){
			if(adc0_ch2_data_buffer[0] != 0xff)
				return 0;
			else {
				s1measurement_error++;
				return 1;
			}
		}else {
			errormessage_payload[0]=0x0E;
			errormessagetimestamp();
			VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
		}
	} else {
		databuffer_empty(adc1_ch2_data_buffer);
		halI2cReadBytes(MCP3424_1_READ_ADDRESS, &adc1_ch2_data_buffer, 4);
	/*	emberAfAppPrint("adc1 ch2 payload:   ");
				for(int i=0;i<4;i++) {
					emberAfAppPrint(" %d ", adc1_ch2_data_buffer[i]);
				}
				emberAfAppPrintln("");*/
		if(adc1_ch2_data_buffer[3] == adc1_ch2_check_byte){
			if(adc1_ch2_data_buffer[0] != 0xff)
				return 0;
			else {
				s3measurement_error++;
				return 2;
			}
		 }else {
				errormessage_payload[0]=0x0E;
				errormessagetimestamp();
				VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
			}
	}
}

/*
 *
 * Reads from channel one of the ADC and checks the returned 'Configuration Register' bitfield
 * to ensure that latest reading has been taken using the correct settings as stated above.
 *
 */
static uint8_t ADCReadChannelThree(uint8_t adc_select_in){

	if(adc_select_in == 0){
		databuffer_empty(adc0_ch3_data_buffer);
		halI2cReadBytes(MCP3424_0_READ_ADDRESS, &adc0_ch3_data_buffer, 4);
	/*	emberAfAppPrint("adc0 ch3 payload:   ");
		for(int i=0;i<4;i++)
				{
					emberAfAppPrint("%d  ", adc0_ch3_data_buffer[i]);
				}
		emberAfAppPrintln("");*/
		if(adc0_ch3_data_buffer[3] == adc0_ch3_check_byte){
			if(adc0_ch3_data_buffer[0] != 0xff)
				return 0;
			else {
				s2measurement_error++;
				return 1;
			}
		}else {
			errormessage_payload[0]=0x0E;
			errormessagetimestamp();
			VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
		}
	} else {
		databuffer_empty(adc1_ch3_data_buffer);
		halI2cReadBytes(MCP3424_1_READ_ADDRESS, &adc1_ch3_data_buffer, 4);
	/*	emberAfAppPrint("adc1 ch3 payload:   ");
				for(int i=0;i<4;i++) {
					emberAfAppPrint(" %d ", adc1_ch3_data_buffer[i]);
				}
				emberAfAppPrintln("");*/
		if(adc1_ch3_data_buffer[3] == adc1_ch3_check_byte){
			if(adc1_ch3_data_buffer[0] != 0xff)
				return 0;
			else {
				s4measurement_error++;
				return 2;
			}
		}else {
			errormessage_payload[0]=0x0E;
			errormessagetimestamp();
			VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
		}
	}

}

/*
 *
 * Reads from channel one of the ADC and checks the returned 'Configuration Register' bitfield
 * to ensure that latest reading has been taken using the correct settings as stated above.
 *
 */

static uint8_t ADCReadChannelFour(uint8_t adc_select_in){

	if(adc_select_in == 0){
		databuffer_empty(adc0_ch4_data_buffer);
		halI2cReadBytes(MCP3424_0_READ_ADDRESS, &adc0_ch4_data_buffer, 4);
	/*	emberAfAppPrint("adc0 ch4 payload:   ");
		for(int i=0;i<4;i++)
		{
						emberAfAppPrint(" %d", adc0_ch4_data_buffer[i]);
					}
		emberAfAppPrintln("");*/
		if(adc0_ch4_data_buffer[3] == adc0_ch4_check_byte){
			if(adc0_ch4_data_buffer[0] != 0xff)
				return 0;
			else {
				s2measurement_error++; return 1;
			}

		}else {
			errormessage_payload[0]=0x0E;
			errormessagetimestamp();
			VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
		}
	} else {
		databuffer_empty(adc1_ch4_data_buffer);
		halI2cReadBytes(MCP3424_1_READ_ADDRESS, &adc1_ch4_data_buffer, 4);
	/*	emberAfAppPrint("adc1 ch4 payload:   ");
				for(int i=0;i<4;i++) {
					emberAfAppPrint(" %d ", adc1_ch4_data_buffer[i]);
				}
				emberAfAppPrintln("");*/
		if(adc1_ch4_data_buffer[3] == adc1_ch4_check_byte){
			if(adc1_ch4_data_buffer[0] != 0xff)
				return 0;
			else {
				s4measurement_error++; return 2;
			}
		}else {
			errormessage_payload[0]=0x0E;
			errormessagetimestamp();
			VMN_payloadPackager(&global_server, &errormessage_payload, 9, DAT, 0x0035); //When ADC1 failed
		}
	}

}

void setAirTemp(uint16_t input){
	float CONST1 = 175.72;
	float CONST2 = 46.85;
	air_temp = input;//((CONST1*input ) / 65536)-CONST2;
}

void setAirHumidity(uint16_t input){
	air_humidity = input;//( (125 * input ) / 65536)-6;
}

void ADCDataPackaging_oldformat(void) {
	uint32_t local_time_var = 0;
	uint32_t local_date_var = 0;
	uint8_t oldformatpayload[74];
	memset(oldformatpayload, '\0', 74); //empty the payload buffer everytime before update
	oldformatpayload[0] = air_temp >> 8;
	oldformatpayload[1] = air_temp;

	oldformatpayload[2] = air_humidity >> 8;
	oldformatpayload[3] = air_humidity;

	oldformatpayload[4] = (adc_gain_a << 4) | adc_gain_b;
	oldformatpayload[5] = sample_rate;

	if((sensor_countbuffer[0] == 1) && (sensor_countbuffer[2] == 1)){
		for(uint8_t i = 6; i < 21; i++){
			oldformatpayload[i] = after_airblow_sensor1_data[0][i-6];
			oldformatpayload[i+15] = after_airblow_sensor1_data[1][i-6];
			oldformatpayload[i+30] = after_airblow_sensor3_data[0][i-6];
			oldformatpayload[i+45] = after_airblow_sensor3_data[1][i-6];
		//	emberAfAppPrintln("1 : %d %d %d %d", payload[i],payload[i+15],payload[i+30],payload[i+45]);
		}
	}else {
		if(sensor_countbuffer[0] == 1){
			for(uint8_t i = 6; i < 21; i++){
				oldformatpayload[i] = after_airblow_sensor1_data[0][i-6];
				oldformatpayload[i+15] = after_airblow_sensor1_data[1][i-6];
			}
		} else{
			for(uint8_t i = 6; i < 21; i++){
				oldformatpayload[i+30] = after_airblow_sensor3_data[0][i-6];
				oldformatpayload[i+45] = after_airblow_sensor3_data[1][i-6];
			}
		}
	}
	VMN_rtccUpdate();
	local_time_var = VMN_rtccGetTime();
	local_date_var = VMN_rtccGetDate();
	oldformatpayload[66] = local_time_var >> 24;
	oldformatpayload[67] = local_time_var >> 16;
	oldformatpayload[68] = local_time_var >> 8;
	oldformatpayload[69] = local_time_var;

	oldformatpayload[70] = local_date_var >> 24;
	oldformatpayload[71] = local_date_var >> 16;
	oldformatpayload[72] = local_date_var >> 8;
	oldformatpayload[73] = local_date_var;

	VMN_payloadPackager(&global_server, oldformatpayload, 74, DAT, 0x0031); // I am using same message id which I am using in V3.1 for NO2/O3 data

	gas_temp =  oldformatpayload[0] << 8 |
			oldformatpayload[1];
	gas_humidity = oldformatpayload[2] << 8 |
			oldformatpayload[3];

	gas_adc_gain = oldformatpayload[4];
	gas_adc_sample_rate = oldformatpayload[5];


	//emberAfAppPrint("DAT-ALPHAGAS,	");
	emberAfAppPrint("TEMP=%d,	", gas_temp);
	emberAfAppPrint("HUM=%d,	",gas_humidity);
	emberAfAppPrint("ADC-GAIN=%d,	",gas_adc_gain);
	emberAfAppPrint("ADC-SAMPLE-RATE=%d,	",gas_adc_sample_rate);

	emberAfAppPrint("NO2-WORK := ");
	for(uint8_t i = 6; i < 21; i += 3){
		no2_temp = oldformatpayload[i] & 0x01; //0x03;
		no2_sign = oldformatpayload[i] & 0x80;

		no2_value = 	(no2_sign << 24 |
						 no2_temp << 16 |
						 oldformatpayload[i+1] << 8 |
						 oldformatpayload[i+2]);
		emberAfAppPrint("%d:%d,	", (i-6)/3, no2_value);
	}
	emberAfAppPrint("NO2-AUX := ");
	for(uint8_t i = 21; i < 36; i += 3){
		no2_temp = oldformatpayload[i] & 0x01; //0x03;
		no2_sign = oldformatpayload[i] & 0x80;

		no2_value = 	(no2_sign << 24 |
						 no2_temp << 16 |
						 oldformatpayload[i+1] << 8 |
						 oldformatpayload[i+2]);
		emberAfAppPrint("%d:%d,	", (i-21)/3, no2_value);
	}
	emberAfAppPrint("O3-WORK := ");
	for(uint8_t i = 36; i < 51; i += 3){
		o3_temp = oldformatpayload[i] & 0x01; //0x03;
		o3_sign = oldformatpayload[i] & 0x80;

		o3_value = 	(o3_sign << 24 |
						 o3_temp << 16 |
						 oldformatpayload[i+1] << 8 |
						 oldformatpayload[i+2]);
		emberAfAppPrint("%d:%d,	", (i-36)/3, o3_value);
	}
		emberAfAppPrint("O3-AUX := ");
	for(uint8_t i = 51; i < 66; i += 3){
		o3_temp = oldformatpayload[i] & 0x01; //0x03;
		o3_sign = oldformatpayload[i] & 0x80;

		o3_value = 	(o3_sign << 24 |
						 o3_temp << 16 |
						 oldformatpayload[i+1] << 8 |
						 oldformatpayload[i+2]);
		emberAfAppPrint("%d:%d,	", (i-51)/3, o3_value);
	}

	emberAfAppPrintln("");
}

//void ADCDataPackaging(void){
void ADCDataPackaging(uint8_t sensorcount){
//	uint8_t payload[66];
//	emberAfAppPrintln("I am ADCDataPackaging");
	memset(payload, '\0', 66); //empty the payload buffer everytime before update
		payload[0] = air_temp >> 8;
		payload[1] = air_temp;

		payload[2] = air_humidity >> 8;
		payload[3] = air_humidity;

		payload[4] = (adc_gain_a << 4) | adc_gain_b;
		payload[5] = sample_rate;

		switch(sensorcount) {
			case SENSOR1: //SENSOR1 working electrode data
				emberAfAppPrint("DAT-SENSOR1GAS:=	");
				for(uint8_t i = 6; i < 21; i++){
					payload[i] = before_airblow_sensor1_data[0][i-6];
					payload[i+15] = before_airblow_sensor1_data[1][i-6];
					payload[i+30] = after_airblow_sensor1_data[0][i-6];
					payload[i+45] = after_airblow_sensor1_data[1][i-6];
				//	emberAfAppPrintln("1 : %d %d %d %d", payload[i],payload[i+15],payload[i+30],payload[i+45]);
				}
				break;
			case SENSOR2: //SENSOR2 working electrode data
				emberAfAppPrint("DAT-SENSOR2GAS:=	");
				for(uint8_t i = 6; i < 21; i++){
					payload[i] = before_airblow_sensor2_data[0][i-6];
					payload[i+15] = before_airblow_sensor2_data[1][i-6];
					payload[i+30] = after_airblow_sensor2_data[0][i-6];
					payload[i+45] = after_airblow_sensor2_data[1][i-6];
				//	emberAfAppPrintln("2 : %d %d %d %d", payload[i],payload[i+15],payload[i+30],payload[i+45]);
				} break;
			case SENSOR3: //SENSOR3 working electrode data
				emberAfAppPrint("DAT-SENSOR3GAS:=	");
				for(uint8_t i = 6; i < 21; i++){
					payload[i] = before_airblow_sensor3_data[0][i-6];
					payload[i+15] = before_airblow_sensor3_data[1][i-6];
					payload[i+30] = after_airblow_sensor3_data[0][i-6];
					payload[i+45] = after_airblow_sensor3_data[1][i-6];
				//	emberAfAppPrintln("3 : %d %d %d %d", payload[i],payload[i+15],payload[i+30],payload[i+45]);
				} break;
			case SENSOR4: //SENSOR4 working electrode data
				emberAfAppPrint("DAT-SENSOR4GAS:=	");
				for(uint8_t i = 6; i < 21; i++){
					payload[i] = before_airblow_sensor4_data[0][i-6];
					payload[i+15] = before_airblow_sensor4_data[1][i-6];
					payload[i+30] = after_airblow_sensor4_data[0][i-6];
					payload[i+45] = after_airblow_sensor4_data[1][i-6];
				//	emberAfAppPrintln("4 : %d %d %d %d", payload[i],payload[i+15],payload[i+30],payload[i+45]);
				} break;
		}
	//	if(sensorcount > 0) {
			printData();
			gasDataLog(payload);
		//}
			/* else {
			memset(payload, '\0', 66);
		}*/


}
/*
void GasSensor_DataPackaging(void){

//	uint8_t payload[66];
	uint16_t temp=0;
	uint16_t humidity=0;
		si7021RequestHumidity();
		delay(20);
//		  emberAfAppPrintln("FETCH");
		si7021ReadHumidity();
		humidity =  si7021getHumidityData();
		//  delay(1); // This is added on 05/08/2021 To work with HTU21D(F) RH/T SENSOR - 1ms delay
		  si7021RequestTemperature();
		  delay(50); // This is added on 05/08/2021 To work with HTU21D(F) RH/T SENSOR - 50ms delay
		  si7021ReadTemperature();
		  temp =  si7021getTemperatureData());
			payload[0] = temp >> 8;
			payload[1] = temp;

			payload[2] = humidity >> 8;
			payload[3] = humidity;

			payload[4] = (adc_gain_a << 4) | adc_gain_b;
			payload[5] = sample_rate;

			//no2 working electrode data
			for(uint8_t i = 6; i < 21; i++){
				payload[i] = no2_data[0][i-6];
			}

			//no2 auxiliary electrode data
			for(uint8_t i = 21; i < 36; i++){
				payload[i] = no2_data[1][i-21];
			}

			//o3 working electrode data
			for(uint8_t i = 36; i < 51; i++){
				payload[i] = o3_data[0][i-36];
			}

			//o3 working electrode data
			for(uint8_t i = 51; i < 66; i++){
				payload[i] = o3_data[1][i-51];
			}

	printData();
	gasDataLog(payload);

//	VMN_payloadPackager(&global_server, payload, 66, DAT, 0x0016);
}*/
static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs)
{
  gasSensorState = nextState;
  emberEventControlSetDelayMS(gasSensorEventControl, delayMs);
}

uint8_t test_print_entrycount =0 ;
void printData(void){
	/*if(test_print_entrycount == 0)
	{
		si7021Gas();
		test_print_entrycount++;
	}*/
//	si7021Gas(); //addded for test by shravanthi on 15/09/2021
	gas_temp =  payload[0] << 8 |
				payload[1];
	gas_humidity = payload[2] << 8 |
				   payload[3];

	gas_adc_gain = payload[4];
	gas_adc_sample_rate = payload[5];


	//emberAfAppPrint("DAT-ALPHAGAS,	");
	emberAfAppPrint("TEMP=%d,	", gas_temp);
	emberAfAppPrint("HUM=%d,	",gas_humidity);
	emberAfAppPrint("ADC-GAIN=%d,	",gas_adc_gain);
	emberAfAppPrint("ADC-SAMPLE-RATE=%d,	",gas_adc_sample_rate);

	emberAfAppPrint("Before-WORK=");
	for(uint8_t i = 6; i < 21; i += 3){
		no2_temp = payload[i] & 0x01; //0x03;
		no2_sign = payload[i] & 0x80;

		no2_value = 	(no2_sign << 24 |
						 no2_temp << 16 |
						 payload[i+1] << 8 |
						 payload[i+2]);
		emberAfAppPrint("%d:%d,	", (i-6)/3, no2_value);
	}
	emberAfAppPrint("Before-AUX=");
	for(uint8_t i = 21; i < 36; i += 3){
		no2_temp = payload[i] & 0x01; //0x03;
		no2_sign = payload[i] & 0x80;

		no2_value = 	(no2_sign << 24 |
						 no2_temp << 16 |
						 payload[i+1] << 8 |
						 payload[i+2]);
		emberAfAppPrint("%d:%d,	", (i-21)/3, no2_value);
	}
	emberAfAppPrint("After-WORK=");
	for(uint8_t i = 36; i < 51; i += 3){
		o3_temp = payload[i] & 0x01; //0x03;
		o3_sign = payload[i] & 0x80;

		o3_value = 	(o3_sign << 24 |
						 o3_temp << 16 |
						 payload[i+1] << 8 |
						 payload[i+2]);
		emberAfAppPrint("%d:%d,	", (i-36)/3, o3_value);
	}
		emberAfAppPrint("After-AUX=");
	for(uint8_t i = 51; i < 66; i += 3){
		o3_temp = payload[i] & 0x01; //0x03;
		o3_sign = payload[i] & 0x80;

		o3_value = 	(o3_sign << 24 |
						 o3_temp << 16 |
						 payload[i+1] << 8 |
						 payload[i+2]);
		emberAfAppPrint("%d:%d,	", (i-51)/3, o3_value);
	}

	emberAfAppPrintln("");
//    channel_1_data = ((channel_1_data_buffer[0] - 0xFC) << 16 |
//						  channel_1_data_buffer[1] << 8  |
//						  channel_1_data_buffer[2]);
//
//	channel_2_data = ((channel_2_data_buffer[0] - 0xFC) << 16 |
//					  channel_2_data_buffer[1] << 8  |
//					  channel_2_data_buffer[2]);
//
//	emberAfAppPrintln("ADC select = %d", adc_select);
//	emberAfAppPrintln("air temp = %d", air_temp);
//	emberAfAppPrintln("air hum = %d", air_humidity);
//	emberAfAppPrintln("gain = %d", adc_gain);
//	emberAfAppPrintln("sample rate = %d", sample_rate);
//	emberAfAppPrintln("NO2 Work = %X%X%X , NO2 AUX = %X%X%X", no2_data[0][0],no2_data[0][1],no2_data[0][2], no2_data[1][0],no2_data[1][1],no2_data[1][2]);
//	emberAfAppPrintln("O3 Work = %X%X%X , O3 AUX = %X%X%X", o3_data[0][0],o3_data[0][1],o3_data[0][2], o3_data[1][0],o3_data[1][1],o3_data[1][2]);
//	emberAfAppPrintln(" data = %X%X%X", channel_2_data_buffer[0],channel_2_data_buffer[1],channel_2_data_buffer[2]);
}



uint8_t setGasPurgeFanSpeed(uint8_t new_fan_speed){
	purge_fan_speed = new_fan_speed;
	uint8_t payload[] = {purge_fan_speed};
	return payload[0];
//	VMN_payloadPackager(&global_server, payload, 1, DAT, 0x0017);
}

uint32_t setGasPurgeDuration(long new_purge_duration){
	purge_duration = new_purge_duration;
	uint32_t return_payload = {purge_duration >> 24,
						  purge_duration >> 16,
						  purge_duration >> 8,
						  purge_duration
	};
	return return_payload;
//	VMN_payloadPackager(&global_server, payload, 4, DAT, 0x0018);
}

uint32_t setGasPostPurgeDelay(long new_delay){
	post_purge_delay = new_delay;
	uint32_t return_payload = {post_purge_delay >> 24,
						 post_purge_delay >> 16,
						 post_purge_delay >> 8,
						 post_purge_delay
	};
	return return_payload;
//	VMN_payloadPackager(&global_server, payload, 4, DAT, 0x0019);
}

/*
  if(adc_select == 0){ //if(count < 5 && adc_select == 0)
//			  emberAfAppPrintln("IF 1");
			//  adc_select = ;
			  if(loop_entry_count < 9) // 1 3 5 7 9 sensor2, 2 4 6 8 sensor1
			  {  // 0 1 2 3 4 5 6 7 8
				  if(sensor_select == 1) //0 2 4 6 8
					  sensor_select = 2;
				  else if(sensor_select == 2) //1 3 5 7
				  {
					  sensor_select = 1;
				  }
			  }
			  else
			 {
			 		adc_select = 1;
			 		sensor_select = 3;
			 		loop_entry_count = 0;
			 }
			  loop_entry_count++; //
			  setNextState(INITIAL);
		  } else if( adc_select == 1){//else if(count < 4 && adc_select == 1){  //

			  if(loop_entry_count < 9) // 1 3 5 7 9 sensor2, 2 4 6 8 sensor1
			  {  // 0 1 2 3 4 5 6 7 8
					if(sensor_select == 3) //0 2 4 6 8
						sensor_select = 4;
					else if(sensor_select == 4) //1 3 5 7
					{
						sensor_select = 3;
					}
			  	}
			  	 else
			  	{
			  		adc_select = 2; //for test think whether is it safe to keep 2 or anything else
			  		sensor_select = 1;
			  		loop_entry_count = 0;
			  	}
			  	  loop_entry_count++; //
//			  emberAfAppPrintln("IF 2");
			//  adc_select = 0;
			//  count ++;
			  setNextState(INITIAL);
		  }

 */

