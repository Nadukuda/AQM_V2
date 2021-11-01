/*
 * VMN_RTCC.h
 *
 *  Created on: 14 Nov 2019
 *      Author: Aidan
 */

/*
#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#include EMBER_AF_API_HAL
#include EMBER_AF_API_COMMAND_INTERPRETER2
#ifdef EMBER_AF_API_DEBUG_PRINT
  #include EMBER_AF_API_DEBUG_PRINT
#endif
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "bsp.h"
#include "em_emu.h"
#include "em_rtcc.h"
#include "em_gpio.h"
#include "Data_Protocol/VMN_globals.h"
#include "Data_Protocol/VMN_definitions.h"
#include "VMN/Data_Queue/Queue.h"

#define START_TIME      0x00150000      // 15:00:00 //
#define START_DATE      0x06130727      // 2013 July 27 /

uint32_t VMN_time = START_TIME;
uint32_t VMN_date = START_DATE;

static uint8_t cal_sec = 45, cal_min = 59,cal_hour = 23, cal_day = 28, cal_month = 2,cal_year = 16;
static uint8_t temp_sec,temp_min,temp_hour,temp_day,temp_month,temp_year;
static uint32_t last_RTCC_val;

uint32_t VMN_rtccGetTime(void);
uint32_t VMN_rtccGetDate(void);
void VMN_getDateTime(void);
void VMN_rtccSetDateTime(uint8_t *payload);

//uint32_t rtccFlag;



void rtccSetup(void)
{
//	RTCDRV_DeInit();

	  RTCC_Enable(false);
	  RTCC_Unlock();
	  // Configure the RTCC settings
	  RTCC_Init_TypeDef vmn_rtcc = RTCC_INIT_DEFAULT;
	  vmn_rtcc.enable   = false;
	  vmn_rtcc.presc = rtccCntPresc_32768;
	  vmn_rtcc.cntMode = rtccCntModeCalendar;
	  vmn_rtcc.cntWrapOnCCV1 = true;

	  // Configure the compare settings
	  RTCC_CCChConf_TypeDef compare = RTCC_CH_INIT_COMPARE_DEFAULT;

	  // Turn on the clock for the RTCC
//	  CMU_ClockEnable(cmuClock_HFLE, true);
//	  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
//	  CMU_ClockEnable(cmuClock_RTCC, true);

	  CMU_ClockEnable(cmuClock_HFLE, true);
	  CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFXO);
	  CMU_ClockEnable(cmuClock_RTCC, true);


	  // Initialise RTCC with pre-defined settings
	  RTCC_Init(&vmn_rtcc);

	  // Set current date and time
	  RTCC_DateSet(START_DATE);
	  RTCC_TimeSet(START_TIME);

	  // Initialise RTCC compare with a date, the date when interrupt will occur
//	  RTCC_ChannelInit(1, &compare);
//	  RTCC_ChannelDateSet(1, alarmd_start);
//	  RTCC_ChannelTimeSet(1,alarmh_start);

	  // Set channel 1 to cause an interrupt
//	  RTCC_IntEnable(RTCC_IEN_CC1);
//	  NVIC_ClearPendingIRQ(RTCC_IRQn);
//	  NVIC_EnableIRQ(RTCC_IRQn);

	  // Start counter after all initialisations are complete
	  RTCC_Enable(true);


  emberAfAppPrintln("RTCC ENABLE");
}



void rtccPrintDate(void)
{
	VMN_rtccUpdate();
	emberAfAppPrintln("DATE:%02d/%02d/%02d",
			((uint8_t)(VMN_rtccGetDate() >> 16)),
			((uint8_t)(VMN_rtccGetDate() >> 8)),
			((uint8_t)(VMN_rtccGetDate())));

}


void rtccPrintDateTime(void)
{
  VMN_rtccUpdate();
//  emberAfAppPrintln("Time:%d",rtccTemp);
	emberAfAppPrintln("DATE:%02d/%02d/20%02d",
			((uint8_t)(VMN_rtccGetDate())),
			((uint8_t)(VMN_rtccGetDate() >> 8)),
			((uint8_t)(VMN_rtccGetDate() >> 16))
			);
//  emberAfAppPrintln("%02d/%02d/%02-%02d:%02d:%02d",
//          ((uint8_t)(VMN_time >> 16)),
//          ((uint8_t)(VMN_time >> 8)),
//          ((uint8_t)VMN_time));
  emberAfAppPrintln("time = %02d:%02d:%02d",
			((uint8_t)(VMN_rtccGetTime() >> 16)),
			((uint8_t)(VMN_rtccGetTime() >> 8)),
			((uint8_t)(VMN_rtccGetTime())));

//  emberAfAppPrintln("VMN_date = %d/%d/20%d", cal_day,cal_month,cal_year);
}

void VMN_rtccUpdate(void){
	  uint32_t rtccTemp, elapsed_time;

	  rtccTemp = RTCDRV_TicksToSec(RTCDRV_GetWallClockTicks64());

	  elapsed_time = rtccTemp - last_RTCC_val;

	  last_RTCC_val = rtccTemp;

	  temp_sec = elapsed_time % 60;
	  temp_min = (elapsed_time/60) % 60;
	  temp_hour = (elapsed_time/60/60) %24;
	  temp_day	= (elapsed_time/60/60/24);

	  cal_sec += temp_sec;
	  if (cal_sec >= 60) {
	    cal_sec -= 60;
	    cal_min++;
	  }

	  cal_min += temp_min;
	  if (cal_min >= 60) {
	    cal_min -= 60;
	    cal_hour++;
	  }

	  cal_hour += temp_hour;
	  if (cal_hour >= 24) {
	    cal_hour -= 24;
	    cal_day++;
	  }

	  cal_day += temp_day;

	  switch (cal_month) {
	    case 1:
	    case 3:
	    case 5:
	    case 7:
	    case 8:
	    case 10:
	      if (cal_day > 31) {
	        cal_day -= 31;
	        cal_month++;
	      }
	      break;

	    case 12:
	      if (cal_day > 31) {
	        cal_day -= 31;
	        cal_month = 1;
	        cal_year++;
	      }
	      break;

	    case 4:
	    case 6:
	    case 9:
	    case 11:
	      if (cal_day > 30) {
	        cal_day -= 30;
	        cal_month++;
	      }
	      break;

	    case 2:
	      // Leap year check
	      if (cal_year % 4 == 0 && (cal_year % 100 != 0 || cal_year % 400 == 0)) {
	        if (cal_day > 29) {
	          cal_day -= 29;
	          cal_month++;
	        }
	      } else {
	        if (cal_day > 28) {
	          cal_day -= 28;
	          cal_month++;
	        }
	      }
	      break;
	  }
}


uint32_t VMN_rtccGetTime(void){
	  VMN_time = (cal_hour << 16)	|
			  	 (cal_min << 8)		|
				 (cal_sec);

	  return VMN_time;
}

uint32_t VMN_rtccGetDate(void){
	  VMN_date = (cal_year << 16)	|
			  	 (cal_month << 8)	|
				 (cal_day);

	  return VMN_date;
}

void VMN_rtccSetDateTime(uint8_t *payload){

	cal_year	= payload[6];
	cal_month	= payload[7];
	cal_day		= payload[8];
	cal_hour 	= payload[9];
	cal_min	 	= payload[10];
	cal_sec  	= payload[11];
	VMN_rtccGetDateTime();
}


void VMN_rtccGetDateTime(void){
	VMN_rtccUpdate();
	uint8_t dt_payload[6] = {
			cal_year,
			cal_month,
			cal_day,
			cal_hour,
			cal_min,
			cal_sec
	};
	rtccPrintDateTime();
	VMN_payloadPackager(&global_server, dt_payload, sizeof(dt_payload), DAT, 0x0028);
}
*/

