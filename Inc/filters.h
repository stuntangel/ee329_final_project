#include <math.h>
#include "stm32l4xx_hal.h"
#include "filter_utils.h"
#include "stm32l4xx_it.h"
float* computeLowpassCoeffs(float bandwidth, float center_freq);
