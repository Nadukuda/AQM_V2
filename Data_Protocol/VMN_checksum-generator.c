/*
 * VMN_checksum-generator.c
 *
 *  Created on: 28 Nov 2018
 *      Author: Aidan
 */


#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include EMBER_AF_API_STACK
#include EMBER_AF_API_HAL
#include EMBER_AF_API_COMMAND_INTERPRETER2


/*
 *
 * Taken from VMN documentation
 *
 */
uint16_t checksumGenerator(uint8_t *input, int seq_length){

    uint16_t sum1 = 0;
    uint16_t sum2 = 0;

    for(int index = 0; index < seq_length; ++index ){
       sum1 = (sum1 + input [index]) % 255;
       sum2 = (sum2 + sum1) % 255;
    }
    return (sum2 << 8) | sum1;

}
