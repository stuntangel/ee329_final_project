#include <math.h>
#define BLOCK_SIZE 64 // process 64 samples in one go
#define FFT_LENGTH 256 // number of elements per sample
#define SAMPLING_FREQ 800
void init_FFT( void );
void load_IIR( float* iirCoeffs );
float* perform_FFT(float* input_fft);
float* perform_IIR(float* X);
float* normalizeToBiquad( float* pCoeffs );
float omega( float center_freq );
float alpha_bw( float center_freq, float bandwidth );
