#include "filter_utils.h"
#include "arm_math.h"
#include <stdio.h>

static arm_rfft_fast_instance_f32 fft_instance;
static arm_biquad_casd_df1_inst_f32 iir_instance;

void init_FFT( void ) {
	arm_rfft_fast_init_f32(&fft_instance, FFT_LENGTH); // initializes FFT
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
	float pState; // where data gets stored (not used when numStates = 0, but still necessary)
	arm_biquad_cascade_df1_init_f32(&iir_instance, 0, iirCoeffs, &pState); // store coeffs for future use
}

/*
 * input: input_fft[FFT_LENGTH]
 * output: output_fft_mag[FFT_LENGTH/2] (even fn, so only positive)
 */
float* perform_FFT(float* input_fft) {
	float output_fft[64];
	float output_fft_mag[64];
	arm_rfft_fast_f32(&fft_instance, input_fft, output_fft, 0); // 0 is the # of complex samples
	arm_cmplx_mag_f32(output_fft, output_fft_mag, FFT_LENGTH/2); // return magnitude
	return output_fft_mag;
}
/*
 * performs Y=X*H
 * inputs: X, H (input_signal =  sample_L)
 * output: Y
 */
float* perform_IIR(float* X) {
	float iirOutput[64];
	arm_biquad_cascade_df1_f32(&iir_instance, X, iirOutput, BLOCK_SIZE);
	return iirOutput;
}

// pCoeffs: {b0, b1, b2, a1, a2}
float* normalizeToBiquad( float* pCoeffs ) {
	// takes coefficients of Z transform and rearranges them to be in form of Eqn 2
	float newB0 = pCoeffs[0]/pCoeffs[3]; // b0/a0
	float newB1 = pCoeffs[1]/pCoeffs[3]; // b1/a0
	float newB2 = pCoeffs[2]/pCoeffs[3]; // b1/a0
	float newA1 = pCoeffs[4]/pCoeffs[3]; // a1/a0
	float newA2 = pCoeffs[5]/pCoeffs[3]; // a2/a0
	float coeffs[5] = {newB0, newB1, newB2, newA1, newA2};
	return coeffs;
}

float omega( float center_freq ) {
	return (2*PI*center_freq/SAMPLING_FREQ);
}

float alpha_bw( float center_freq, float bandwidth ) {
	return (sin(omega(center_freq))*sinh(log10(2)*bandwidth*omega(center_freq)/(2*sin(omega(center_freq)))));
}
