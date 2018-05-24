#ifndef CONTROL_H_H
#define CONTROL_H_H
 
 //void Servo_contol(u8 Certre_Line);
void motor_control();
void MotorPI();
void move_forward(uint8 plus,int32 ms);
void car_stop();
void move_back(uint8 plus,int32 ms);
void move_Scircle(uint8 plus,int32 ms);
void move_Ncircle(uint8 plus,int32 ms);
void move_right(uint8 plus,int32 ms);
void move_left(uint8 plus,int32 ms);
void move_DiagobalQ(uint8 plus,int32 ms);
void move_DiagobalE(uint8 plus,int32 ms);
void move_DiagobalZ(uint8 plus,int32 ms);
void move_DiagobalC(uint8 plus,int32 ms);
void motor_testPI();
void Kmove_right(int32 ms);
void Kmove_left(int32 ms);
void Kmove_forward(int32 ms);
void Kmove_back(int32 ms);

 //void duoji_contol(u8 Certre_Line);
 void Car_Stop();
 void Bluetooth_Car();
#define Theory_centry_line 40

#endif