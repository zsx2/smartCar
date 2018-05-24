#ifndef DEBUG_H_H
#define DEBUG_H_H

extern uint8 Mode;
extern uint8 Launch;


extern uint8 BlueTooth_flag;

extern float S_P,D_P;
//extern float P,D;
//extern u8 Certre_Line;
//extern s16 Servo_Duty;
//extern s16 Differ;
//extern s16 Motor_Differ;
//
//extern s16 Set_Speed_L,Set_Speed_R;
//extern s16 Set_Sp;
//extern s16 Set_P;
//extern s16 Set_I;
//extern s16 Speed_Duty_L;
//extern s16 Speed_Duty_R;
//extern s16 Speed_P,Speed_I;
//extern u8 Speed_flag;//是否低速
//extern u8 Oled_close;//是否显示OLED
void Oled() ;
void Oled_Inter();
#endif