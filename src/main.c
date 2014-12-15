#include "main.h"

#define SAMPLES     512
#define FFT_SIZE    SAMPLES / 2

extern volatile uint32_t usTicks;

int main(void) {
    Time_Init();

    GPIO_Configuration();
    ADC_Configuration();

    LCD_4BitInit(GPIOE, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10,
        GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13);

    LCD_Begin(2, 16, LCD_5x8DOTS);

    LCD_clear();
    LCD_BarGraph_Init(2,16,2,16,1,0,BAR_VERTICAL,16);

    arm_cfft_radix4_instance_f32 S;

    uint16_t i;
    float32_t Input[SAMPLES];
    float32_t Output[FFT_SIZE];
    float32_t maxValue; //, highest;
    uint32_t maxIndex;
    float32_t sum[16];
    uint8_t group = FFT_SIZE/32;

    while(1) {

        delay(10);

        for(i = 0; i < SAMPLES; i+= 2) {
            delayMicroseconds(22);
            Input[i] = (float32_t)((float32_t)analogRead() - (float32_t) 2048.0) / (float32_t) 2048.0;
            Input[i+1] = 0;
        }

        arm_cfft_radix4_init_f32(&S, FFT_SIZE, 0, 1);
        arm_cfft_radix4_f32(&S, Input);
        arm_cmplx_mag_f32(Input, Output, FFT_SIZE);
        arm_max_f32(Output, FFT_SIZE, &maxValue, &maxIndex);


        for(i = 0; i < 16; i++) {
            sum[i] = 0;
        }

        for(i = 1; i < FFT_SIZE/2; i++) {
            sum[i/group] += Output[i];          // each output at 177.5Hz * i
        }                                       // group of 8

        LCD_BarGraph_draw(sum, 1.5);
    }

	return(0);
}
