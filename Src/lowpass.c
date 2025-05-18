#include "lowpass.h"
/*
 * H(s) = 1 / (s^2 + s/Q + 1)
 */
// pCoeffs: {b0, b1, b2, a1, a2}
float32_t[] computeCoeffs(float32_t bandwidth, float32_t center_freq) {
	float32_t b1 = (1-cos(omega(center_freq)));
	float32_t b0 = (b1/2);
	float32_t b2 = b0;
	float32_t a0 = (1+alpha_bw(center_freq, bandwidth));
	float32_t a1 = (-2*cos(omega(center_freq)));
	float32_t a2 = (1-alpha_bw(center_freq, bandwidth));
	return normalizeToBiquad({b0, b1, b2, a0, a1, a2});
}
