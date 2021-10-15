/*
 * VMN_I2C1_stub.c
 *
 *  Created on: 21 Jan 2019
 *      Author: Aidan
 */


// Copyright 2015 Silicon Laboratories, Inc.
#include PLATFORM_HEADER
#include "stack/include/ember-types.h"
#include "hal/hal.h"
#include "hal/plugin/i2c-driver/i2c-driver.h"

#include "em_gpio.h"
#include "em_i2c.h"
#include "VMN/I2C_drivers/i2cspm/i2cspm.h"

#define I2C1_INIT 																  \
{                                                                                 \
	    I2C1,  /* Use I2C instance */                         \
	    BSP_I2C1_SCL_PORT,    /* SCL port */                                 \
	    BSP_I2C1_SCL_PIN,     /* SCL pin */                                  \
	    BSP_I2C1_SDA_PORT,    /* SDA port */                                 \
	    BSP_I2C1_SDA_PIN,     /* SDA pin */                                  \
	    BSP_I2C1_SCL_LOC,     /* Location of SCL */                          \
	    BSP_I2C1_SDA_LOC,     /* Location of SDA */                          \
	    0,                         /* Use currently configured reference clock */ \
	    I2C_FREQ_STANDARD_MAX,     /* Set to standard rate  */                    \
	    i2cClockHLRStandard,       /* Set to use 4:4 low/high duty cycle */       \
}

static boolean flag = false;

I2CSPM_Init_TypeDef i2c1Config = I2C1_INIT;


void halI2c1Initialize(void)
{
	if(!flag){
		I2CSPM_Init(&i2c1Config);
		flag = true;
	}
}

// This initialization callback is for compatibility with AFV2.

void emberAfPluginI2c1DriverInitCallback(void)
{
  halI2c1Initialize();
}

uint8_t halI2c1WriteBytes(uint8_t address,
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

  ret = I2CSPM_Transfer(i2c1Config.port, &seq);

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

uint8_t halI2c1ReadBytes(uint8_t address, uint8_t *buffer, uint8_t count)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t halRetCode;

  seq.addr = address;
  seq.flags = I2C_FLAG_READ;
  seq.buf[0].data = (uint8_t*)buffer;
  seq.buf[0].len = (uint16_t)count;

  ret = I2CSPM_Transfer(i2c1Config.port, &seq);

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
