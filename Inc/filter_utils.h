#ifndef __FILTER_UTILS_H
#define __FILTER_UTILS_H
#include "../Drivers/CMSIS/DSP/Include/arm_math.h"
#include <math.h>
#include <stdio.h>
#define FFTLEN 4096
#define BLOCK_SIZE 8
#define RFFT 0
#define RIFFT 1
#define ARM_MATH_CM4 1

void init_FFT( void );
void load_IIR( float* iirCoeffs );
void perform_FFT(float* input_fft);
void perform_IIR(float* X);
float* normalizeToBiquad( float* pCoeffs );
float omega( float center_freq );
float alpha_bw( float center_freq, float bandwidth );
float alpha_q(float center_freq, float q_factor);
#endif