/*
 * VMN_RTCC.h
 *
 *  Created on: 14 Nov 2019
 *      Author: AShravanthi
 */


#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#include EMBER_AF_API_HAL
#include EMBER_AF_API_COMMAND_INTERPRETER2
#ifdef EMBER_AF_API_DEBUG_PRINT
  #include EMBER_AF_API_DEBUG_PRINT
#endif
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "bsp.h"
#include "em_emu.h"
#include "em_rtcc.h"
#include "em_gpio.h"
#include "Data_Protocol/VMN_globals.h"
#include "Data_Protocol/VMN_definitions.h"
#include "VMN/Data_Queue/Queue.h"
#include "time.h"

#define START_TIME      0x00150520//0x00150000      /* 15:00:00 */ 00HMS
#define START_DATE      0x20210916 //0x06130727      /* 2013 July 27 */ //  /YYMD

uint32_t VMN_time = START_TIME;
uint32_t VMN_date = START_DATE;

static uint8_t cal_sec = 30, cal_min = 30,cal_hour = 14, cal_day = 28, cal_month = 10, cal_year = 21; //after update
//static uint32_t last_RTCC_val = 0;
//uint32_t rtcc_flag,rtcc_time,rtcc_date,rtccResetVal=0,rtccResetTs=0,bodCnt=0,buTime=0,bodTime=0,flagrtcc=0;
//extern char timebuffer;
//char month_table[13][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

