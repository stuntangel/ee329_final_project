#include "filter_utils.h"
#include "stdlib.h"
static arm_rfft_fast_instance_f32 FFTInstance;
static arm_rfft_fast_instance_f32 *ptr_FFT_instance = &FFTInstance;
static arm_biquad_casd_df1_inst_f32 iir_instance;
static arm_biquad_casd_df1_inst_f32 *ptr_iir_instance = &iir_instance;
#define NUMSECTIONS 1 // number of cascade sections
static float32_t taps[4 * NUMSECTIONS];
static float32_t rFFT_Out[FFTLEN] = {0}; // points to the output of the FFT function
static float32_t dFFT[FFTLEN] = {0}; // points to the output of the magnitude of FFT function
static float32_t iir_Out[FFTLEN] = {0}; // points to the output of IIR

void init_FFT( void ) {
	arm_rfft_4096_fast_init_f32(ptr_FFT_instance);
}

void load_IIR( float* iirCoeffs ) {
	/*
	 * The Digital Biquad Filter is the bulk of how our audio processing will be done
	 * due to lower latency compared to FIR and preexisting literature
	 * Sets coefficients inside IIR (different from performing the IIR)
	 * The Digital Biquad Filter has the following structure:
	 *          (b0/a0) + (b1/a0)*z^-1 + (b2/a0)*z^-2
     * H(z) =   ---------------------------------------    (Eq 2)
     *              1 + (a1/a0)*z^-1 + (a2/a0)*z^-2
     *  Source: Robert Bristow-Johnson  <rbj@audioimagination.com>
	 */
	arm_biquad_cascade_df1_init_f32(ptr_iir_instance, NUMSECTIONS, iirCoeffs, taps); // store coeffs for future use
}

/*
 * input: input_fft[FFT_LENGTH]
 * output: output_fft_mag[FFT_LENGTH/2] (even fn, so only positive)
 */
void perform_FFT(float* input_fft) {
	arm_rfft_fast_f32(ptr_FFT_instance, input_fft, rFFT_Out, RFFT); // Performs FFT, stores
	//data in rFFT_Out
	arm_cmplx_mag_f32(rFFT_Out, dFFT, FFTLEN/2); // stores only magnitude
	dFFT[0] = 0; // output stored in dFFT
}
/*
 * performs Y=X*H
 * inputs: X, H (input_signal =  sample_L)
 * output: Y
 */
void perform_IIR(float* X) {
	arm_biquad_cascade_df1_f32(ptr_iir_instance, X, iir_Out, BLOCK_SIZE);
}

// pCoeffs: {b0, b1, b2, a1, a2}
float* normalizeToBiquad( float* pCoeffs ) {
	// takes coefficients of Z transform and rearranges them to be in form of Eqn 2
	float newB0 = pCoeffs[0]/pCoeffs[3]; // b0/a0
	float newB1 = pCoeffs[1]/pCoeffs[3]; // b1/a0
	float newB2 = pCoeffs[2]/pCoeffs[3]; // b1/a0
	float newA1 = pCoeffs[4]/pCoeffs[3]; // a1/a0
	float newA2 = pCoeffs[5]/pCoeffs[3]; // a2/a0
	float* coeffs = (float*)malloc(5*sizeof(float));
	coeffs[0] = newB0;
	coeffs[1] = newB1;
	coeffs[2] = newB2;
	coeffs[3] = newA1;
	coeffs[4] = newA2;
	return coeffs;
}

float omega( float center_freq ) {
	return (2*PI*center_freq/FFTLEN);
}

float alpha_bw( float center_freq, float bandwidth ) {
	return (sin(omega(center_freq))*sinh(log10(2)*bandwidth*omega(center_freq)/(2*sin(omega(center_freq)))));
}

float alpha_q(float center_freq, float q_factor) {
	return (sin(omega(center_freq)) / (2 * q_factor));
}
