#include "filters.h"
/*
 * Lowpass:
 * H(s) = 1 / (s^2 + s/Q + 1)
 */
// pCoeffs: {b0, b1, b2, a1, a2}
float* computeLowpassCoeffs(float bandwidth, float center_freq) {
	float b1 = (1-cos(omega(center_freq)));
	float b0 = (b1/2);
	float b2 = b0;
	float a0 = (1+alpha_bw(center_freq, bandwidth));
	float a1 = (-2*cos(omega(center_freq)));
	float a2 = (1-alpha_bw(center_freq, bandwidth));
	float* coeffs = (float*)malloc(6*sizeof(float));
	coeffs[0] = b0;
	coeffs[1] = b1;
	coeffs[2] = b2;
	coeffs[3] = a0;
	coeffs[4] = a1;
	coeffs[5] = a2;
	return normalizeToBiquad(coeffs);
}
