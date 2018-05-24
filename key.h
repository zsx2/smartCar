#ifndef KEY_H_H
#define KEY_H_H

#define Boma1 gpio_get(PTA1)
#define Boma2 gpio_get(PTE28)
#define Boma3 gpio_get(PTE26)


uint8 KeyScan(void) ;
void KeyRead();


#endif