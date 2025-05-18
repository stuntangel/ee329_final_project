#include <math.h>

#define FFT_LENGTH 256
#define OMEGA (2*PI*CENTER_FREQ/SAMPLING_FREQ)
#define ALPHA_BW (sin(OMEGA)*sinh(log10(2)*BW*OMEGA/(2*sin(OMEGA))))