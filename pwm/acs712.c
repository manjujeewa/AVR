
/*
 * get the current
 * current = (V - Vref/2) / sensitivity
 */

#include "acs712.h"

double acs712_getcurrent(double voltagein, double adcvref) {
	return (double)((double)(voltagein - (double)(adcvref/(double)2)) / (double)ACS712_SENSITIVITY);
}


