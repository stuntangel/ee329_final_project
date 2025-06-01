#include "filters.h"
/*
 * Low Pass Filter:
 * H(s) = 1 / (s^2 + s/Q + 1)
 */
// pCoeffs: {b0, b1, b2, a1, a2}
float* LPF(float bandwidth, float corner_freq)
{
	float w0 = omega(corner_freq);
	float a = alpha_bw(corner_freq, bandwidth);

	float b1 = (1 - cos(w0));
	float b0 = (b1/2);
	float b2 = b0;
	float a0 = (1 + a);
	float a1 = (-2 * cos(w0));
	float a2 = (1 - a);
	float coeffs[6] = {b0, b1, b2, a0, a1, a2};
	return normalizeToBiquad(coeffs);
}

/*
 * High Pass Filter:
 * H(s) = (s^2) / (s^2 + (s / Q) + 1)
 * b0 = (1 + cos(w0)) / 2
 * b1 = -(1 + cos(w0))
 * b2 = (1 + cos(w0)) / 2
 * a0 = 1 + a
 * a1 = -2cos(w0)
 * a2 = 1 - a
 */
float* HPF(float bandwidth, float corner_freq)
{
	float w0 = omega(corner_freq);
	float a = alpha_bw(corner_freq, bandwidth);

	float b0 = (1 + cos(w0) )/ 2;
	float b1 = -(1 + cos(w0));
	float b2 = (1 + cos(w0));
	float a0 = 1 + a;
	float a1 = -2 * cos(w0);
	float a2 = 1 - a;
	float coeffs[6] = {b0, b1, b2, a0, a1, a2};
	return normalizeToBiquad(coeffs);
}

/*
 * Band Pass Filter, Peak gain = Q, constant skirt gain
 * H(s) = (s) / (s^2 + (s / Q) + 1)
 * b0 = sin(w0) / 2 = Qa
 * b1 = 0
 * b2 = -(sin(w0)) / 2 = -Qa
 * a0 = 1 + a
 * a1 = -2cos(w0)
 * a2 = 1 - a
 */
float* BPF_Amp_Q(float q_factor, float center_freq)
{
	float w0 = omega(center_freq);
	float a = alpha_Q(center_freq, q_factor);

	float b0 = (q_factor * a);
	float b1 = 0;
	float b2 = -(q_factor * a);
	float a0 = (1 + a);
	float a1 = (-2 * cos(w0));
	float a2 = (1 - a);
	float coeffs[6] = {b0, b1, b2, a0, a1, a2};
	return normalizeToBiquad(coeffs);
}

/*
 * Band Pass Filter, Constant gain = 1
 * H(s) = (s / Q) / (s^2 + (s / Q) + 1)
 * b0 = a
 * b1 = 0
 * b2 = -a
 * a0 = 1 + a
 * a1 = -2cos(w0)
 * a2 = 1 - a
 */
float* BPF_Amp_Unity(float bandwidth, float center_freq)
{
	float w0 = omega(center_freq);
	float a = alpha_bw(center_freq, bandwidth);

	float b0 = a;
	float b1 = 0;
	float b2 = -a;
	float a0 = 1 + a;
	float a1 = -2 * cos(w0);
	float a2 = 1 - a;
	float coeffs[6] = {b0, b1, b2, a0, a1, a2};
	return normalizeToBiquad(coeffs);
}

/*
 * Notch Filter
 * H(s) = (s^2 + 1) / (s^2 + (s / Q) + 1)
 * b0 = 1
 * b1 = -2cos(w0)
 * b2 = 1
 * a0 = 1 + a
 * a1 = -2cos(w0)
 * a2 = 1 - a
 */
float* Notch(float bandwidth, float center_freq)
{
	float w0 = omega(center_freq);
	float a = alpha_bw(center_freq, bandwidth);

	float b0 = 1;
	float b1 = -(2 * cos(w0));
	float b2 = 1;
	float a0 = 1 + a;
	float a1 = -2 * cos(w0);
	float a2 = 1 - a;
	float coeffs[6] = {b0, b1, b2, a0, a1, a2};
	return normalizeToBiquad(coeffs);
}