struct tm timeinfo;
time_t seconds;
uint8_t rtc_datetime_setflag = 0;
uint32_t rtcc_tickstosec = 0;
uint32_t rtccTemp = 0;
uint32_t elapsed_time = 0;

uint32_t VMN_rtccGetTime(void);
uint32_t VMN_rtccGetDate(void);
//void VMN_getDateTime(void);
void VMN_rtccSetDateTime(uint8_t *payload);
void VMN_rtccUpdate(void);

uint8_t rtccfunctioncall_entryflag=1;

/**************************************************************************//**
* @brief Setup RTCC for calendar.
*****************************************************************************/
/*  commnted this function on 04/10/2021 by shravanthi because this function is not called anywhere to setup the RTCC
void rtccSetup(void)
{
	  CMU_ClockEnable(cmuClock_CORELE, true);
	  CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFXO);//cmuSelect_LFRCO);
	  CMU_ClockEnable(cmuClock_RTCC, true);  // Enable RTCC module clock.
	  // Configure the RTCC settings
	  RTCC_Init_TypeDef vmn_rtcc = RTCC_INIT_DEFAULT;
	  vmn_rtcc.enable   = false;
	  vmn_rtcc.presc = rtccCntPresc_32768;
	  vmn_rtcc.cntMode = rtccCntModeNormal;//rtccCntModeCalendar;
	  vmn_rtcc.prescMode = rtccCntTickPresc; //CNT register ticks according to configuration in CNTPRESC.
	  vmn_rtcc.cntWrapOnCCV1 = false;//true;
	  vmn_rtcc.precntWrapOnCCV0 = false;//true;
	  vmn_rtcc.disLeapYearCorr = true; //leap year correction
	  vmn_rtcc.enaOSCFailDetect = false;//true;
	  vmn_rtcc.debugRun = false; //RTCC halt on debug

	  RTCC_Init(&vmn_rtcc);

	//  emberAfAppPrintln("R2");
	  // Set current date and time
	  RTCC_DateSet(START_DATE);
	  RTCC_TimeSet(START_TIME);

	//  halInternalEm4Wakeup();
	//  RTCC_EM4WakeupEnable(true);
//	  emberAfAppPrintln("R3");
	  RTCC_IntDisable(_RTCC_IF_MASK);
	  RTCC_IntClear(_RTCC_IF_MASK);
	  //The interrupt flag CNTTICK is set each time the main counter receives a tick (each second in calendar mode). In calendar mode, there
	 // are also interrupt flags being set each minute, hour, day, week, and month.
	  RTCC_IntEnable(RTCC_IEN_CNTTICK); //RTCC_IEN_CC1
	  NVIC_ClearPendingIRQ(RTCC_IRQn);
	  NVIC_EnableIRQ(RTCC_IRQn);


	  // Start counter after all initialisations are complete
	  RTCC_Enable(true);
  emberAfAppPrintln("RTCC ENABLE");
}
*/

