#ifndef IMAGE_H_H
#define IMAGE_H_H
 
#include "math.h"


 
void Get_center(uint8 (*data)[DATACOUNT]);
void Get_center_qiang(uint8 (*data)[DATACOUNT],uint8 (*Tu)[DATACOUNT]);
void Get_distance(uint8*);
void Get_center_third(uint8(*data)[DATACOUNT]);
void Get_sinal_status();
void Image_PR(uint8(*data)[DATACOUNT]);
void removeNoise(uint8 imginput[][80]);
//void Get_center(u8 (*data)[DATACOUNT] ,u8 *Certre_point);
#endif