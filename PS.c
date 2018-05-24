#include "PS2.h"

//#include "usart.h"
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File: PS2 drive work
Author:  pinggai    Version:1.0     Data:2015/05/16
Description: PS2 drive work
**********************************************************/
#define DELAY_TIME  DELAY_US(50); 
uint16 Handkey=0;
uint8 Comd[2]={0x01,0x42};	//srart command  ask data
uint8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //array for data
uint16 MASK[]={
  PSB_SELECT,
  PSB_L3,
  PSB_R3 ,
  PSB_START,
  PSB_PAD_UP,
  PSB_PAD_RIGHT,
  PSB_PAD_DOWN,
  PSB_PAD_LEFT,
  PSB_L2,
  PSB_R2,
  PSB_L1,
  PSB_R1 ,
  PSB_GREEN,
  PSB_RED,
  PSB_BLUE,
  PSB_PINK
};	//

//PS2init      input   DI->PB0
//             output  DO->PB1    CS->PB4  CLK->PB5
void PS2_Init(void)
{
  //input  DI->PB0
  gpio_init (PTB0, GPI,0);
  port_init_NoALT (PTB0, PF | PULLDOWN );
  //  DO->PB1    CS->PB4  CLK->PB5 ÍÆÍìÊä³ö
  gpio_init (PTB1, GPO,0);
  gpio_init (PTB4, GPO,0);
  gpio_init (PTB5, GPO,0);
}

//send command to PS2
void PS2_Cmd(uint8 CMD)
{
  volatile uint16 ref=0x01;//
  Data[1] = 0;
  for(ref=0x01;ref<0x0100;ref<<=1)
  {
    if(ref&CMD)
    {
      DO_H;                   
    }
    else DO_L;
    
    CLK_H;                        
    DELAY_TIME;
    CLK_L;
    DELAY_TIME;
    CLK_H;
    if(DI==1)
      Data[1] = ref|Data[1];        
  }
} 
//judge mode(red/green)
//return  0  : red mode     ((0x73)
//        1  : green mode() (0x41)
uint8 PS2_RedLight(void)
{
  CS_L;
  PS2_Cmd(Comd[0]);  
  PS2_Cmd(Comd[1]); 
  CS_H;
  if( Data[1] == 0X73)   return 0 ;
  else return 1;
  
}
//read data from PS2
void PS2_ReadData(void)
{
  uint8 B=0;//volatile
  volatile uint16 ref=0x01;
  
  CS_L;
  
  PS2_Cmd(Comd[0]);  
  PS2_Cmd(Comd[1]);  
  
  for(B=2;B<9;B++)          
  {
    for(ref=0x01;ref<0x100;ref<<=1)
    {
      CLK_H;
      CLK_L;
      DELAY_TIME;
      CLK_H;
      if(DI==1)
        Data[B] = ref|Data[B];
    }
    DELAY_TIME;
  }
  CS_H;	
}

//handle key data    (red mode)     
//press :0 un-press :1
uint8 PS2_DataKey()
{
  uint8 index;
  
  PS2_ClearData();
  PS2_ReadData();
  
  Handkey=(Data[4]<<8)|Data[3];     
  for(index=0;index<16;index++)
  {	    
    if((Handkey&(1<<(MASK[index]-1)))==0)
      return index+1;
  }
  return 0;          
}

//get analog variable :0~256 (green mode)
uint8 PS2_AnologData(uint8 button)
{
  return Data[button];
}

//clear buff
void PS2_ClearData()
{
  uint8 a;
  for(a=0;a<9;a++)
    Data[a]=0x00;
}


/******************************************************
Function:    void PS2_Vibration(u8 motor1, u8 motor2)
Description: vibration function
Calls:		 void PS2_Cmd(u8 CMD);
Input: motor1:right motor (0 :close !0:open)
motor2:left motor  (value from 0x40 to 0xff)
******************************************************/
void PS2_Vibration(uint8 motor1, uint8 motor2)
{
  CS_L;
  DELAY_US(16);
  PS2_Cmd(0x01);  
  PS2_Cmd(0x42);  
  PS2_Cmd(0X00);
  PS2_Cmd(motor1);
  PS2_Cmd(motor2);
  PS2_Cmd(0X00);
  PS2_Cmd(0X00);
  PS2_Cmd(0X00);
  PS2_Cmd(0X00);
  CS_H;
  DELAY_US(16);  
}
//short poll
void PS2_ShortPoll(void)
{
  CS_L;
  DELAY_US(16);
  PS2_Cmd(0x01);  
  PS2_Cmd(0x42);  
  PS2_Cmd(0X00);
  PS2_Cmd(0x00);
  PS2_Cmd(0x00);
  CS_H;
  DELAY_US(16);	
}
//
void PS2_EnterConfing(void)
{
  CS_L;
  DELAY_US(16);
  PS2_Cmd(0x01);  
  PS2_Cmd(0x43);  
  PS2_Cmd(0X00);
  PS2_Cmd(0x01);
  PS2_Cmd(0x00);
  PS2_Cmd(0X00);
  PS2_Cmd(0X00);
  PS2_Cmd(0X00);
  PS2_Cmd(0X00);
  CS_H;
  DELAY_US(16);
}
//
void PS2_TurnOnAnalogMode(void)
{
  CS_L;
  PS2_Cmd(0x01);  
  PS2_Cmd(0x44);  
  PS2_Cmd(0X00);
  PS2_Cmd(0x01); 
  PS2_Cmd(0xEE); 
  
  PS2_Cmd(0X00);
  PS2_Cmd(0X00);
  PS2_Cmd(0X00);
  PS2_Cmd(0X00);
  CS_H;
  DELAY_US(16);
}
//
void PS2_VibrationMode(void)
{
  CS_L;
  DELAY_US(16);
  PS2_Cmd(0x01);  
  PS2_Cmd(0x4D);  
  PS2_Cmd(0X00);
  PS2_Cmd(0x00);
  PS2_Cmd(0X01);
  CS_H;
  DELAY_US(16);	
}

void PS2_ExitConfing(void)
{
  CS_L;
  DELAY_US(16);
  PS2_Cmd(0x01);  
  PS2_Cmd(0x43);  
  PS2_Cmd(0X00);
  PS2_Cmd(0x00);
  PS2_Cmd(0x5A);
  PS2_Cmd(0x5A);
  PS2_Cmd(0x5A);
  PS2_Cmd(0x5A);
  PS2_Cmd(0x5A);
  CS_H;
  DELAY_US(16);
}

void PS2_SetInit(void)
{
  PS2_ShortPoll();
  PS2_ShortPoll();
  PS2_ShortPoll();
  PS2_EnterConfing();		
  PS2_TurnOnAnalogMode();	
  PS2_VibrationMode();	
  PS2_ExitConfing();		
}

