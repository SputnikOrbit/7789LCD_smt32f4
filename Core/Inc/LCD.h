#include "main.h"
#include "stm32f4xx_hal.h"


HAL_StatusTypeDef LCD_Init(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef LCD_WriteCmd(SPI_HandleTypeDef *hspi, uint8_t *cmd);

HAL_StatusTypeDef LCD_WriteData(SPI_HandleTypeDef *hspi, uint8_t *data);