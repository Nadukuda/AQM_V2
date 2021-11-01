/*
 * VMN_I2C0_Stub.c
 *
 *  Created on: 21 Jan 2019
 *      Author: Aidan
 */

#include PLATFORM_HEADER
#include "stack/include/ember-types.h"
#include "hal/hal.h"
#include "hal/plugin/i2c-driver/i2c-driver.h"

#include "em_gpio.h"
#include "em_i2c.h"
#include "VMN/I2C_drivers/i2cspm/i2cspm.h"


#define I2C0_INIT 																  \
{                                                                                 \
	    I2C0,  /* Use I2C instance */                         \
	    BSP_I2C0_SCL_PORT,    /* SCL port */                                 \
	    BSP_I2C0_SCL_PIN,     /* SCL pin */                                  \
	    BSP_I2C0_SDA_PORT,    /* SDA port */                                 \
	    BSP_I2C0_SDA_PIN,     /* SDA pin */                                  \
	    BSP_I2C0_SCL_LOC,     /* Location of SCL */                          \
	    BSP_I2C0_SDA_LOC,     /* Location of SDA */                          \
	    0,                         /* Use currently configured reference clock */ \
	    I2C_FREQ_STANDARD_MAX,     /* Set to standard rate  */                    \
	    i2cClockHLRStandard,       /* Set to use 4:4 low/high duty cycle */       \
}

I2CSPM_Init_TypeDef i2cConfig = I2C0_INIT; //I2CSPM_INIT_DEFAULT;

static boolean flag = false;

void halI2cInitialize(void)
{
	if(!flag){
		I2CSPM_Init(&i2cConfig);
		flag = true;
	}
}

void halI2C_Si7021Initialize(void) {
	flag = false;
	halI2cInitialize();
}
void halI2C_ADC1Initialize(void) {
	flag = false;
	halI2cInitialize();
}

void halI2C_ADC0Initialize(void) {
	flag = false;
	halI2cInitialize();
}
// This initialization callback is for compatibility with AFV2.
void emberAfPluginI2cDriverInitCallback(void)
{
  halI2cInitialize();
}

uint8_t halI2cWriteBytes(uint8_t address,
                         const uint8_t *buffer,
                         uint8_t count)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t halRetCode;

  seq.addr = address;
  seq.flags = I2C_FLAG_WRITE;
  seq.buf[0].data = (uint8_t*)buffer;
  seq.buf[0].len = (uint16_t)count;

  ret = I2CSPM_Transfer(i2cConfig.port, &seq);

  switch (ret) {
    case i2cTransferDone:
      halRetCode = I2C_DRIVER_ERR_NONE;
      break;
    case i2cTransferBusErr:
      halRetCode = I2C_DRIVER_ERR_ADDR_NAK;
      break;
    case i2cTransferArbLost:
      halRetCode = I2C_DRIVER_ERR_ARB_LOST;
      break;
    case i2cTransferUsageFault:
      halRetCode = I2C_DRIVER_ERR_USAGE_FAULT;
      break;
    case i2cTransferSwFault:
      halRetCode = I2C_DRIVER_ERR_SW_FAULT;
      break;
    default:
      halRetCode = I2C_DRIVER_ERR_UNKOWN;
      break;
  }

  return halRetCode;
}


uint8_t halI2cWriteBytesDelay(uint8_t address,
                              const uint8_t *buffer,
                              uint8_t count,
                              uint8_t delay)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t halRetCode;

  seq.addr = address;
  seq.flags = I2C_FLAG_WRITE;
  seq.buf[0].data = (uint8_t*)buffer;
  seq.buf[0].len = (uint16_t)count;

  ret = I2CSPM_Transfer(i2cConfig.port, &seq);

  switch (ret) {
    case i2cTransferDone:
      halRetCode = I2C_DRIVER_ERR_NONE;
      break;
    case i2cTransferBusErr:
      halRetCode = I2C_DRIVER_ERR_ADDR_NAK;
      break;
    case i2cTransferArbLost:
      halRetCode = I2C_DRIVER_ERR_ARB_LOST;
      break;
    case i2cTransferUsageFault:
      halRetCode = I2C_DRIVER_ERR_USAGE_FAULT;
      break;
    case i2cTransferSwFault:
      halRetCode = I2C_DRIVER_ERR_SW_FAULT;
      break;
    default:
      halRetCode = I2C_DRIVER_ERR_UNKOWN;
      break;
  }

  return halRetCode;
}

uint8_t halI2cReadBytes(uint8_t address, uint8_t *buffer, uint8_t count)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t halRetCode;

  seq.addr = address;
  seq.flags = I2C_FLAG_READ;
  seq.buf[0].data = (uint8_t*)buffer;
  seq.buf[0].len = (uint16_t)count;

  ret = I2CSPM_Transfer(i2cConfig.port, &seq);

  switch (ret) {
    case i2cTransferDone:
      halRetCode = I2C_DRIVER_ERR_NONE;
      break;
    case i2cTransferBusErr:
      halRetCode = I2C_DRIVER_ERR_ADDR_NAK;
      break;
    case i2cTransferArbLost:
      halRetCode = I2C_DRIVER_ERR_ARB_LOST;
      break;
    case i2cTransferUsageFault:
      halRetCode = I2C_DRIVER_ERR_USAGE_FAULT;
      break;
    case i2cTransferSwFault:
      halRetCode = I2C_DRIVER_ERR_SW_FAULT;
      break;
    default:
      halRetCode = I2C_DRIVER_ERR_UNKOWN;
      break;
  }

  return halRetCode;
}
