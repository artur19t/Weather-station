#include "hw_i2c.h"

void I2C1_Init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
  
  LL_I2C_InitTypeDef I2C1_InitStruct = {0};
  I2C1_InitStruct.ClockSpeed = 100000;
  I2C1_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
  I2C1_InitStruct.OwnAddress1 = 0;
  I2C1_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  I2C1_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C1_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  
  LL_I2C_Init(I2C1, &I2C1_InitStruct);
}
