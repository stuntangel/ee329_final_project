#include "filters.h"
/*
 * Lowpass:
 * H(s) = 1 / (s^2 + s/Q + 1)
 */
// pCoeffs: {b0, b1, b2, a1, a2}
float* computeCoeffs(float bandwidth, float center_freq) {
	float b1 = (1-cos(omega(center_freq)));
	float b0 = (b1/2);
	float b2 = b0;
	float a0 = (1+alpha_bw(center_freq, bandwidth));
	float a1 = (-2*cos(omega(center_freq)));
	float a2 = (1-alpha_bw(center_freq, bandwidth));
	return normalizeToBiquad({b0, b1, b2, a0, a1, a2});
}
