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
//right side address 0x6C

enum{
	MCP3424_0_WRITE_ADDRESS			=		0XD0,
	MCP3424_0_READ_ADDRESS			=		0XD1,
	MCP3424_1_WRITE_ADDRESS			=		0XD8,
	MCP3424_1_READ_ADDRESS			=		0XD9,
	//sensor 1 and 3 uses these configs  (O3 and SO2)
	channel_config_register1_G8	=		0x8F, // 8x gain
	channel_config_register1_G4	=		0x8E, // 4x gain
	channel_config_register1_G2	=		0x8D, // 2x gain
	channel_config_register1_G1	=		0x8C, // 1x gain
	CHANNEL_1_CHECK_BYTE_G8			=		0x0F, // 8x gain
	CHANNEL_1_CHECK_BYTE_G4			=		0x0E, // 4x gain
	CHANNEL_1_CHECK_BYTE_G2			=		0x0D, // 2x gain
	CHANNEL_1_CHECK_BYTE_G1			=		0x0C, // 1x gain
	channel_config_register2_G8	=		0xAF, // 8x gain
	channel_config_register2_G4	=		0xAE, // 4x gain
	channel_config_register2_G2	=		0xAD, // 2x gain
	channel_config_register2_G1	=		0xAC, // 1x gain
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

uint8_t channel_config_register1 = channel_config_register1_G1;
uint8_t channel_config_register2 = channel_config_register2_G1;
uint8_t channel_1_check_byte;
uint8_t channel_2_check_byte;
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

/*******  variables added for V2 by sharavanthi ****************/

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
uint8_t adc0_measurementskip_sensor1flag = 0; // this flag is set when there is no sensor adc0_measurementskip_sensor2flag
uint8_t adc0_measurementskip_sensor2flag = 0; // this flag is set when there is no sensor
uint8_t adc1_measurementskip_sensor3flag = 0; // this flag is set when there is no sensor
uint8_t adc1_measurementskip_sensor4flag = 0; // this flag is set when there is no sensor
uint8_t loop_entry_count = 0;
static uint8_t adc0_sensor_select = 1; //added newly on 06/10/2021 to selecr the sensor
static uint8_t adc1_sensor_select = 3; //added newly on 06/10/2021 to selecr the sensor
static uint8_t gas_measurement_fanspinflag = 1;


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
static long purge_duration = 3000;
static long post_purge_delay = 5000;

static boolean initial_flag = false;

EmberEventControl gasSensorEventControl;
static void setNextStateWithDelay(uint8_t nextState, uint32_t delayMs);
#define setNextState(nextState)			setNextStateWithDelay((nextState), 0)
#define repeatState()					setNextStateWithDelay(gasSensorState, 0)
#define repeatStateWithDelay(delayMs)	setNextStateWithDelay(gasSensorState, (delayMs))


void gasSensorEventHandler(void){
	emberEventControlSetInactive(gasSensorEventControl);
	uint8_t adc_presence_check1 = 0;
	uint8_t adc_presence_check2 = 0;
	switch(gasSensorState) {

	case INACTIVE:

		break;

	case INITIAL:
	//	ADCInitialise(adc_select,sensor_select); //initially adc _Select=0 and sensor_select=1
		ADCInitialise(ADC0,adc0_sensor_select); //initially adc _Select=0 and sensor_select=1
		ADCInitialise(ADC1,adc1_sensor_select); //initially adc _Select=0 and sensor_select=1
		if(gas_measurement_fanspinflag != 1) {// This if enables the measurements after F
			setFanSpeed(0); //Fan init
			if(!low_power_mode){
				Solenoid(1,SOLENOID_OPEN);  // 1 open 0 close solenoid
			} else if(initial_flag == false){
				Solenoid(1,SOLENOID_OPEN);
				initial_flag = true;
			}
	//		setFanSpeed(100); // spin fan to refresh chamber
	//		fan_flag = 1;
	//		setNextStateWithDelay(PURGE_CHAMBER, 3000);
			if(count == 0) {// && adc_select == 0){
				setNextState(PURGE_CHAMBER);
			} else {
				setNextState(REQUEST_MEASUREMENT1);
			}
		} else
			setNextState(REQUEST_MEASUREMENT1);

		break;


	case PURGE_CHAMBER:
		setFanSpeed(purge_fan_speed); //running fan for sometime
		setNextStateWithDelay(POST_PURGE_DELAY,purge_duration);
		break;

	case POST_PURGE_DELAY:
		setFanSpeed(0); //fan off
		setNextStateWithDelay(REQUEST_MEASUREMENT1, post_purge_delay);
		break;


	case REQUEST_MEASUREMENT1:
//		if(count >= 5){
//			setFanSpeed(0);
//			fan_flag = 0;
//		}

//		emberAfAppPrintln("REQUEST_MEASUREMENT1");
 //here think about how to request for ADC's based on sensor
		//1st sensor and 3rd ch1 & ch2,   2 and 4th sensors ch3, ch4
		if(channel_call_flag == 1) {
			ADCRequestCh3(ADC0); //requesting channel 3 of ADC0
			ADCRequestCh3(ADC1);////requesting channel 3 of ADC1
		} else {
			ADCRequestCh1(ADC0); //requesting channel 1 of ADC0
			ADCRequestCh1(ADC1); //requesting channel 1 of ADC1
		}
		setNextStateWithDelay(FETCH_MEASUREMENT1,500);
		break;


	case FETCH_MEASUREMENT1: //In sensor_countbuffer 1->connected 0->not connected
		if(channel_call_flag == 1){ //If sensor is exist read the channel
			adc_presence_check1 = ADCReadChannelThree(ADC0);
			adc_presence_check2 = ADCReadChannelThree(ADC1);
			if((adc_presence_check1 == 1) && (adc_presence_check2 == 1)) {  //if there is no sensor2 and sensor4
				adc0_sensor_select = SENSOR2; // adc0,adc1 sensor select flags are set to 2 and 4 to skip measurement cycle for sensor 2 and 4
				adc1_sensor_select = SENSOR4;
				adc0_measurementskip_sensor2flag = adc1_measurementskip_sensor4flag = 1;
				channel_call_flag = 0;
				sensor_countbuffer[3] = 0;
				sensor_countbuffer[1] = 0;
				setNextState(TRANSMIT); //go to transmit
			} else if(adc_presence_check1 == 1) {
				adc0_measurementskip_sensor2flag = 1; //means there is no sensor2
				sensor_countbuffer[3] = 1;
				sensor_countbuffer[1] = 0;
				setNextState(REQUEST_MEASUREMENT2); //go and take measuremetn for sensor 4
			} else if(adc_presence_check2 == 1){
				adc1_measurementskip_sensor4flag = 1; //means there is no sensor42
				sensor_countbuffer[1] = 1;
				sensor_countbuffer[3] = 0;
				setNextState(REQUEST_MEASUREMENT2); //go and take measurement for sensor
			} else {
				sensor_countbuffer[1] = 1; //sensor2
				sensor_countbuffer[3] = 1; //sensor4
				setNextState(REQUEST_MEASUREMENT2); //if 2 sensors are available to measure
			}

		} else { // Read the channel, if there is no response just loop back with a delay of 1s until finds the sensor.
			adc_presence_check1 = ADCReadChannelOne(ADC0);
			adc_presence_check2 = ADCReadChannelOne(ADC1);
			if(( adc_presence_check1 == 1) && (adc_presence_check2 == 1)){
				emberAfAppPrintln("ADC0 & ADC1 Channel 1 Fail");
				repeatStateWithDelay(500); //reduced from 1s to 500ms
				sensor_countbuffer[0] = 0;  //not connected sensor1
				sensor_countbuffer[2] = 0;  //sensor 3
			}  else if(adc_presence_check1 == 1) {
				adc0_measurementskip_sensor1flag = 1; //means there is no sensor2
				sensor_countbuffer[0] = 0;
				sensor_countbuffer[2] = 1; //there is sensor 3
				setNextState(REQUEST_MEASUREMENT2); //go and take measuremetn for sensor 4
			} else if(adc_presence_check2 == 1){
				adc1_measurementskip_sensor3flag = 1; //means there is no sensor4
				sensor_countbuffer[0] = 1; //there is sensor 1
				sensor_countbuffer[2] = 0;
				setNextState(REQUEST_MEASUREMENT2); //go and take measurement for sensor 2
			}else {
				sensor_countbuffer[0] = 1;
				sensor_countbuffer[2] = 1;
				setNextState(REQUEST_MEASUREMENT2); //if both sensors are populated
			}
		}
		break;
	case REQUEST_MEASUREMENT2:
//		emberAfAppPrintln("REQUEST_MEASUREMENT2");

		if(channel_call_flag == 1) {
				ADCRequestCh4(ADC0);
				ADCRequestCh4(ADC1);
		} else {
					ADCRequestCh2(ADC0);
					ADCRequestCh2(ADC1);
		}

		break;

	case FETCH_MEASUREMENT2:
		if(channel_call_flag == 1){  //If sensor is exist read the channel
		//	adc_presence_check1 =
			if(!adc0_measurementskip_sensor2flag)
				ADCReadChannelFour(ADC0);
			//adc_presence_check2 =
			if(!adc1_measurementskip_sensor4flag)
				ADCReadChannelFour(ADC1);
			else {
				ADCReadChannelFour(ADC0);
				ADCReadChannelFour(ADC1);
			}
			setNextStateWithDelay(TRANSMIT, 500);
		} else { // Read the channel, if there is no response just loop back with a delay of 1s until finds the sensor.
			adc_presence_check1 = ADCReadChannelTwo(ADC0);
			adc_presence_check2 = ADCReadChannelTwo(ADC1);
			if(( adc_presence_check1 == 1) && (adc_presence_check2 == 1)){
				emberAfAppPrintln("ADC0 & ADC1 Channel 2 Fail");
				repeatStateWithDelay(500);
			} else {
				setNextStateWithDelay(TRANSMIT, 500);
			}
		}

		break;

	  case TRANSMIT:  //think about this when sensor 2 and sensor 4 are missing or any of the sensor from 2 and 4 is missing hpow to log data and how to skip the cycle of the measuremtn
		  //or how to handle one of the sensors measure when there are 3 sensors
		  if((adc0_sensor_select == SENSOR1) && (adc1_sensor_select == SENSOR3)) {
			  logData(adc0_sensor_select); //adc0
			  logData(adc1_sensor_select); //adc1
		  } else  {
			  if((!adc0_measurementskip_sensor2flag) && (!adc1_measurementskip_sensor4flag))	{ //this will be called when there is 4 sensors
			  		logData(adc0_sensor_select); //adc0
			  		logData(adc1_sensor_select); //adc1
			  	} else if((!adc0_measurementskip_sensor2flag)) { //
			  		logData(adc0_sensor_select); //adc0

			  	} else if(!adc1_measurementskip_sensor4flag)
			  		logData(adc1_sensor_select); //adc1
			  	else
			  		emberAfAppPrintln("SENSOR2 & SENSOR 4 are not exist");
		  }

//		  printData();
	/*
*/
		  if(count < 5 ) {//&& adc_select == 0){   //0 1 2  3 4
		  //			  emberAfAppPrintln("IF 1");
		  	//	adc_select = 1;
				 if((adc0_sensor_select == SENSOR1) && (adc1_sensor_select == SENSOR3)) {
					adc0_sensor_select = SENSOR2;
					adc1_sensor_select = SENSOR4;
					channel_call_flag = 1;
				} else{
					adc0_sensor_select = SENSOR1;
					adc1_sensor_select = SENSOR3;
					channel_call_flag = 0;
					count++;  // 1 2 3 4
				}
				setNextState(INITIAL);
		  	} /*else if(count < 4 && adc_select == 1){
		  //			  emberAfAppPrintln("IF 2");
		  		adc_select = 0;
		  		count ++;
		  		setNextState(INITIAL);
		  	} */
		  //else { //check whther this else is needed or not
		  if((count == 5) && (gas_measurement_fanspinflag != 1)){
			  count = 0;
			  gas_measurement_fanspinflag = 2;
			  adc0_sensor_select = SENSOR1;
			  adc1_sensor_select = SENSOR3;
			  channel_call_flag = 0;
			  setNextState(INITIAL);
		  } else{
			  	  // if it comes back from Enqueue data, this Transmit has to be called from AIr-quality-control.c's aqmMasterEventControl event handler.
			 // GasSensor_DataPackaging();
			  count = 0;
			  gas_measurement_fanspinflag = 1;
			  channel_call_flag = 0;
			  adc0_measurementskip_sensor2flag = 0;
			  adc1_measurementskip_sensor4flag = 0;
			//  adc_select = 0;
			  if(!low_power_mode){
				  Solenoid(1,SOLENOID_CLOSE);
			  }
			  if(repeat){
				  setNextStateWithDelay(INITIAL, interval);
			  }else{

//				  remotePurgeStart();
				//  for(int8_t i=0;i<4;i++) { //commented on 15/10/2021 to handle the payload for all 4 sensors
				  	si7021Gas();
				  	for(uint8_t i=0;i<4;i++){
				  		if(sensor_countbuffer[i] == 1)
				  			firstpopulatedsensor = i+1;
				  	}
				  	ADCDataPackaging(firstpopulatedsensor);
				  	remoteEnqueueStart();
				//  } //commented on 15/10/2021 to handle the payload for all 4 sensors
				  setNextState(INACTIVE);
			  }

		  }
//		  ADCdataPackaging();
//		  if(count < 9){
//			  setNextState(REQUEST_MEASUREMENT1);
//			  count++;
//		  }else if(repeat){
//			  Solenoid(1,0);
//			  count = 0;
//			  setNextStateWithDelay(INITIAL,interval);
//		  } else {
//			  count = 0;
//			  Solenoid(1,0);
//			  remotePurgeStart();
//			  setNextState(INACTIVE);
//		  }
		  break;
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
	//if(adc_select == 0){
	if(gas_measurement_fanspinflag != 1)
	{
	 switch(sensor_number)
	 {
		case 1:
			for(uint8_t i = 0; i < 3; i++){  //18bit - 3bytes for each read
				after_airblow_sensor1_data[0][(count*3)+i] = adc0_ch1_data_buffer[i];
				after_airblow_sensor1_data[1][(count*3)+i] = adc0_ch2_data_buffer[i];
			}
			break;
		case 2:
				for(uint8_t i = 0; i < 3; i++){
					after_airblow_sensor2_data[0][(count*3)+i] = adc0_ch3_data_buffer[i];
					after_airblow_sensor2_data[1][(count*3)+i] = adc0_ch4_data_buffer[i];
				}
		  break;
		case 3:
				for(uint8_t i = 0; i < 3; i++){
					after_airblow_sensor3_data[0][(count*3)+i] = adc1_ch1_data_buffer[i];
					after_airblow_sensor3_data[1][(count*3)+i] = adc1_ch2_data_buffer[i];
				}
			break;
		case 4:
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
			case 1:
				for(uint8_t i = 0; i < 3; i++){  //18bit - 3bytes for each read
					before_airblow_sensor1_data[0][(count*3)+i] = adc0_ch1_data_buffer[i];
					before_airblow_sensor1_data[1][(count*3)+i] = adc0_ch2_data_buffer[i];
				}
				break;
			case 2:
				for(uint8_t i = 0; i < 3; i++){
					before_airblow_sensor2_data[0][(count*3)+i] = adc0_ch3_data_buffer[i];
					before_airblow_sensor2_data[1][(count*3)+i] = adc0_ch4_data_buffer[i];
				}
				break;
			case 3:
				for(uint8_t i = 0; i < 3; i++){
					before_airblow_sensor3_data[0][(count*3)+i] = adc1_ch1_data_buffer[i];
					before_airblow_sensor3_data[1][(count*3)+i] = adc1_ch2_data_buffer[i];
				}
				break;
			case 4:
				for(uint8_t i = 0; i < 3; i++){
					before_airblow_sensor4_data[0][(count*3)+i] = adc1_ch3_data_buffer[i];
					before_airblow_sensor4_data[1][(count*3)+i] = adc1_ch4_data_buffer[i];
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

/**Aidan*
 *
 * Initialises i2c communications and sets up both channels of the MCP3423 as follows:
 *
 *  - 'one shot' conversion mode
 *  - 18 bit sample rate
 *  - 8x PGA gain
 *
 */

/*static void ADCInitialise(uint8_t adc_select_in){
	halI2cInitialize();
	switch(SENSOR_SELECT_FLAG) {
		case 1:  // adc_gain for NO2/SO2
			adc_gain = 2;// = 4;
			adc_gain_a = 2;// = 4;
			adc_gain_b = 2;// = 4;
		break;
		case 2: // adc_gain for NO2/O3
			adc_gain = 4;
			adc_gain_a =  4;
			adc_gain_b = 4;
		break;
		case 3: // adc_gain for NO2/CO
			adc_gain = 4;
			adc_gain_a =  4;
			adc_gain_b = 4;
		break;
		case 4: // adc_gain for NO2/H2S  example for test purpose I am assuming
			adc_gain = 4;
			adc_gain_a =  4;
			adc_gain_b = 4;
		break;
		default: // default adc_gain is set for NO2/O3
			adc_gain = 4;
			adc_gain_a = 4;
			adc_gain_b = 4;
		break;
	}
	if(adc_select_in == 0){
		adc_gain = adc_gain_a;
	} else {
		adc_gain = adc_gain_b;
	}

	switch (adc_gain){
	case 1:
		channel_config_register1 = channel_config_register1_G1;
		channel_config_register2 = channel_config_register2_G1;
		channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G1;
		channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G1;
		break;

	case 2:
		channel_config_register1 = channel_config_register1_G2;
		channel_config_register2 = channel_config_register2_G2;
		channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G2;
		channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G2;
		break;

	case 4:
		channel_config_register1 = channel_config_register1_G4;
		channel_config_register2 = channel_config_register2_G4;
		channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G4;
		channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G4;
		break;

	case 8:
		channel_config_register1 = channel_config_register1_G8;
		channel_config_register2 = channel_config_register2_G8;
		channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G8;
		channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G8;
		break;

	default:
		channel_config_register1 = channel_config_register1_G1;
		channel_config_register2 = channel_config_register2_G1;
		channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G1;
		channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G1;
		break;
	}
	if(adc_select_in == 0){
		halI2cWriteBytes(MCP3423_0_WRITE_ADDRESS, &channel_config_register1, 1);
		halI2cWriteBytes(MCP3423_0_WRITE_ADDRESS, &channel_config_register2, 1);
	} else {
		halI2cWriteBytes(MCP3423_1_WRITE_ADDRESS, &channel_config_register1, 1);
		halI2cWriteBytes(MCP3423_1_WRITE_ADDRESS, &channel_config_register2, 1);
	}

}

*/

//static void ADCInitialise(uint8_t adc_select_in)

//This function is fine but need to test with hardware
static void ADCInitialise(uint8_t adc_select_in, uint8_t sensor_number){
	halI2cInitialize();
	switch(sensor_number) {  // as for schematic s1- O3 s2 - NO2 s3 - SO2 s4-optional
		case 1:  // adc_gain for O3
			adc_gain = 4;
			adc_gain_a =  4;
			adc_gain_b  = 4;
		break;
		case 2: // adc_gain for NO2
			adc_gain = 4;
			adc_gain_a =  4;
			adc_gain_b = 4;
		break;
		case 3: // default adc_gain is set for SO2
			adc_gain = 2;
			adc_gain_a = 2;
			adc_gain_b = 2;
		case 4: // Assuming as gain 2
			adc_gain = 2;
			adc_gain_a = 2;
			adc_gain_b = 2;
		default: // default adc_gain is set for SO2
			adc_gain = 4;
			adc_gain_a = 4;
			adc_gain_b = 4;
		break;
	}

	if(adc_select_in == 0){
		adc_gain = adc_gain_a;
	} else {
		adc_gain = adc_gain_b;
	}

	switch(sensor_number)
	{
		case 1: case 3: default:
			switch (adc_gain){
				case 1:
					channel_config_register1 = channel_config_register1_G1;
					channel_config_register2 = channel_config_register2_G1;
					channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G1;
					channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G1;
					break;

				case 2:
					channel_config_register1 = channel_config_register1_G2;
					channel_config_register2 = channel_config_register2_G2;
					channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G2;
					channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G2;
					break;

				case 4:
					channel_config_register1 = channel_config_register1_G4;
					channel_config_register2 = channel_config_register2_G4;
					channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G4;
					channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G4;
					break;

				case 8:
					channel_config_register1 = channel_config_register1_G8;
					channel_config_register2 = channel_config_register2_G8;
					channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G8;
					channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G8;
					break;

				default:
					channel_config_register1 = channel_config_register1_G1;
					channel_config_register2 = channel_config_register2_G1;
					channel_1_check_byte = CHANNEL_1_CHECK_BYTE_G1;
					channel_2_check_byte = CHANNEL_2_CHECK_BYTE_G1;
					break;
			}
			break;
		case 2: case 4:
			switch (adc_gain){
					case 1:
						channel_config_register1 = CHANNEL_3_CONFIG_REGISTER_G1;
						channel_config_register2 = CHANNEL_4_CONFIG_REGISTER_G1;
						channel_1_check_byte = CHANNEL_3_CHECK_BYTE_G1;
						channel_2_check_byte = CHANNEL_4_CHECK_BYTE_G1;
						break;

					case 2:
						channel_config_register1 = CHANNEL_3_CONFIG_REGISTER_G2;
						channel_config_register2 = CHANNEL_4_CONFIG_REGISTER_G2;
						channel_1_check_byte = CHANNEL_3_CHECK_BYTE_G2;
						channel_2_check_byte = CHANNEL_4_CHECK_BYTE_G2;
						break;

					case 4:
						channel_config_register1 = CHANNEL_3_CONFIG_REGISTER_G4;
						channel_config_register2 = CHANNEL_4_CONFIG_REGISTER_G4;
						channel_1_check_byte = CHANNEL_3_CHECK_BYTE_G4;
						channel_2_check_byte = CHANNEL_4_CHECK_BYTE_G4;
						break;

					case 8:
						channel_config_register1 = CHANNEL_3_CONFIG_REGISTER_G8;
						channel_config_register2 = CHANNEL_4_CONFIG_REGISTER_G8;
						channel_1_check_byte = CHANNEL_3_CHECK_BYTE_G8;
						channel_2_check_byte = CHANNEL_4_CHECK_BYTE_G8;
						break;

					default:
						channel_config_register1 = CHANNEL_3_CONFIG_REGISTER_G1;
						channel_config_register2 = CHANNEL_4_CONFIG_REGISTER_G1;
						channel_1_check_byte = CHANNEL_3_CHECK_BYTE_G1;
						channel_2_check_byte = CHANNEL_4_CHECK_BYTE_G1;
						break;
					}
			break;
		}
		switch(sensor_number)
		{
			case 1:
				adc0_ch1_config_register = channel_config_register1;
				adc0_ch2_config_register = channel_config_register2;
				adc0_ch1_check_byte = channel_1_check_byte;
				adc0_ch2_check_byte = channel_2_check_byte;
				break;
			case 2:
				adc0_ch3_config_register = channel_config_register1;
				adc0_ch4_config_register = channel_config_register2;
				adc0_ch3_check_byte = channel_1_check_byte;
				adc0_ch4_check_byte = channel_2_check_byte;
				break;
			case 3:
				adc1_ch1_config_register = channel_config_register1;
				adc1_ch2_config_register = channel_config_register2;
				adc1_ch1_check_byte = channel_1_check_byte;
				adc1_ch2_check_byte = channel_2_check_byte;
				break;
			case 4:
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
			halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &channel_config_register1, 1);
			halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &channel_config_register2, 1);
		}

}

static void ADCRequestCh1(uint8_t adc_select_in){

	if(adc_select_in == 0){
		halI2cWriteBytes(MCP3424_0_WRITE_ADDRESS, &adc0_ch1_config_register, 1);
	} else {
		halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &adc1_ch1_config_register, 1);
	}
}

static void ADCRequestCh2(uint8_t adc_select_in){
	if(adc_select_in == 0){
		halI2cWriteBytes(MCP3424_0_WRITE_ADDRESS, &adc0_ch2_config_register, 1);
	} else {
		halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &adc1_ch2_config_register, 1);
	}
}

static void ADCRequestCh3(uint8_t adc_select_in){

	if(adc_select_in == 0){
		halI2cWriteBytes(MCP3424_0_WRITE_ADDRESS, &adc0_ch3_config_register, 1);
	} else {
		halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &adc1_ch3_config_register, 1);
	}
}

static void ADCRequestCh4(uint8_t adc_select_in){
	if(adc_select_in == 0){
		halI2cWriteBytes(MCP3424_0_WRITE_ADDRESS, &adc0_ch4_config_register, 1);
	} else {
		halI2cWriteBytes(MCP3424_1_WRITE_ADDRESS, &adc1_ch4_config_register, 1);
	}
}

void databuffer_empty(uint8_t buffer[]) {
	for(int8_t i; i< sizeof(buffer);i++)
	{
		buffer[i] = '\0';
	}
}
/**Aidan*
 *
 * Reads from channel one of the ADC and checks the returned 'Configuration Register' bitfield
 * to ensure that latest reading has been taken using the correct settings as stated above.
 *
 */
static uint8_t ADCReadChannelOne(uint8_t adc_select_in){

	if(adc_select_in == 0){
		databuffer_empty(adc0_ch1_data_buffer);
		halI2cReadBytes(MCP3424_0_READ_ADDRESS, &adc0_ch1_data_buffer, 4);
		if(adc0_ch1_data_buffer[3] == adc0_ch1_check_byte){
			return 0;
		} else {
			return 1;
		}
	} else {
		databuffer_empty(adc1_ch1_data_buffer);
		halI2cReadBytes(MCP3424_1_READ_ADDRESS, &adc1_ch1_data_buffer, 4);
		if(adc1_ch1_data_buffer[3] == adc1_ch1_check_byte){
			return 0;
		} else {
			return 1;
		}
	}

}

/**Aidan*
 *
 * Reads from channel one of the ADC and checks the returned 'Configuration Register' bitfield
 * to ensure that latest reading has been taken using the correct settings as stated above.
 *
 */

static uint8_t ADCReadChannelTwo(uint8_t adc_select_in){

	if(adc_select_in == 0){
		databuffer_empty(adc0_ch2_data_buffer);
		halI2cReadBytes(MCP3424_0_READ_ADDRESS, &adc0_ch2_data_buffer, 4);
		if(adc0_ch2_data_buffer[3] == adc0_ch2_check_byte){
			return 0;
		} else {
			return 1;
		}
	} else {
		databuffer_empty(adc1_ch2_data_buffer);
		halI2cReadBytes(MCP3424_1_READ_ADDRESS, &adc1_ch2_data_buffer, 4);
		if(adc1_ch2_data_buffer[3] == adc1_ch2_check_byte){
			return 0;
		} else {
			return 1;
		}
	}

}

/**Aidan*
 *
 * Reads from channel one of the ADC and checks the returned 'Configuration Register' bitfield
 * to ensure that latest reading has been taken using the correct settings as stated above.
 *
 */
static uint8_t ADCReadChannelThree(uint8_t adc_select_in){

	if(adc_select_in == 0){
		databuffer_empty(adc0_ch3_data_buffer);
		halI2cReadBytes(MCP3424_0_READ_ADDRESS, &adc0_ch3_data_buffer, 4);
		if(adc0_ch3_data_buffer[3] == adc0_ch3_check_byte){
			return 0;
		} else {
			return 1;
		}
	} else {
		databuffer_empty(adc1_ch3_data_buffer);
		halI2cReadBytes(MCP3424_1_READ_ADDRESS, &adc1_ch3_data_buffer, 4);
		if(adc1_ch3_data_buffer[3] == adc1_ch3_check_byte){
			return 0;
		} else {
			return 1;
		}
	}

}

/**Aidan*
 *
 * Reads from channel one of the ADC and checks the returned 'Configuration Register' bitfield
 * to ensure that latest reading has been taken using the correct settings as stated above.
 *
 */

static uint8_t ADCReadChannelFour(uint8_t adc_select_in){

	if(adc_select_in == 0){
		databuffer_empty(adc0_ch4_data_buffer);
		halI2cReadBytes(MCP3424_0_READ_ADDRESS, &adc0_ch4_data_buffer, 4);
		if(adc0_ch4_data_buffer[3] == adc0_ch4_check_byte){
			return 0;
		} else {
			return 1;
		}
	} else {
		databuffer_empty(adc1_ch4_data_buffer);
		halI2cReadBytes(MCP3424_1_READ_ADDRESS, &adc1_ch4_data_buffer, 4);
		if(adc1_ch4_data_buffer[3] == adc1_ch4_check_byte){
			return 0;
		} else {
			return 1;
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

//void ADCDataPackaging(void){
void ADCDataPackaging(uint8_t sensorcount){
//	uint8_t payload[66];
	//emberAfAppPrintln("I am ADCDataPackaging");
		payload[0] = air_temp >> 8;
		payload[1] = air_temp;

		payload[2] = air_humidity >> 8;
		payload[3] = air_humidity;

		payload[4] = (adc_gain_a << 4) | adc_gain_b;
		payload[5] = sample_rate;
		switch(sensorcount)
		{
			case 1:
				//no2 working electrode data
				for(uint8_t i = 6; i < 21; i++){
					payload[i] = before_airblow_sensor1_data[0][i-6];
					payload[i+15] = before_airblow_sensor1_data[1][i-6];
					payload[i+30] = after_airblow_sensor1_data[0][i-6];
					payload[i+45] = after_airblow_sensor1_data[1][i-6];
				} break;
			case 2:
				//no2 working electrode data
				for(uint8_t i = 6; i < 21; i++){
					payload[i] = before_airblow_sensor2_data[0][i-6];
					payload[i+15] = before_airblow_sensor2_data[1][i-6];
					payload[i+30] = after_airblow_sensor2_data[0][i-6];
					payload[i+45] = after_airblow_sensor2_data[1][i-6];
				} break;
			case 3:
				//no2 working electrode data
				for(uint8_t i = 6; i < 21; i++){
					payload[i] = before_airblow_sensor3_data[0][i-6];
					payload[i+15] = before_airblow_sensor3_data[1][i-6];
					payload[i+30] = after_airblow_sensor3_data[0][i-6];
					payload[i+45] = after_airblow_sensor3_data[1][i-6];
				} break;
			case 4:
				//no2 working electrode data
				for(uint8_t i = 6; i < 21; i++){
					payload[i] = before_airblow_sensor4_data[0][i-6];
					payload[i+15] = before_airblow_sensor4_data[1][i-6];
					payload[i+30] = after_airblow_sensor4_data[0][i-6];
					payload[i+45] = after_airblow_sensor4_data[1][i-6];
				} break;
		}
				//no2 auxiliary electrode data
			/*	for(uint8_t i = 21; i < 36; i++){
					payload[i] = before_airblow_sensor1_data[1][i-21];
				}

				//o3 working electrode data
				for(uint8_t i = 36; i < 51; i++){
					payload[i] = before_airblow_sensor1_data[0][i-36];
				}

				//o3 working electrode data
				for(uint8_t i = 51; i < 66; i++){
					payload[i] = before_airblow_sensor1_data[1][i-51];
				}*/
//writing this to finish the
	printData();
	gasDataLog(payload);

//	VMN_payloadPackager(&global_server, payload, 66, DAT, 0x0016);
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


	emberAfAppPrint("DAT-ALPHAGAS,	");
	emberAfAppPrint("TEMP=%d,	", gas_temp);
	emberAfAppPrint("HUM=%d,	",gas_humidity);
	emberAfAppPrint("ADC-GAIN=%d,	",gas_adc_gain);
	emberAfAppPrint("ADC-SAMPLE-RATE=%d,	",gas_adc_sample_rate);

	emberAfAppPrint("NO2-WORK=");
	for(uint8_t i = 6; i < 21; i += 3){
		no2_temp = payload[i] & 0x01; //0x03;
		no2_sign = payload[i] & 0x80;

		no2_value = 	(no2_sign << 24 |
						 no2_temp << 16 |
						 payload[i+1] << 8 |
						 payload[i+2]);
		emberAfAppPrint("%d:%d,	", (i-6)/3, no2_value);
	}
	emberAfAppPrint("NO2-AUX=");
	for(uint8_t i = 21; i < 36; i += 3){
		no2_temp = payload[i] & 0x01; //0x03;
		no2_sign = payload[i] & 0x80;

		no2_value = 	(no2_sign << 24 |
						 no2_temp << 16 |
						 payload[i+1] << 8 |
						 payload[i+2]);
		emberAfAppPrint("%d:%d,	", (i-21)/3, no2_value);
	}
if(SENSOR_SELECT_FLAG == 1)
	emberAfAppPrint("SO2-WORK=");
if(SENSOR_SELECT_FLAG == 2)
	emberAfAppPrint("O3-WORK=");
	for(uint8_t i = 36; i < 51; i += 3){
		o3_temp = payload[i] & 0x01; //0x03;
		o3_sign = payload[i] & 0x80;

		o3_value = 	(o3_sign << 24 |
						 o3_temp << 16 |
						 payload[i+1] << 8 |
						 payload[i+2]);
		emberAfAppPrint("%d:%d,	", (i-36)/3, o3_value);
	}
	if(SENSOR_SELECT_FLAG == 1)
		emberAfAppPrint("SO2-AUX=");
	if(SENSOR_SELECT_FLAG == 2)
		emberAfAppPrint("O3-AUX=");
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

