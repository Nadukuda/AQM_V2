/*
 * VNM_usart2.h
 *
 *  Created on: 7 Dec 2018
 *      Author: Aidan
 */

#ifndef DATA_PROTOCOL_VNM_USART2_H_
#define DATA_PROTOCOL_VNM_USART2_H_



#endif /* DATA_PROTOCOL_VNM_USART2_H_ */

// $[USART2]
#define PORTIO_USART2_CLK_PIN                 (8U)
#define PORTIO_USART2_CLK_PORT                (gpioPortA)
#define PORTIO_USART2_CLK_LOC                 (1U)

#define PORTIO_USART2_CS_PIN                  (9U)
#define PORTIO_USART2_CS_PORT                 (gpioPortA)
#define PORTIO_USART2_CS_LOC                  (1U)

#define PORTIO_USART2_RX_PIN                  (5U)
#define PORTIO_USART2_RX_PORT                 (gpioPortF)
#define PORTIO_USART2_RX_LOC                  (17U)

#define PORTIO_USART2_TX_PIN                  (7U)
#define PORTIO_USART2_TX_PORT                 (gpioPortF)
#define PORTIO_USART2_TX_LOC                  (20U)

#define HAL_USART2_ENABLE                     (1)

#define BSP_USART2_RX_PIN                     (5U)
#define BSP_USART2_RX_PORT                    (gpioPortF)
#define BSP_USART2_RX_LOC                     (17U)

#define BSP_USART2_TX_PIN                     (7U)
#define BSP_USART2_TX_PORT                    (gpioPortF)
#define BSP_USART2_TX_LOC                     (20U)

#define HAL_USART2_RX_QUEUE_SIZE              (128UL)
#define HAL_USART2_BAUD_RATE                  (9600UL)
#define HAL_USART2_RXSTOP                     (16UL)
#define HAL_USART2_RXSTART                    (16UL)
#define HAL_USART2_TX_QUEUE_SIZE              (128UL)
#define HAL_USART2_FLOW_CONTROL               (HAL_USART_FLOW_CONTROL_NONE)
