#include "filters.h"
/*
 * Low Pass Filter:
 * H(s) = 1 / (s^2 + s/Q + 1)
 */
// pCoeffs: {b0, b1, b2, a1, a2}
void LPF(biquad_t* biq_stage1, float bandwidth, float corner_freq)
{
	float w0 = omega(corner_freq);
	float a = alpha_bw(corner_freq, bandwidth);

	biq_stage1->b[0] = ((1 - cosf(w0))/2)/(1 + a);
	biq_stage1->b[1] = (1 - cosf(w0))/(1 + a);
	biq_stage1->b[2] = ((1 - cosf(w0))/2)/(1 + a);
	biq_stage1->a[0] = (-2 * cosf(w0))/(1 + a);
	biq_stage1->a[1] = (1 - a)/(1 + a);
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
void HPF(biquad_t* biq_stage1, float bandwidth, float corner_freq)
{
	float w0 = omega(corner_freq);
	float a = alpha_bw(corner_freq, bandwidth);

	biq_stage1->b[0] = ((1 + cosf(w0) )/2)/(1 + a);
	biq_stage1->b[1] = -(1 + cosf(w0))/(1 + a);
	biq_stage1->b[2] = (1 + cosf(w0))/(1 + a);
	biq_stage1->a[0] = (-2 * cosf(w0))/(1 + a);
	biq_stage1->a[1] = (1 - a)/(1 + a);
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
void BPF_Amp_Q(biquad_t* biq_stage1, float q_factor, float center_freq)
{
	float w0 = omega(center_freq);
	float a = alpha_q(center_freq, q_factor);

	biq_stage1->b[0] = (q_factor * a)/(1 + a);
	biq_stage1->b[1] = 0;
	biq_stage1->b[2] = -(q_factor * a)/(1 + a);
	biq_stage1->a[0] = (-2 * cosf(w0))/(1 + a);
	biq_stage1->a[1] = (1 - a)/(1 + a);

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
void BPF_Amp_Unity(biquad_t* biq_stage1, float bandwidth, float center_freq)
{
	float w0 = omega(center_freq);
	float a = alpha_bw(center_freq, bandwidth);

	biq_stage1->b[0] = a/(1 + a);
	biq_stage1->b[1] = 0;
	biq_stage1->b[2] = -a/(1 + a);
	biq_stage1->a[0] = (-2 * cosf(w0))/(1 + a);
	biq_stage1->a[1] = (1 - a)/(1 + a);

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
void Notch(biquad_t* biq_stage1, float bandwidth, float center_freq)
{
	float w0 = omega(center_freq);
	float a = alpha_bw(center_freq, bandwidth);

	biq_stage1->b[0] = 1/(1 + a);
	biq_stage1->b[1] = -(2 * cos(w0))/(1 + a);
	biq_stage1->b[2] = 1/(1 + a);
	biq_stage1->a[0] = (-2 * cos(w0))/(1 + a);
	biq_stage1->a[1] = (1 - a)/(1 + a);

}

float omega( float center_freq ) {
	return (2*PI*center_freq/SAMPLES);
}

float alpha_bw( float center_freq, float bandwidth ) {
	float omega_value = omega(center_freq);
	float sin_omega = sinf(omega_value);
	float inside_sinh = 0.1505*bandwidth*omega_value/sin_omega;
	return sin_omega*sinhf(inside_sinh);
}

float alpha_q(float center_freq, float q_factor) {
	return (sin(omega(center_freq)) / (2 * q_factor));
}
