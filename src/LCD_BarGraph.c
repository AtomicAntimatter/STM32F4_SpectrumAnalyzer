#include "LCD_BarGraph.h"

static uint8_t vertical_bar[7][8] =
{
    {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x1f},
    {0x00,0x00,0x00,0x00,0x00,0x1f,0x1f},
    {0x00,0x00,0x00,0x00,0x1f,0x1f,0x1f},
    {0x00,0x00,0x00,0x1f,0x1f,0x1f,0x1f},
    {0x00,0x00,0x1f,0x1f,0x1f,0x1f,0x1f},
    {0x00,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f}
};

static uint8_t inverted_vertical_bar[7][8] =
{
    {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f},
    {0x1f,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x1f,0x1f,0x00,0x00,0x00,0x00,0x00},
    {0x1f,0x1f,0x1f,0x00,0x00,0x00,0x00},
    {0x1f,0x1f,0x1f,0x1f,0x00,0x00,0x00},
    {0x1f,0x1f,0x1f,0x1f,0x1f,0x00,0x00},
    {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x00}
};

static uint8_t horizontal_bar[5][8] =
{
    {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f},
    {0x10,0x10,0x10,0x10,0x10,0x10,0x10},
    {0x18,0x18,0x18,0x18,0x18,0x18,0x18},
    {0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c},
    {0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e}
};

static uint8_t inverted_horizontal_bar[5][8] =
{
    {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f},
    {0x01,0x01,0x01,0x01,0x01,0x01,0x01},
    {0x03,0x03,0x03,0x03,0x03,0x03,0x03},
    {0x07,0x07,0x07,0x07,0x07,0x07,0x07},
    {0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f}
};

static void drawVertical(float32_t *value, float32_t max);
static void drawHorizontal(float32_t *value, float32_t max);
static void loadCustomCharSet(uint8_t set[][8], uint8_t n);

static volatile bar_conf_t Bar_Conf;
static uint8_t textDir = 0;
static int8_t vertDir = 0;

/*
*   lcd_r       number of rows on lcd
*   lcd_c       number of columns on lcd
*   g_r         number of rows to display graph
*   g_c         number of columns to display graph
*   s_r         the row to start graph (zero index)
*   s_c         the column to start graph (zero index)
*   type        the type of graph
*/
void LCD_BarGraph_Init(
    uint8_t lcd_r, uint8_t lcd_c,
    uint8_t g_r, uint8_t g_c,
    uint8_t s_r, uint8_t s_c,
    uint8_t type, uint8_t splits)
{
    /* Check if params are valid */
    if(s_r >= lcd_r || s_c < 0 || lcd_r <= 0)
        return;
    if(s_c >= lcd_c || s_c < 0 || lcd_c <= 0)
        return;

    /* Cutoff rows, cols if params are invalid */
    Bar_Conf.rows = g_r + s_r > lcd_r ? lcd_r - s_r : g_r;
    Bar_Conf.cols = g_c + s_c > lcd_c ? lcd_c - s_c : g_c;

    if(type == (BAR_VERTICAL | BAR_INVERTED)) {
        loadCustomCharSet(inverted_vertical_bar, FONT_LENGTH);
        vertDir = 1;
    } else if(type == (BAR_HORIZONTAL | BAR_INVERTED)) {
        loadCustomCharSet(inverted_horizontal_bar, FONT_WIDTH);
        Bar_Conf.cols = s_c + 1 < g_c ? s_c + 1 : g_c;
    } else if(type & BAR_VERTICAL) {
        loadCustomCharSet(vertical_bar, FONT_LENGTH);
        Bar_Conf.rows = s_r + 1 < g_r ? s_r + 1 : g_r;
        vertDir = -1;
    } else if(type & BAR_HORIZONTAL)
        loadCustomCharSet(horizontal_bar, FONT_WIDTH);

    Bar_Conf.s_Row = s_r;
    Bar_Conf.s_Col = s_c;
    Bar_Conf.type = type;

    if(type & BAR_HORIZONTAL) {
        Bar_Conf.barWidth = ceil(Bar_Conf.rows/splits);
        Bar_Conf.numVals = ceil(Bar_Conf.rows/Bar_Conf.barWidth);
    } else {
        Bar_Conf.barWidth = ceil(Bar_Conf.cols/splits);
        Bar_Conf.numVals = ceil(Bar_Conf.cols/Bar_Conf.barWidth);
    }
}

void LCD_BarGraph_draw(float32_t *value, float32_t max) {
    if(Bar_Conf.type & BAR_HORIZONTAL)
        drawHorizontal(value, max);
    else
        drawVertical(value, max);
}

void LCD_BarGraph_clear(void) {}

void LCD_BarGraph_flip(void) {
    textDir = !textDir;
    if(textDir)
        LCD_leftToRight();
    else
        LCD_rightToLeft();
}

void LCD_BarGraph_destroy(void) {}

static void drawVertical(float32_t *value, float32_t max) {
    uint8_t i, j, k, fulls, part, cOffset;          // init variables
    uint8_t buffer[Bar_Conf.rows][Bar_Conf.cols];   // init buffer

    for(i = 0; i < Bar_Conf.numVals; i++) {
        fulls = value[i] * Bar_Conf.rows / max;     // calculate full bars
        part = (uint8_t)(value[i] * Bar_Conf.rows * FONT_LENGTH / max) % FONT_LENGTH;
        // calculate partial bars

        for(j = 0; j < Bar_Conf.barWidth; j++) {
            cOffset = j + i * Bar_Conf.barWidth;

            if(cOffset >= Bar_Conf.cols)
                break;

            // perform output validation and store to buffer
            for(k = 0; k < fulls && k < Bar_Conf.rows; k++)
                buffer[k][cOffset] = 0;

            if(part && k != Bar_Conf.rows)
                buffer[k][cOffset] = part;

            for(k += !!part; k < Bar_Conf.rows; k++)
                buffer[k][cOffset] = 32;
        }
    }

    // check orientation and output to LCD
    for(i = 0; i < Bar_Conf.rows; i++) {
        LCD_setCursor(Bar_Conf.s_Row + vertDir*i, Bar_Conf.s_Col);
        for(j = 0; j < Bar_Conf.cols; j++)
            LCD_put(buffer[i][j]);
    }
}

static void drawHorizontal(float32_t* value, float32_t max) {
    if(Bar_Conf.type & ~textDir)
        LCD_BarGraph_flip();

    uint8_t i, j, k, fulls, part, rOffset;

    for(i = 0; i < Bar_Conf.numVals; i++) {
        fulls = value[i] * Bar_Conf.cols / max;
        part = (uint8_t)(value[i] * Bar_Conf.cols * FONT_WIDTH / max) % FONT_WIDTH;

        for(j = 0; j < Bar_Conf.barWidth; j++) {
            rOffset = j + i * Bar_Conf.barWidth;

            if(rOffset >= Bar_Conf.rows)
                break;

            LCD_setCursor(Bar_Conf.s_Row + rOffset, Bar_Conf.s_Col);

            for(k = 0; k < fulls && k < Bar_Conf.cols; k++)
                LCD_put(0);             // print full bar

            if(part && k != Bar_Conf.cols)
                LCD_put(part);          // print partial bar

            for(k += !!part; k < Bar_Conf.cols; k++)
                LCD_put(32);            // clear location with space
        }
    }
}

static void loadCustomCharSet(uint8_t set[][8], uint8_t n) {
    uint8_t i;
    for(i = 0; i < n; i++)
        LCD_createChar(i, set[i]);
}
