#include <stdio.h>

void init_FFT( void ) {
	arm_rfft_fast_instance_f32 fft_instance;
	arm_rfft_fast_init_f32(&fft_instance, FFT_LENGTH);
}

void init_FIR( void ) {
	arm_fir_init_f32(&S_L_FIR, nData_in_values, (float *) xData, pState, BlockSize);
}

void init_IIR( void ) {
	arm_biquad_cascade_df1_init_f32(&S_IIR, IIR_ORDER, iirCoeffs, iirState);
}

/*
 * input: input_fft[FFT_LENGTH]
 * output: output_fft_mag[FFT_LENGTH/2] (even fn, so only positive)
 */
float32_t[] perform_FFT(float32_t[] input_fft) {
	arm_rfft_fast_f32(&fft_instance, input_fft, output_fft, 0); // 0 is the # of complex samples
	return arm_cmplx_mag_f32(output_fft, output_fft_mag, FFT_LENGTH/2); // return magnitude
}
/*
 * performs Y=X*H
 * inputs: X, H (input_signal =  sample_L)
 * output: Y
 */
float32_t[] perform_IIR(float32_t[] X, float32_t[] H) {
	arm_biquad_cascade_df1_f32(&S_IIR, inputSignal, iirOutput, BLOCK_SIZE);
	arm_fir_f32(&S_FIR, inputSignal, firOutput, BLOCK_SIZE);
}

float32_t[] perform_FIR(float32_t[] X, float32_t[] H) {
	arm_fir_f32(&S_FIR, inputSignal, firOutput, BLOCK_SIZE);
}