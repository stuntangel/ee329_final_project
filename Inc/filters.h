#include <math.h>
#include "stm32l4xx_hal.h"
#include "filter_utils.h"
#include "stm32l4xx_it.h"
float* LPF(float bandwidth, float center_freq);
float* HPF(float bandwidth, float center_freq);
float* BPF_Amp_Q(float q_factor, float center_freq);
float* BPF_Amp_Unity(float bandwidth, float center_freq);
float* Notch(float bandwidth, float center_freq);
