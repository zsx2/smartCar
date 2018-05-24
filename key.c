#include "include.h"
extern uint8 START; 
extern uint8 BarFlag ;
extern int SeP[4];
extern int SeI[4];

uint8 KeyScan(void) 
{
  if (gpio_get(PTA9)==0)  {systick_delay_ms(5);if (gpio_get(PTA9)==0){while (!gpio_get(PTA9)) ;return 1;}}
  if (gpio_get(PTA8)==0)  {systick_delay_ms(5);if (gpio_get(PTA8)==0){while (!gpio_get(PTA8)) ;return  2;}}
  if (gpio_get(PTA10)==0)  {systick_delay_ms(5);if (gpio_get(PTA10)==0){while (!gpio_get(PTA10)) ;return  3;}}
  if (gpio_get(PTA14)==0)  {systick_delay_ms(5);if (gpio_get(PTA14)==0){while (!gpio_get(PTA14)) ; return 4;}}
  else return 0 ;
  return 0 ;
}

void  KeyRead()
{
  
  uint8 key;
  key = KeyScan();
  if(key == 1)
  {
    SeP[1]++;
  }
  if(key == 2)
  {
    SeP[1]--;
  }
   if(key == 3)
  {
    SeI[1]++;
  }
  if(key == 4)
  {
    SeI[1]--;
  }
}