/* below functions are used to get the current date &time by passing the date &time through the string buffer
int rtcmonth(char *input_month) {
    int i=0;
    int month;
    for( i = 0; i < 12; i++) {
    //    printf("%s : %s ",month_table[i], input_month);
        if (!strncmp((char *)month_table[i], input_month,3)) {
            month = i;
            i = 12;
        }
    }
  //  printf("month %d\n",month);
    return month;
}

time_t time_in_seconds_standardclibrary(char *time_buffer) {//1 4 12 2021 12:50:02
	    char *remaining_data_after_tok;
	    char checkbyte = time_buffer[0];
	    char *test;
	    time_t secs;
	    // = 34936;

	    if(isalpha(checkbyte))
	    {
	     //   test = strtok_r(time_buffer, " ", &remaining_data_after_tok);
	    //    printf("month %s: \n",test);
	       timeinfo.tm_mon =  rtcmonth(strtok_r(time_buffer, " ", &remaining_data_after_tok));
	       timeinfo.tm_mday = atoi(strtok_r(remaining_data_after_tok, " ", &time_buffer));
	    }
	    else {
	        timeinfo.tm_mday = atoi(strtok_r(time_buffer, " ", &remaining_data_after_tok));   //after this 2021 12:50:02
	        timeinfo.tm_mon = (atoi(strtok_r(remaining_data_after_tok, " ", &time_buffer)) - 1);  //after this 12 2021 12:50:02
	    }
	   // printf("hi\n");
	 //   timeinfo.tm_wday = atol(strtok_r(time_buffer, " ", &remaining_data_after_tok));  //after this 4 12 2021 12:50:02
	    timeinfo.tm_year = (atoi(strtok_r(time_buffer, " ", &remaining_data_after_tok)) - 1900);   //after this tmebuffer with 12:50:02
	    timeinfo.tm_hour = atoi(strtok_r(remaining_data_after_tok, ":", &time_buffer));   //12
	    timeinfo.tm_min = atoi(strtok_r(time_buffer, ":", &remaining_data_after_tok));    //50
	    timeinfo.tm_sec = atoi(strtok_r(remaining_data_after_tok, ":", &time_buffer));    //02
	  //  timeinfo.tm_isdst = -1;
	    secs = mktime(&timeinfo);
	  //  bool res =  _rtc_maketime(time, seconds, leap_year_type);
	//    emberAfAppPrintln("internal sec %d  %d %d %d %d %d %u\n", timeinfo.tm_mday, timeinfo.tm_mon,(timeinfo.tm_year + 1900), timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, seconds);
	    return secs;
}
*/

