#include "hw_spi.h"

void SPI1_Init(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
  
  LL_SPI_InitTypeDef SPI1_InitStruct = {0};
  SPI1_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI1_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI1_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI1_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI1_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI1_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV4;
  SPI1_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI1_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI1_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  LL_SPI_Init(SPI1, &SPI1_InitStruct);  
}
