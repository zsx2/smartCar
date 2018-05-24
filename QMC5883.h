#ifndef QMC5883_H_H
#define QMC5883_H_H

#include "include.h"

uint8 QMC5883_ReadReg(uint8 Reg);
void QMC5883_WriteReg(uint8 Val,uint8 Reg);
uint8_t QMC5883_InitConfig(void);
void QMC5883_GetData(float *angle);
void QMC5883_parameter_dispaly(void);

#endif