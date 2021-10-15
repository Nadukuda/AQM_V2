/*
 * Queue.h
 *
 *  Created on: 11 Nov 2019
 *      Author: Aidan
 */

#ifndef VMN_DATA_QUEUE_QUEUE_H_
#define VMN_DATA_QUEUE_QUEUE_H_



#endif /* VMN_DATA_QUEUE_QUEUE_H_ */

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK

#define MAX_SIZE 2042 // 7 days of measurements at 5 min intervals

uint8_t pm_payload[28];
uint8_t gas_payload[74];

boolean transmission_flag_pm;

enum{
	VMN_SUCCESS =	0,
	VMN_FAILURE =	1,
};

typedef struct {
//	uint32_t index;
	uint32_t time;
	uint32_t date;
	uint8_t pm_data[20];
	uint8_t gas_data[66];
}AirQualityData;

AirQualityData queue[MAX_SIZE];

