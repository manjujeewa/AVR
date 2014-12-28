
#ifndef ACS712_H_
#define ACS712_H_

//defined sensitivity
#define ACS712_SENSITIVITY5 0.185
#define ACS712_SENSITIVITY20 0.100
#define ACS712_SENSITIVITY30 0.066

//setup sensitivity
#define ACS712_SENSITIVITY ACS712_SENSITIVITY30

extern double acs712_getcurrent(double voltagein, double adcvref);

#endif