//This function will convert the date and time into seconds by using the C standard library
time_t time_in_seconds(void) {//1 4 12 2021 12:50:02
	    time_t secs;
	    timeinfo.tm_mday = (int)cal_day;
	    timeinfo.tm_mon =  ((int)cal_month)-1;
	    timeinfo.tm_year = ((int )cal_year) + 100;
	    timeinfo.tm_hour = (int)cal_hour;
	    timeinfo.tm_min = (int)cal_min;
	    timeinfo.tm_sec = (int)cal_sec;
	    seconds = mktime(&timeinfo);
	    rtccfunctioncall_entryflag = 2;
	//    RTCDRV_SetWallClock(seconds);// added on 21/09/2021 to test RTC
	//    emberAfAppPrintln("internal sec %d  %d %d %d %d %d %u\n", timeinfo.tm_mday, timeinfo.tm_mon,(timeinfo.tm_year + 1900), timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, seconds);
	    return secs;
}
/**************************************************************************//**
* @brief Print RTCC date & time.
*****************************************************************************/
void rtccPrintDateTime(void)
{
  emberAfAppPrintln("DATE:%02d/%02d/20%02d", cal_day,cal_month,cal_year);
  emberAfAppPrintln("TIME = %02d:%02d:%02d", cal_hour,cal_min,cal_sec);
}
void rtctimestamp(void)
{
	emberAfAppPrint("DATE:%02d/%02d/20%02d  TIME = %02d:%02d:%02d", cal_day,cal_month,cal_year, cal_hour,cal_min,cal_sec);
}
//Updates the time added by Shravanthi to reduce the time drift.
void VMN_rtccUpdate(void){
	  time_t rtc_reading;// = seconds;
	  struct tm *datetime;
	  if(rtccfunctioncall_entryflag == 1) {
		  time_in_seconds();
		  rtccfunctioncall_entryflag = 2;
	  }
	  if(rtc_datetime_setflag == 2) {
		  rtccTemp = rtcc_tickstosec;// + elapsed_time;
		  rtc_datetime_setflag = 1;
	  }
	  rtcc_tickstosec = RTCDRV_TicksToSec(RTCDRV_GetWallClockTicks64());
	  elapsed_time = rtcc_tickstosec - rtccTemp;
	  rtc_reading =  (elapsed_time + seconds);
	  datetime = localtime(&rtc_reading);
	  cal_day = datetime->tm_mday;
	  cal_month = datetime->tm_mon + 1; //time.h starts months count from 0 to 11
	  cal_year = datetime->tm_year - 100; //2021 - 1900 = 121 so to get 21 here I am doing -100 from the results
	  cal_hour = datetime->tm_hour;
	  cal_min =datetime->tm_min;
	  cal_sec = datetime->tm_sec;
	//  emberAfAppPrintln(" VMN_rtccUpdate : %2d/%2d/20%d %2d:%2d:%2d", cal_day,cal_month,cal_year,cal_hour,cal_min,cal_sec);

}


uint32_t VMN_rtccGetTime(void){
	  VMN_time = (cal_hour << 16)	|
			  	 (cal_min << 8)		|
				 (cal_sec);
	  return VMN_time;
}

uint32_t VMN_rtccGetDate(void){
	  VMN_date = (cal_year << 16)	|
			  	 (cal_month << 8)	|
				 (cal_day);
	  return VMN_date;
}

void VMN_rtccSetDateTime(uint8_t *payload){

	//emberAfAppPrint ("timepayload %d %d %d %d %d %d", payload[6],payload[7],payload[8],payload[9],payload[10],payload[11]);
	cal_year	= payload[6];
	cal_month	= payload[7];
	cal_day		= payload[8];
	cal_hour 	= payload[9];
	cal_min	 	= payload[10];
	cal_sec  	= payload[11]; //"01 6 2021 17:49:00"
	time_in_seconds(); //converts the time in seconds
	rtc_datetime_setflag = 2;
	rtcc_tickstosec = RTCDRV_TicksToSec(RTCDRV_GetWallClockTicks64());
}

//This function is called by using the "get_datetime" cli command to get the updated Date & Time
void rtc_datetime_get(void)
{
	if(rtc_datetime_setflag == 0) {
		time_in_seconds();
		rtc_datetime_setflag = 1;
	//	emberAfAppPrintln( "get d&t: %u", seconds);
	}
	VMN_rtccGetDateTime();
}

//This function is called by using the "set_datetime" cli command to set the default time
void rtc_datetime_set(void)
{
	time_in_seconds(); //commented above line on 20/09/2021 to solve the time drift problem
	VMN_rtccUpdate();
}

/*
 *REQUIRES DEBUGGING
 *
 *DO NOT USE
 *
 */
void VMN_rtccGetDateTime(void){
	VMN_rtccUpdate();
	uint8_t dt_payload[6] = {
			cal_year,
			cal_month,
			cal_day,
			cal_hour,
			cal_min,
			cal_sec
	};
	rtccPrintDateTime();
	VMN_payloadPackager(&global_server, dt_payload, sizeof(dt_payload), DAT, 0x0028);
}
