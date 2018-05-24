#include "include.h"
extern float S_P;
extern uint8 M_offset;
extern uint8 h_width;
extern uint8 find_item_flag;
extern uint8 L_width;
extern uint8 L_center;
extern uint8 distanceto_item;
extern int16 dianji1_Duty;
extern int16 dianji2_Duty;
extern uint8 Dis_turn ;
void Oled()
{
  if(Mode==1)
  {  
   LCD_Print(0,2,"S_P:"); 
   Dis_Float(36,2,(0+S_P ),3);
   LCD_Print(0,4,"SN");
   Dis_Float(36,4,(0+ 0 ),1);
   LCD_Print(0,6,"speed");
   Dis_Float(36,6,(0+ dianji2_Duty ),0);
  }
  
  if(Mode==2)
  {
    LCD_Print(0,2,"flag:"); 
    Dis_Float(37,2,(0+find_item_flag),2);
  
    LCD_Print(0,4,"L_CN:"); 
    Dis_Float(37,4,(0+L_center),2);
    
    LCD_Print(0,6,"Ldis:"); 
    Dis_Float(37,6,(0+ Dis_turn),2);
  }
  
   if(Mode==3)
  {
    LCD_Print(0,2,"h_width:"); 
    Dis_Num(38,2,h_width ,2); 
    
    LCD_Print(0,4,"L_width:"); 
    Dis_Num(38,4,L_width,2); 
    
    LCD_Print(0,6,"ccd:"); 
    Dis_Float(38,6,(0+distanceto_item),2);
  }
 
}


void Oled_Inter()
{
    LCD_Print(36,2,"Welcome");
//    if(FB_flag1==0)
//    {
     LCD_Print(32,4,"sinal car");
//    }
//    else
//    {
//     LCD_Print(36,4,"BackCar");
 //   }
    DELAY_MS(1000);
    LCD_Fill(0x00);
}
