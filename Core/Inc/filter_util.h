
#ifndef INC_FILTER_UTIL_H_
#define INC_FILTER_UTIL_H_
typedef struct {
  float *a; //numerator coefficients
  float *b; //denominator coefficients

  int Xn1; //x[n-1] (prev input)
  int Xn2; //x[n-2] (prev prev input)

  int Yn1;    //y[n-1] (prev output)
  int Yn2;    //y[n-2] (prev prev output)
}biquad_t;
void computeBiquad(biquad_t *biq, float input, float* output);

#endif /* INC_FILTER_UTIL_H_ */
