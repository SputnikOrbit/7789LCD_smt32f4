#ifndef __LCD_H
#define __LCD_H

#include "main.h"

extern uint8_t *LCD_Img;

HAL_StatusTypeDef LCD_Init(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef LCD_WriteCmd(SPI_HandleTypeDef *hspi, uint8_t cmd);

HAL_StatusTypeDef LCD_WriteData8(SPI_HandleTypeDef *hspi, uint8_t data);

HAL_StatusTypeDef LCD_WriteData16(SPI_HandleTypeDef *hspi, uint16_t data);

HAL_StatusTypeDef LCD_Reset(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef LCD_ShowImg(SPI_HandleTypeDef *hspi);

void LCD_SetRegion(SPI_HandleTypeDef *hspi, u16 x0, u16 y0, u16 x1, u16 y1);

extern const uint8_t my_img[57600];

#endif