#ifndef __LCD_BARGRAPH_H
#define __LCD_BARGRAPH_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stm32f4xx.h"
#include "arm_math.h"
#include "LCD.h"

#define BAR_VERTICAL        0x01
#define BAR_HORIZONTAL      0x02
#define BAR_INVERTED        0x04
#define FONT_WIDTH          5
#define FONT_LENGTH         7

typedef struct
{
    uint8_t rows, cols;
    uint8_t s_Row, s_Col;
    uint8_t type;
    uint8_t numVals, barWidth;
} bar_conf_t;

void LCD_BarGraph_Init(
    uint8_t lcd_r, uint8_t lcd_c,
    uint8_t g_r, uint8_t g_c,
    uint8_t s_r, uint8_t s_c,
    uint8_t type, uint8_t splits);

void LCD_BarGraph_draw(float32_t *value, float32_t max);
void LCD_BarGraph_clear(void);
void LCD_BarGraph_flip(void);
void LCD_BarGraph_destroy(void);

#endif
