/*
 * VMN_setup_parameters.h
 *
 *  Created on: 16 Aug 2019
 *      Author: Aidan
 */

#ifndef VMN_SETUP_PARAMETERS_H_
#define VMN_SETUP_PARAMETERS_H_

#define VMN_SETUP_NETWORK_NAME "VMN/SETUP"
#define VMN_SETUP_NETWORK_NAME_LENGTH (9)
#define VMN_SETUP_PAN_ID 5418 // decimal representation of hex PAN ID
#define VMN_SETUP_EXTENDED_PAN_ID { 0x7E, 0xC1, 0xE5, 0xDE, 0x5F, 0xA2, 0xE4, 0xE6 }
#define VMN_SETUP_ULA_PREFIX { 0xFD, 0xE6, 0x4C, 0xD3, 0xA4, 0xE0, 0x00, 0x01 }
#define VMN_SETUP_RADIO_TX_POWER 17
#define VMN_SETUP_CHANNEL 25
#define VMN_SETUP_MASTER_KEY { 0x56, 0x4D, 0x4E, 0x5F, 0x53, 0x45, 0x54, 0x55, 0x50, 0x4E, 0x45, 0x54, 0x57, 0x4F, 0x52, 0x4B }

#endif /* VMN_SETUP_PARAMETERS_H_ */
