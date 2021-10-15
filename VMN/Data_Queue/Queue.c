/*
 * Queue.c
 *
 *  Created on: 11 Nov 2019
 *      Author: Aidan
 */

#include "Queue.h"

void Enqueue (AirQualityData aq_in);
uint8_t Dequeue(void);
AirQualityData Peek(void);
uint8_t dequeueRear(void);

int front = -1;
int rear = -1;
int size = -1;

uint8_t queueIsEmpty(void);

void Enqueue(AirQualityData aq_in){
    if(size<MAX_SIZE){
        if(size<0){
            queue[0] = aq_in;
            front = rear = 0;
            size = 1;
        }else if(rear == MAX_SIZE-1){
            queue[0] = aq_in;
            rear = 0;
            size++;
        }else{
            queue[rear+1] = aq_in;
            rear++;
            size++;
        }
    }else{
        emberAfAppPrintln("Queue is full");
    }
    remoteTransmitPmStart();
}



 uint8_t Dequeue(void){
    if(size<0){
        emberAfAppPrintln("Queue is empty");
    }else if(front == MAX_SIZE-1){
    	size--;
		front = 0;
    }else{
        size--;
        front++;
        return VMN_SUCCESS;
    }
    return VMN_FAILURE;
}

 uint8_t dequeueRear(void){
    if(size<0){
        emberAfAppPrintln("Queue is empty");
    }else{
        size--;
        rear--;
        emberAfAppPrintln("queue size = %d, rear position = %d", size,rear);
        return VMN_SUCCESS;
    }
    return VMN_FAILURE;
}

void getFront(AirQualityData *aq_data){
	 if(size < 0){
		 emberAfAppPrintln("Queue is empty");
	 }else{
//		 aq_data->index = *queue[front].index;
//		 emberAfAppPrintln("Queue Front - index = %d", queue[front].index);
//		 aq_data->index = queue[front].index;
		 aq_data->time = queue[front].time;
		 aq_data->date = queue[front].date;
		 memcpy(aq_data->pm_data, queue[front].pm_data, 20);
		 memcpy(aq_data->gas_data, queue[front].gas_data, 66);
//		 emberAfAppPrintln("AQ_DATA - index = %d", aq_data->index);
	 }
 }

 uint8_t queueIsEmpty(void){
	 if(size <= 0){
		 return 1;
	 } else {
		 return 0;
	 }
 }


 uint8_t emptyBuffer(void){
	 if(size <= 0){
		 // NOTHING TO SEND
	 } else {
//		 VMN_payloadPackager(&global_server, queue[front].pm_data, 20, DAT, 0x001A);
	 }
 }



