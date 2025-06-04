#include <math.h>
#include "stm32l4xx_hal.h"
#include "filter_util.h"
#include "stm32l4xx_it.h"
#define PI 3.1415926535
#define SAMPLE_TIME (400000)
#define SAMPLES (256)
#define SAMPLE_INTVL (int)(SAMPLE_TIME / SAMPLES)
void LPF(biquad_t* biq_stage1, float bandwidth, float corner_freq);
void HPF(biquad_t* biq_stage1, float bandwidth, float corner_freq);
void BPF_Amp_Q(biquad_t* biq_stage1, float q_factor, float center_freq);
void BPF_Amp_Unity(biquad_t* biq_stage1, float bandwidth, float center_freq);
void Notch(biquad_t* biq_stage1, float bandwidth, float center_freq);

float omega( float center_freq );
float alpha_bw( float center_freq, float bandwidth );
float alpha_q(float center_freq, float q_factor);
