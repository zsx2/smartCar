#ifndef PS2_H_H
#define PS2_H_H
#include "include.h"
#include "common.h"

#define DI    (GPIOB_PDIR & (0x01))      //read DI (PTB0)
  
#define DO_H  GPIO_PDOR_REG(GPIOX_BASE(PTB1))  |= (1 << PTn(PTB1))     //DO output 1
#define DO_L  GPIO_PDOR_REG(GPIOX_BASE(PTB1)) &= ~(1 << PTn(PTB1))    //   output 0

#define CS_H  GPIO_PDOR_REG(GPIOX_BASE(PTB4))  |= (1 << PTn(PTB4))    //CS  output 1
#define CS_L  GPIO_PDOR_REG(GPIOX_BASE(PTB4)) &= ~(1 << PTn(PTB4))    //    output 0

#define CLK_H GPIO_PDOR_REG(GPIOX_BASE(PTB5))|= (1 << PTn(PTB5))   //CLK  output 1
#define CLK_L GPIO_PDOR_REG(GPIOX_BASE(PTB5))&= ~(1 << PTn(PTB5))   //     output 0


//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      26

//#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX 5                //¨®¨°¨°???X?¨¢¨ºy?Y
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8





void PS2_Init(void);
uint8 PS2_RedLight(void);
void PS2_ReadData(void);
void PS2_Cmd(uint8 CMD);		  //
uint8 PS2_DataKey(void);		  
uint8 PS2_AnologData(uint8 button); 
void PS2_ClearData(void);	  //

void PS2_Vibration(uint8 motor1, uint8 motor2);//R:motor1 (colse=>0x00 open=> else value)   L:motor2 (0x40~0xFF)
void PS2_EnterConfing(void);	 //
void PS2_TurnOnAnalogMode(void); //¡¤
void PS2_VibrationMode(void);    //
void PS2_ExitConfing(void);	     //
void PS2_SetInit(void);		     //motor init

#endif