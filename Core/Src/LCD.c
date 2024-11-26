#include<stdio.h>
#include "LCD.h"

HAL_StatusTypeDef LCD_Init(SPI_HandleTypeDef *hspi){
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t cmd = 0x01;
    uint8_t data = 0x01;

    status = LCD_WriteCmd(hspi, &cmd);
    if(status != HAL_OK){
        return status;
    }

    status = LCD_WriteData(hspi, &data);
    if(status != HAL_OK){
        return status;
    }

    return status;
}


HAL_StatusTypeDef LCD_WriteCmd(SPI_HandleTypeDef *hspi, uint8_t *cmd){
    HAL_SPI_StateTypeDef state = hspi->State;

    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hspi, &cmd, 1, 100);
}

HAL_StatusTypeDef LCD_WriteData(SPI_HandleTypeDef *hspi, uint8_t *data){
    HAL_SPI_StateTypeDef state = hspi->State;

    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(hspi, &data, 1, 100);
}