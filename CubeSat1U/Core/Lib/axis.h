#ifndef AXIS_H
#define AXIS_H

#include "stdio.h"
#include "stm32h7xx_hal.h"
typedef struct axis3_t
{
  int16_t x;
  int16_t y;
  int16_t z;
};

typedef struct faxis3_t
{
  float x;
  float y;
  float z;
};


#endif