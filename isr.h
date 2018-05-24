
#ifndef __ISR_H
#define __ISR_H 1

#include  "include.h"

extern void PORTA_IRQHandler();
extern void PORTC_IRQHandler();           //PORTA�жϷ�����
extern void PORTB_IRQHandler();           //PORTB�жϷ�����
extern void FTM0_IRQHandler();            //FTM0���벶׽�ж�
extern void FTM1_IRQHandler();            //FTM0���벶׽�ж�
void DMA4_IRQHandler();
void DMA0_IRQHandler();
void PIT0_IRQHandler();
void PIT1_IRQHandler();
void PIT2_IRQHandler();
void FTM2_IRQHandler();
void UART0_IRQHandler();
#endif  //__ISR_H

/* End of "isr.h" */
