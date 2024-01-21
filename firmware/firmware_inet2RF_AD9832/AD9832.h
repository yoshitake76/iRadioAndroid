#ifndef _AD9832_H_
#define _AD9832_H_

#include <Arduino.h>

// pin configuration
#define FSYNC    4
#define SCLK     8
#define SDATA    6

void setTxFreq(uint32_t);
void initTX(void);


#endif
