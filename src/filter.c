#include "filter.h"

volatile float readings[N_SAMPLES];

volatile int counter;
volatile float sum;

void initSensorReadings()
{
	int i;
	for(i = 0; i < N_SAMPLES; i++)
	{
		readings[i] = 0;
	}
	counter = 0;
	sum = 0;
}

float returnAverage(float sample)
{
	sum += sample - readings[counter % N_SAMPLES];
	readings[counter % N_SAMPLES] = sample;

	float avg = sum/N_SAMPLES;

	counter++;

	return avg;
}
