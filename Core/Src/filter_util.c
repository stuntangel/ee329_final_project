#include "filter_util.h"

void computeBiquad(biquad_t *biq, float input, float *output) {
  int acc = 0;

  //direct form 1 difference equation
  acc += input*biq->b[0];
  acc += biq->Xn1*biq->b[1];
  acc += biq->Xn2*biq->b[2];

  acc -= biq->Yn1*biq->a[1];
  acc -= biq->Yn2*biq->a[2];

  //update past inputs
  biq->Xn2 = biq->Xn1;
  biq->Xn1 = input;

  //update past outputs
  biq->Yn2 = biq->Yn1;
  biq->Yn1 = acc;

  //output sample
  *output = acc;
}
