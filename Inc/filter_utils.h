#include <math.h>
#define BLOCK_SIZE 64 // process 64 samples in one go
#define FFT_LENGTH 256 // number of elements per sample
void init_FFT( void );
void load_IIR( float32_t[] iirCoeffs );
float32_t[] perform_FFT(float32_t[] input_fft);
float32_t[] perform_IIR(float32_t[] X);
float32_t[] normalizeToBiquad( float32_t[] pCoeffs );
float32_t omega( float32_t center_freq );
float32_t alpha_bw( float32_t center_freq, float32_t bandwidth );
