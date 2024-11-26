#include "LCD.h"

uint8_t *LCD_Img;

HAL_StatusTypeDef LCD_Init(SPI_HandleTypeDef *hspi){

    LCD_Reset(hspi); // 复位屏幕，防止出现乱色

    LCD_WriteCmd(hspi, 0x3A);  // 设置颜色格式为16位真彩
    LCD_WriteData8(hspi, 0x05); // 03 4K; 05 65K; 06 262K

    LCD_WriteCmd(hspi, 0x36);  // 设置屏幕方向
    LCD_WriteData8(hspi, 0x00); 
    // 00 从左到右，从上到下; 
    // 01 从上到下，从左到右; 
    // 02 从右到左，从上到下; 
    // 03 从上到下，从右到左;

    //----------- ST7789S Frame rate setting ---------//
    //解释：帧率设置，用于调整屏幕刷新速度, 60Hz, 通过调整VBP、VFP、VSYNC、HBP、HFP、HSYNC来调整
	LCD_WriteCmd(hspi, 0xB2);				
	LCD_WriteData8(hspi, 0x0C);
	LCD_WriteData8(hspi, 0x0C);
	LCD_WriteData8(hspi, 0x00);
	LCD_WriteData8(hspi, 0x33);
	LCD_WriteData8(hspi, 0x33);
	
	LCD_WriteCmd(hspi, 0xB7); 
	LCD_WriteData8(hspi, 0x35);
	  
    //----------- ST7789S Power setting ---------//
    //解释：电源设置，用于调整屏幕亮度
	LCD_WriteCmd(hspi, 0xBB);
	LCD_WriteData8(hspi, 0x19);
	
	LCD_WriteCmd(hspi, 0xC0);
	LCD_WriteData8(hspi, 0x2C);
	
	LCD_WriteCmd(hspi, 0xC2);
	LCD_WriteData8(hspi, 0x01);
	
	LCD_WriteCmd(hspi, 0xC3);
	LCD_WriteData8(hspi, 0x12);   
	
	LCD_WriteCmd(hspi, 0xC4);
	LCD_WriteData8(hspi, 0x20);  
	
	LCD_WriteCmd(hspi, 0xC6); 
	LCD_WriteData8(hspi, 0x0F);    
	
	LCD_WriteCmd(hspi, 0xD0); 
	LCD_WriteData8(hspi, 0xA4);
	LCD_WriteData8(hspi, 0xA1);
 
    //----------- Posistive Voltage Gamma Control ---------//
    //解释：正电压伽马控制， 用于调整屏幕颜色
	LCD_WriteCmd(hspi, 0xE0);
	LCD_WriteData8(hspi, 0xD0);
	LCD_WriteData8(hspi, 0x04);
	LCD_WriteData8(hspi, 0x0D);
	LCD_WriteData8(hspi, 0x11);
	LCD_WriteData8(hspi, 0x13);
	LCD_WriteData8(hspi, 0x2B);
	LCD_WriteData8(hspi, 0x3F);
	LCD_WriteData8(hspi, 0x54);
	LCD_WriteData8(hspi, 0x4C);
	LCD_WriteData8(hspi, 0x18);
	LCD_WriteData8(hspi, 0x0D);
	LCD_WriteData8(hspi, 0x0B);
	LCD_WriteData8(hspi, 0x1F);
	LCD_WriteData8(hspi, 0x23);
 
	LCD_WriteCmd(hspi, 0xE1);
	LCD_WriteData8(hspi, 0xD0);
	LCD_WriteData8(hspi, 0x04);
	LCD_WriteData8(hspi, 0x0C);
	LCD_WriteData8(hspi, 0x11);
	LCD_WriteData8(hspi, 0x13);
	LCD_WriteData8(hspi, 0x2C);
	LCD_WriteData8(hspi, 0x3F);
	LCD_WriteData8(hspi, 0x44);
	LCD_WriteData8(hspi, 0x51);
	LCD_WriteData8(hspi, 0x2F);
	LCD_WriteData8(hspi, 0x1F);
	LCD_WriteData8(hspi, 0x1F);
	LCD_WriteData8(hspi, 0x20);
	LCD_WriteData8(hspi, 0x23);
 
    //----------- Setting ---------//
    //解释：设置，用于调整屏幕显示效果，如是否显示，是否反色等
	LCD_WriteCmd(hspi, 0x21); 
	LCD_WriteCmd(hspi, 0x11);
	LCD_WriteCmd(hspi, 0x29);

    //设置显示区域
    LCD_SetRegion(hspi, 0, 0, 239, 239);
    //冲刷sram，防止出现雪花屏
    HAL_GPIO_WritePin(DC_GPIO_Port ,DC_Pin, GPIO_PIN_SET);
    for (int i = 0; i < 57600; i++)
    {
        HAL_SPI_Transmit(hspi, (uint8_t *)0xFFFF, 2, 100);
    }
    HAL_GPIO_WritePin(DC_GPIO_Port ,DC_Pin, GPIO_PIN_RESET);
    
    return HAL_OK;
}

void LCD_SetRegion(SPI_HandleTypeDef *hspi, u16 x0, u16 y0, u16 x1, u16 y1)//设置显示区域（x0，y0，x1，y1）
{
	LCD_WriteCmd(hspi, 0x2a);
	LCD_WriteData16(hspi, x0);
	LCD_WriteData16(hspi, x1);
	LCD_WriteCmd(hspi, 0x2b);
	LCD_WriteData16(hspi, y0);
	LCD_WriteData16(hspi, y1);
	LCD_WriteCmd(hspi, 0x2c);	        //下面发送的是数据
}

HAL_StatusTypeDef LCD_WriteCmd(SPI_HandleTypeDef *hspi, uint8_t cmd){
    HAL_SPI_StateTypeDef state = hspi->State;

    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hspi, &cmd, 1, 100);
}

HAL_StatusTypeDef LCD_WriteData8(SPI_HandleTypeDef *hspi, uint8_t data){
    HAL_SPI_StateTypeDef state = hspi->State;

    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(hspi, &data, 1, 100);
}

HAL_StatusTypeDef LCD_WriteData16(SPI_HandleTypeDef *hspi, uint16_t data){
    HAL_SPI_StateTypeDef state = hspi->State;

    //需要拆分成两个字节发送
    uint8_t data_0 = (data >> 8) & 0xFF;
    uint8_t data_1 = data & 0xFF;

    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(hspi, &data_0, 1, 100);
    HAL_SPI_Transmit(hspi, &data_1, 1, 100);
}

HAL_StatusTypeDef LCD_Reset(SPI_HandleTypeDef *hspi){
    HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
}

HAL_StatusTypeDef LCD_ShowImg(SPI_HandleTypeDef *hspi){
    LCD_SetRegion(hspi, 0, 0, 239, 239);
    HAL_GPIO_WritePin(DC_GPIO_Port ,DC_Pin, GPIO_PIN_SET); //DC拉高，发送数据
    for (int i = 0; i < 57600; i++)
    {
        HAL_SPI_Transmit(hspi, (uint8_t *)0xFFFF, 2, 100);
    }
    return HAL_OK; 
}

const uint8_t my_img[57600] = {0};
