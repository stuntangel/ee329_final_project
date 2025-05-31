#ifndef __FILTER_UTILS_H
#define __FILTER_UTILS_H
#include "../Drivers/CMSIS/DSP/Include/arm_math.h"
#include <math.h>
#include <stdio.h>
#define BLOCK_SIZE 64 // process 64 samples in one go
#define FFT_LENGTH 256 // number of elements per sample
#define ARM_MATH_CM4 1
#define SAMPLING_FREQ 800
#define PI 3.14159265
void init_FFT( void );
void load_IIR( float* iirCoeffs );
float* perform_FFT(float* input_fft);
float* perform_IIR(float* X);
float* normalizeToBiquad( float* pCoeffs );
float omega( float center_freq );
float alpha_bw( float center_freq, float bandwidth );
float alpha_q(float center_freq, float q_factor);
#endif
