#include "include.h"



#if CAR1
int SeP[4] = {10,10,10,10};
int SeI[4] = {80,80,80,80};
int oneYPLUS = 1200;
extern char *Car_name;
uint8 mPLUS = 9;
#define waitM systick_delay_ms(200)
#define waitL systick_delay_ms(2000)
#else
int SeP[4] = {5,5,5,5};
int SeI[4] = {80,80,80,80};
int oneYPLUS = 2800;
extern char *Car_name;
uint8 mPLUS = 3;
#define waitM systick_delay_ms(200)
#define waitL systick_delay_ms(2500)
#endif 
int totalYPLUS = 4000;
int onePLUS = 1500;
extern char *CT;
extern uint8 PTP;
uint16 Plus_Offset_New[4] = {0};//这次偏差
uint16 Plus_Offset_Last[4] = {0};//上次偏差
uint16 Set_Plus[4] = {0};
int16 Add_Motor_Duty[4] = {0};
int16 Motor_Duty[4] = {0};
uint8 Set_Dir[4] = {1,1,1,1};
extern uint16 PlusCount[4];
extern uint32 g_time_count_Tms ; 
extern uint32 g_Ltime_count;
int angleX;
int last_angleX;
int CORRECT = 0;

extern double  Angle_XZ, Angle_YZ,Angle_XY;


uint8 Step = 0;
uint8 STOP  = 1;
extern float Distance;
extern uint8 BarFlag; 
volatile MODE_STATUS_e  mode = MODE_WAIT;
volatile PARCEL_GET_POS_e get_pos = GET_POS_UNF;
volatile PARCEL_PUT_POS_e put_pos = PUT_POS_UNF;
volatile CAR_POS_e car_pos = CAR_POS_UNF;
extern uint8 CH ;
static char AUTO_MOVE_STEP[100]={0};
static char AUTO_MOVE_COUNT = 0;
static uint32 AUTO_MOVE_TIME_STO[100] = {0};
void uart_putmystr(char *str)
{
  uart_putstr(UART0,CT);
  uart_putstr(UART0,Car_name);
  uart_putstr(UART0,str);
  uart_putchar (UART0, '\n');
}
void MotorPI()
{
  
  uint8 i;
    if( STOP == 0)
    {
     for( i = 0;i<=3;i++)
     {
         Plus_Offset_Last[i] = Plus_Offset_New[i];
         Plus_Offset_New[i] = Set_Plus[i]-PlusCount[i];
         Add_Motor_Duty[i]=(int16)(SeP[i]*(Plus_Offset_New[i]-Plus_Offset_Last[i])+SeI[i]*Plus_Offset_New[i]);
         Motor_Duty[i]+=Add_Motor_Duty[i]; 
         if(Motor_Duty[i] >= 5000) Motor_Duty[i] = 5000;
         if(Motor_Duty[i] <=0) Motor_Duty[i] = 0;
              if(Motor_Duty[i] <= 0) Motor_Duty[i] = 0;
     }

   if(Set_Dir[0] == 1)  FTM_PWM_Duty(FTM0, FTM_CH4, Motor_Duty[0]); //1轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH5, Motor_Duty[0]); //1轮反转
   if(Set_Dir[1] == 1)  FTM_PWM_Duty(FTM0, FTM_CH7, Motor_Duty[1]); //2轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH6, Motor_Duty[1]); //2轮反转
   if(Set_Dir[2] == 1)  FTM_PWM_Duty(FTM0, FTM_CH2, Motor_Duty[2]); //3轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH0, Motor_Duty[2]); //3轮反转
   if(Set_Dir[3] == 1)  FTM_PWM_Duty(FTM0, FTM_CH3, Motor_Duty[3]); //4轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH1, Motor_Duty[3]); //4轮正转
    } 
    else 
    {
        FTM_PWM_Duty(FTM0, FTM_CH0, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH7, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH6, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH5, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH4, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH3, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH2, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH1, 0); //
        for( i = 0;i<=3 ; i++)
        {
          Plus_Offset_New[i] = 0;
          Plus_Offset_Last[i] = 0;
          Add_Motor_Duty[i] = 0;
          Motor_Duty[i] = 0;
        }
    }         
}
void motor_testPI()
{
   Set_Plus[0] = 3;
   Set_Dir[0] = 0;
   Set_Plus[1] = 3;
   Set_Dir[1] = 0;
   Set_Plus[2] = 3;
   Set_Dir[2] = 0;
   Set_Plus[3] = 3;
   Set_Dir[3] = 0;
   STOP = 0;
}
void myint2str3(int data,char str[3])
{
  str[0] = data/100+'0';
  str[1] = data%100/10+'0';
  str[2] = data%10+'0';
}
void myint2str4(int data,char str[5])
{
  str[0] = data/10000+'0';
  str[1] = data%10000/1000+'0';
  str[2] = data%1000/100+'0';
  str[3] = data%100/10+'0';
  str[4] = data%10+'0';
}
void myint2str(int data[4],char str[8])
{
   str[0] = data[0]/10;
   str[1] = data[0]%10;
   str[2] = data[1]/10;
   str[3] = data[1]%10;
   str[4] = data[2]/10;
   str[5] = data[2]%10;
   str[6] = data[3]/10;
   str[7] = data[3]%10;
}
void AUTO_move_Default();
void AUTO_move_Fir();
void wait(uint16 ms)
{
    systick_delay_ms(ms);
}
void get_goods(int val)
{

   if(get_pos == GET_POS_UNF) return;
   if(car_pos  == CAR_POS_UNF)
      {
         if(val == 1)
         {
           move_right(mPLUS ,(get_pos-car_pos)*onePLUS);
         }
         else 
         {
           move_left(mPLUS ,(5-get_pos)*onePLUS);
         }
           if(get_pos-car_pos != 0) waitM;
           move_back(mPLUS,onePLUS);
           waitL;
           car_pos = get_pos;
      }
      else 
      {
        move_back(mPLUS,oneYPLUS);
        if(get_pos-car_pos != 0) waitM;
        if(get_pos >= car_pos)
        {
          move_right(mPLUS,(get_pos-car_pos)*onePLUS);
          if(get_pos-car_pos != 0) waitM;
          move_back(mPLUS,totalYPLUS-oneYPLUS);
          waitL;
          car_pos = get_pos;
        }
        else
        {
          move_left(mPLUS,(car_pos-get_pos)*onePLUS);
          if(get_pos-car_pos != 0) waitM;
          move_back(mPLUS,totalYPLUS-oneYPLUS);
          waitL;
          car_pos = get_pos;
        }
       }
}

void put_goods()
{
    if(put_pos == PUT_POS_UNF) return;
    move_forward(mPLUS,totalYPLUS-oneYPLUS);
    if(put_pos-car_pos != 0) waitM;
    if(put_pos >= car_pos)
    {
        move_right(mPLUS,(put_pos-car_pos)*onePLUS);
        if(put_pos-car_pos != 0) waitM;
        move_forward(mPLUS,oneYPLUS);
        waitL;
        car_pos = put_pos;
    }
    else
    {
      move_left(mPLUS,(car_pos-put_pos)*onePLUS);
      if(put_pos-car_pos != 0) waitM;
      move_forward(mPLUS,oneYPLUS);
      waitL;
      car_pos = put_pos;
    }
    uart_putmystr("YES"); 
}
void get_current_angle()
{
  Multiple_Read_QMC5883(&Angle_XY,&Angle_XZ,&Angle_YZ);
  angleX  = Angle_XY;
  if(fabs(angleX-last_angleX)<=35)
  {
    last_angleX = angleX;
  }
  else angleX = last_angleX;
}
void correct()
{
  if(CORRECT) CORRECT= 0;
  else CORRECT = 1;
  uart_putstr(UART0,CT); 
  uart_putstr(UART0,Car_name);
  if(CORRECT)
  uart_putstr(UART0,"open correct!");
  else
  uart_putstr(UART0,"close correct!");
}
void fist_get_current_angle()
{
  uint8 i ,j;
  char str[3];
  int anglex[10];
  int temp;
  for(i = 0;i<10;i++)
  {
    Multiple_Read_QMC5883(&Angle_XY,&Angle_XZ,&Angle_YZ);
    anglex[i] = (int)Angle_XY;
  }
  for(i = 0;i<9;i++)
  {
    for(j=0;j<9;j++)
    {
      if(anglex[j]>anglex[j+1])
      {
        temp = anglex[j];
        anglex[j] = anglex[j+1];
        anglex[j+1] = temp;
      }
    }
  }
  angleX = anglex[4];
  last_angleX = angleX;
  myint2str3(angleX,str);
  uart_putstr(UART0,CT); 
  uart_putstr(UART0,Car_name);
  uart_putstr(UART0,"angX=");
  uart_putstr(UART0,str);
}
void motor_control()
{
  if(CORRECT)
    get_current_angle();
  if(CH == 'O') {mode = MODE_AUTO; uart_putstr(UART0,CT); uart_putstr(UART0,Car_name);  uart_putstr  (UART0, "Current Mode is Mode_Auto\n");CH = 0;}
  if(CH == 'H') {mode = MODE_HAND; uart_putstr(UART0,CT); uart_putstr(UART0,Car_name);  uart_putstr  (UART0, "Current Mode is Mode_Hand\n");CH = 0;}
  if(CH == 'W') {mode = MODE_WAIT; uart_putstr(UART0,CT); uart_putstr(UART0,Car_name);  uart_putstr  (UART0, "Current Mode is Mode_Wait\n");CH = 0;}
  if(CH == 'L') {mode = MODE_LEARNING; uart_putstr(UART0,CT); uart_putstr(UART0,Car_name);  uart_putstr  (UART0, "Current Mode is Mode_Learn\n");CH= 0;}
  if(CH == 'P') {mode = MODE_PTP; uart_putstr(UART0,CT); uart_putstr(UART0,Car_name);  uart_putstr  (UART0, "Current Mode is PTP\n");CH = 0;}
  if(CH == 'M') {mode = MODE_MTM; uart_putstr(UART0,CT); uart_putstr(UART0,Car_name);  uart_putstr  (UART0, "Current Mode is MTM\n");CH = 0;}
    if(mode == MODE_HAND)
    {
#if KAI
      if(CH == 'w') {Kmove_forward(-1); CH = 0;}
      if(CH == 's') {Kmove_back(-1); CH = 0;}
      if(CH == 'a') {Kmove_left(-1); CH = 0;}
      if(CH == 'd') {Kmove_right(-1);CH = 0;}
      if(CH == 'x') {car_stop();CH = 0;}
#else
      if(CH == 'w') {move_forward(mPLUS,-1); CH = 0;}
      if(CH == 's') {move_back(mPLUS,-1); CH = 0;}
      if(CH == 'a') {move_left(mPLUS,-1); CH = 0;}
      if(CH == 'd') {move_right(mPLUS,-1);CH = 0;}
      if(CH == 'x') {car_stop();CH = 0;}
      if(CH == 'q') {move_DiagobalQ(mPLUS,-1); CH = 0;}
      if(CH == 'e') {move_DiagobalE(mPLUS ,-1);CH = 0;}
      if(CH == 'z') {move_DiagobalZ(mPLUS ,-1);CH = 0;}
      if(CH == 'n') {move_DiagobalC(mPLUS ,-1);CH = 0;}
      if(CH == 'r') {move_Scircle(mPLUS, -1);CH = 0;}
      if(CH == 't') {move_Ncircle(mPLUS, -1);CH = 0;}
#endif
    }
    if(mode == MODE_WAIT)
      {
        car_stop();
        if(CH == 'w') {fist_get_current_angle(); CH = 0;}
        if(CH == 'x') {correct();CH = 0;}
      }
    if(mode == MODE_AUTO)
    {
      if(CH == 'n'){AUTO_move_Default(); CH = 0;}
      if(CH == 'z'){AUTO_move_Fir();CH = 0;}
    }
    if(mode == MODE_LEARNING)
    {
      if(CH == 'w' || CH == 'a' || CH == 's' || CH ==  'd' || CH == 's' || CH == 'q' || CH ==  'e') 
      {
        AUTO_MOVE_STEP[AUTO_MOVE_COUNT] = CH;
        AUTO_MOVE_COUNT++;
        enable_irq(PIT2_IRQn);//打开PI2定时器
          switch (CH)
       {
       case 'w': move_forward(mPLUS,-1);break;
           case 's': move_back(mPLUS,-1);break;
                case 'a': move_left(mPLUS,-1);break;
                      case 'd': move_right(mPLUS,-1);break;
                            case 'q': move_DiagobalQ(mPLUS,-1);break;
                                  case 'e': move_DiagobalE(mPLUS,-1);break;
                                        case 'z': move_DiagobalZ(mPLUS,-1);break;
                                                case 'n': move_DiagobalC(mPLUS,-1);break;
                                                    case 'r': move_Scircle(mPLUS, -1);break;
                                                          case 't': move_Ncircle(mPLUS, -1);break;
         }
        CH = 0;
      }
      if(CH == 'x')
      {
        char str[5] = {0};
           disable_irq(PIT2_IRQn);
           car_stop();
           AUTO_MOVE_TIME_STO[AUTO_MOVE_COUNT-1] =  g_Ltime_count*5;
           myint2str4(AUTO_MOVE_TIME_STO[AUTO_MOVE_COUNT-1],str);
           uart_putstr(UART0,str);
           uart_putchar (UART0, '\n');
           g_Ltime_count = 0;
           CH = 0;
      }
      if(CH == 'Y')
      {
        uart_putstr(UART0,CT);
        uart_putstr(UART0,Car_name);
        uart_putstr  (UART0, "Learning Done! It has");
        uart_putchar (UART0,AUTO_MOVE_COUNT+'0');
        uart_putstr  (UART0, "step,to run it please press z under auto_mode\n");
        CH = 0;
      }
      if(CH == 'C')
      {
        uart_putstr(UART0,CT);
        uart_putstr(UART0,Car_name);
        uart_putstr  (UART0, "Remove memory Done! Please relearn!\n");
        for(uint8 j = 0; j<AUTO_MOVE_COUNT ; j++)
        {
          AUTO_MOVE_STEP[j] = 0 ;
          AUTO_MOVE_TIME_STO[j] = 0;
        }
        AUTO_MOVE_COUNT = 0;
      }
    }

    if(BarFlag  == 1 )
    {
        move_right(15,1500);
        systick_delay_ms(500);
        move_forward(15,1500);  
        systick_delay_ms(500);
        move_left(15,1500);
        systick_delay_ms(500);
        BarFlag  = 0;
    }
    if(mode == MODE_PTP)
    {
      if(PTP == 1)
      {
        if(get_pos == GET_POS_UNF) return;
        get_goods(1);
        put_goods();
        PTP = 0 ;
      }  
    }
    if(mode == MODE_MTM)
    {
       if(PTP == 1)
       {
#if CAR1
         get_goods(1);
#else
         get_goods(2);
#endif
         put_goods();
         PTP = 0 ;
       }
    }
    if(mode == MODE_PID)
    {
      static uint8 num = 0;
      static uint8 isP = 1;
      char str[8];
      if(CH == 'w') {isP?SeP[num]++:SeI[num]++; CH = 0;}
      if(CH == 's') {isP?SeP[num]--:SeI[num]--; CH = 0;}
      if(CH == 'a') {if(num>0) num--;else num = 0; CH = 0;}
      if(CH == 'd') {if(num < 3) num++;else num = 3; CH = 0;}
      if(CH == 'x') {if(isP)isP = 0; else isP = 1;CH = 0;}
      if(CH == 'z') 
      {
          uart_putstr(UART0,CT); 
          uart_putstr(UART0,Car_name);
          uart_putstr(UART0,"SeP=");
           str[0] = SeP[0]/10+'0';
           str[1] = SeP[0]%10+'0';
           str[2] = SeP[1]/10+'0';
           str[3] = SeP[1]%10+'0';
           str[4] = SeP[2]/10+'0';
           str[5] = SeP[2]%10+'0';
           str[6] = SeP[3]/10+'0';
           str[7] = SeP[3]%10+'0';
          uart_putstr(UART0,str);
          uart_putstr(UART0,"SeI=");
           str[0] = SeI[0]/10+'0';
           str[1] = SeI[0]%10+'0';
           str[2] = SeI[1]/10+'0';
           str[3] = SeI[1]%10+'0';
           str[4] = SeI[2]/10+'0';
           str[5] = SeI[2]%10+'0';
           str[6] = SeI[3]/10+'0';
           str[7] = SeI[3]%10+'0';
          uart_putstr(UART0,str);
          CH = 0;
      }
    }   
}
void AUTO_move_Default()
{
     move_forward(10,3000);  
     systick_delay_ms(500);
     move_right(10,3000);
     systick_delay_ms(500);
     move_Ncircle(15,1150);//顺时针转90度
     systick_delay_ms(500);
     move_left(10,3000);
     systick_delay_ms(500);
     move_Scircle(15,1150);//顺时针转90度
     systick_delay_ms(500);
     move_left(10,3000);
     //systick_delay_ms(500);
    //move_Scircle(15,1150);//顺时针转90度
  
}
void AUTO_move_Fir()
{
  static uint8 temp = 0;
  if(AUTO_MOVE_COUNT == 0) uart_putstr  (UART0, "\nHavn't finish learning!");
  else
  {
    if(temp == 0)
    {
      uart_putstr  (UART0, "\n 1 second later auto run!");
      systick_delay_ms(1000);
      temp = 1;
    }
    for(uint8 i = 0;i<AUTO_MOVE_COUNT;i++)
    {
       switch (AUTO_MOVE_STEP[i])
       {
       case 'w': move_forward(mPLUS,AUTO_MOVE_TIME_STO[i]);systick_delay_ms(500);break;
           case 's': move_back(mPLUS,AUTO_MOVE_TIME_STO[i]);systick_delay_ms(500);break;
                case 'a': move_left(mPLUS,AUTO_MOVE_TIME_STO[i]);systick_delay_ms(500);break;
                      case 'd': move_right(mPLUS,AUTO_MOVE_TIME_STO[i]);systick_delay_ms(500);break;
                            case 'r': move_Ncircle(mPLUS,AUTO_MOVE_TIME_STO[i]);systick_delay_ms(500);break;
                                  case 't': move_Scircle(mPLUS,AUTO_MOVE_TIME_STO[i]);systick_delay_ms(500);break;
       default: car_stop();
       }
    }
  }
}
void move_forward(uint8 plus,int32 ms)
{
   uint8 i;
   STOP = 0;
#if CAR1
   
  SeI[0] = 70;
  SeI[1] = 60;
  SeI[2] = 70;
  SeI[3] = 60;
#else
  SeI[0] = 60;
  SeI[1] = 70;
  SeI[2] = 60;
  SeI[3] = 70;
#endif
   if(CORRECT)
   {
     if(angleX >= last_angleX)
     {
       Set_Plus[0] = plus;
       Set_Dir[0] = 1;
       Set_Plus[1] = plus+1;
       Set_Dir[1] = 1;
       Set_Plus[2] = plus;
       Set_Dir[2] = 1;
       Set_Plus[3] = plus+1;
       Set_Dir[3] = 1;
     }
     else 
     {
       Set_Plus[0] = plus+1;
       Set_Dir[0] = 1;
       Set_Plus[1] = plus;
       Set_Dir[1] = 1;
       Set_Plus[2] = plus+1;
       Set_Dir[2] = 1;
       Set_Plus[3] = plus;
       Set_Dir[3] = 1;
     }
   }
   else
   {
     for(i = 0;i<=3;i++)
     {
       Set_Plus[i] = plus;
       Set_Dir[i] = 1;
     }
   }

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void Kmove_forward(int32 ms)
{
   uint8 i;
   STOP = 0;
   for(i = 0;i<=3;i++)
   {
     Set_Dir[i] = 1;
   }
#if CAR1
   Motor_Duty[0] = 3150;
   Motor_Duty[1] = 2800;
   Motor_Duty[2] = 2900;
   Motor_Duty[3] = 3000;
#else
   Motor_Duty[0] = 3000;
   Motor_Duty[1] = 3000;
   Motor_Duty[2] = 3000;
   Motor_Duty[3] = 3000;
#endif
   if(Set_Dir[0] == 1)  FTM_PWM_Duty(FTM0, FTM_CH4, Motor_Duty[0]); //1轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH5, Motor_Duty[0]); //1轮反转
   if(Set_Dir[1] == 1)  FTM_PWM_Duty(FTM0, FTM_CH7, Motor_Duty[1]); //2轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH6, Motor_Duty[1]); //2轮反转
   if(Set_Dir[2] == 1)  FTM_PWM_Duty(FTM0, FTM_CH2, Motor_Duty[2]); //3轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH0, Motor_Duty[2]); //3轮反转
   if(Set_Dir[3] == 1)  FTM_PWM_Duty(FTM0, FTM_CH3, Motor_Duty[3]); //4轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH1, Motor_Duty[3]); //4轮正转

 //  MotorPI()
   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void Kmove_back(int32 ms)
{
   uint8 i;
   STOP = 0;
   for(i = 0;i<=3;i++)
   {
     Set_Dir[i] = 0;
   }
#if CAR1
   Motor_Duty[0] = 3100;
   Motor_Duty[1] = 2800;
   Motor_Duty[2] = 3150;
   Motor_Duty[3] = 2850;
#else
   Motor_Duty[0] = 3000;
   Motor_Duty[1] = 3000;
   Motor_Duty[2] = 3000;
   Motor_Duty[3] = 3000;
#endif
   if(Set_Dir[0] == 1)  FTM_PWM_Duty(FTM0, FTM_CH4, Motor_Duty[0]); //1轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH5, Motor_Duty[0]); //1轮反转
   if(Set_Dir[1] == 1)  FTM_PWM_Duty(FTM0, FTM_CH7, Motor_Duty[1]); //2轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH6, Motor_Duty[1]); //2轮反转
   if(Set_Dir[2] == 1)  FTM_PWM_Duty(FTM0, FTM_CH2, Motor_Duty[2]); //3轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH0, Motor_Duty[2]); //3轮反转
   if(Set_Dir[3] == 1)  FTM_PWM_Duty(FTM0, FTM_CH3, Motor_Duty[3]); //4轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH1, Motor_Duty[3]); //4轮正转

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void Kmove_left(int32 ms)
{
        STOP = 0;
       uint8 i;
   for(i = 0;i<=3;i++)
   {
     if(i == 0 || i == 3)  Set_Dir[i] = 0;
     else   Set_Dir[i] = 1; 
   }
#if CAR1
   Motor_Duty[0] = 3000;
   Motor_Duty[1] = 3000;
   Motor_Duty[2] = 3000;
   Motor_Duty[3] = 3000;
#else
   Motor_Duty[0] = 3000;
   Motor_Duty[1] = 3000;
   Motor_Duty[2] = 3000;
   Motor_Duty[3] = 3000;
#endif
   if(Set_Dir[0] == 1)  FTM_PWM_Duty(FTM0, FTM_CH4, Motor_Duty[0]); //1轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH5, Motor_Duty[0]); //1轮反转
   if(Set_Dir[1] == 1)  FTM_PWM_Duty(FTM0, FTM_CH7, Motor_Duty[1]); //2轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH6, Motor_Duty[1]); //2轮反转
   if(Set_Dir[2] == 1)  FTM_PWM_Duty(FTM0, FTM_CH2, Motor_Duty[2]); //3轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH0, Motor_Duty[2]); //3轮反转
   if(Set_Dir[3] == 1)  FTM_PWM_Duty(FTM0, FTM_CH3, Motor_Duty[3]); //4轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH1, Motor_Duty[3]); //4轮正转

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void Kmove_right(int32 ms)
{
        STOP = 0;
       uint8 i;
   for(i = 0;i<=3;i++)
   {
     if(i == 0 || i == 3)  Set_Dir[i] = 1;
     else   Set_Dir[i] = 0; 
   }
#if CAR1
   Motor_Duty[0] = 3000;
   Motor_Duty[1] = 3000;
   Motor_Duty[2] = 3000;
   Motor_Duty[3] = 3000;
#else
   Motor_Duty[0] = 3000;
   Motor_Duty[1] = 3000;
   Motor_Duty[2] = 3000;
   Motor_Duty[3] = 3000;
#endif
   if(Set_Dir[0] == 1)  FTM_PWM_Duty(FTM0, FTM_CH4, Motor_Duty[0]); //1轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH5, Motor_Duty[0]); //1轮反转
   if(Set_Dir[1] == 1)  FTM_PWM_Duty(FTM0, FTM_CH7, Motor_Duty[1]); //2轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH6, Motor_Duty[1]); //2轮反转
   if(Set_Dir[2] == 1)  FTM_PWM_Duty(FTM0, FTM_CH2, Motor_Duty[2]); //3轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH0, Motor_Duty[2]); //3轮反转
   if(Set_Dir[3] == 1)  FTM_PWM_Duty(FTM0, FTM_CH3, Motor_Duty[3]); //4轮正转
   else             FTM_PWM_Duty(FTM0, FTM_CH1, Motor_Duty[3]); //4轮正转

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void car_stop()
{
  uint8 i;
   STOP = 1;
     for(i = 0;i<=3;i++)
   {
     Set_Plus[i] = 0;
     Set_Dir[i] = 1;
   }
        FTM_PWM_Duty(FTM0, FTM_CH0, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH7, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH6, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH5, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH4, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH3, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH2, 0); //
        FTM_PWM_Duty(FTM0, FTM_CH1, 0); //
   
  // MotorPI();
}
void move_back(uint8 plus,int32 ms)
{
     uint8 i;
     STOP = 0; 
     
#if CAR1
  SeI[0] = 60;
  SeI[1] = 60;
  SeI[2] = 60;
  SeI[3] = 60;
#else
  SeI[0] = 60;
  SeI[1] = 60;
  SeI[2] = 60;
  SeI[3] = 60;
#endif
    if(CORRECT)
   {
     if(angleX >= last_angleX)
     {
       Set_Plus[0] = plus;
       Set_Dir[0] = 1;
       Set_Plus[1] = plus+1;
       Set_Dir[1] = 1;
       Set_Plus[2] = plus;
       Set_Dir[2] = 1;
       Set_Plus[3] = plus+1;
       Set_Dir[3] = 1;
     }
     else 
     {
       Set_Plus[0] = plus+1;
       Set_Dir[0] = 1;
       Set_Plus[1] = plus;
       Set_Dir[1] = 1;
       Set_Plus[2] = plus+1;
       Set_Dir[2] = 1;
       Set_Plus[3] = plus;
       Set_Dir[3] = 1;
     }
   }
   else
   {
     for(i = 0;i<=3;i++)
     {
       Set_Plus[i] = plus;
       Set_Dir[i] = 0;
     }
   }
   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void move_Scircle(uint8 plus,int32 ms)
{
        STOP = 0;
     uint8 i;
   for(i = 0;i<=3;i++)
   {

     Set_Plus[i] = plus;
     if(i%2 == 0)  Set_Dir[i] = 1;
     else   Set_Dir[i] = 0; 
   }

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void move_Ncircle(uint8 plus,int32 ms)
{
   STOP = 0;
   uint8 i;
   for(i = 0;i<=3;i++)
   {

     Set_Plus[i] = plus;
     if(i%2 == 0)  Set_Dir[i] = 0;
     else   Set_Dir[i] = 1; 
   }

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void move_DiagobalQ(uint8 plus,int32 ms)
{
   STOP = 0;
   Set_Plus[0] = plus;
   Set_Dir[0] = 1;
   Set_Plus[1] = 0;
   Set_Dir[1] = 1;
   Set_Plus[2] = 0;
   Set_Dir[2] = 1;
   Set_Plus[3] = plus;
   Set_Dir[3] = 1;

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void move_DiagobalE(uint8 plus,int32 ms)
{
   STOP = 0;
   Set_Plus[0] = 0;
   Set_Dir[0] = 1;
   Set_Plus[1] = plus;
   Set_Dir[1] = 1;
   Set_Plus[2] = plus;
   Set_Dir[2] = 1;
   Set_Plus[3] = 0;
   Set_Dir[3] = 1;

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void move_DiagobalZ(uint8 plus,int32 ms)
{
   STOP = 0;
   Set_Plus[0] = 0;
   Set_Dir[0] = 1;
   Set_Plus[1] = plus;
   Set_Dir[1] = 0;
   Set_Plus[2] = plus;
   Set_Dir[2] = 0;
   Set_Plus[3] = 0;
   Set_Dir[3] = 1;

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void move_DiagobalC(uint8 plus,int32 ms)
{
   STOP = 0;
   Set_Plus[0] = plus;
   Set_Dir[0] = 0;
   Set_Plus[1] = 0;
   Set_Dir[1] = 1;
   Set_Plus[2] = 0;
   Set_Dir[2] = 1;
   Set_Plus[3] = plus;
   Set_Dir[3] = 0;

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void move_left(uint8 plus,int32 ms)
{
        STOP = 0;
       uint8 i;
#if CAR1
  SeI[0] = 80;
  SeI[1] = 65;
  SeI[2] = 65;
  SeI[3] = 80; 
#else
  SeI[0] = 70;
  SeI[1] = 80;
  SeI[2] = 80;
  SeI[3] = 70;
#endif
       if(CORRECT)
   {
     if(angleX >= last_angleX)
     {
       Set_Plus[0] = plus;
       Set_Dir[0] = 1;
       Set_Plus[1] = plus+1;
       Set_Dir[1] = 1;
       Set_Plus[2] = plus;
       Set_Dir[2] = 1;
       Set_Plus[3] = plus+1;
       Set_Dir[3] = 1;
     }
     else 
     {
       Set_Plus[0] = plus+1;
       Set_Dir[0] = 1;
       Set_Plus[1] = plus;
       Set_Dir[1] = 1;
       Set_Plus[2] = plus+1;
       Set_Dir[2] = 1;
       Set_Plus[3] = plus;
       Set_Dir[3] = 1;
     }
   }
   else
   {
     for(i = 0;i<=3;i++)
     {

       Set_Plus[i] = plus;
       if(i == 0 || i == 3)  Set_Dir[i] = 0;
       else   Set_Dir[i] = 1; 
     }
   }

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}

void move_right(uint8 plus,int32 ms)
{
        STOP = 0;
       uint8 i;
 
#if CAR1
  SeI[0] = 60;
  SeI[1] = 80;
  SeI[2] = 80;
  SeI[3] = 60;  
#else
  SeI[0] = 80;
  SeI[1] = 80;
  SeI[2] = 80;
  SeI[3] = 80;
#endif      
    if(CORRECT)
   {
     if(angleX >= last_angleX)
     {
       Set_Plus[0] = plus;
       Set_Dir[0] = 1;
       Set_Plus[1] = plus+1;
       Set_Dir[1] = 1;
       Set_Plus[2] = plus;
       Set_Dir[2] = 1;
       Set_Plus[3] = plus+1;
       Set_Dir[3] = 1;
     }
     else 
     {
       Set_Plus[0] = plus+1;
       Set_Dir[0] = 1;
       Set_Plus[1] = plus;
       Set_Dir[1] = 1;
       Set_Plus[2] = plus+1;
       Set_Dir[2] = 1;
       Set_Plus[3] = plus;
       Set_Dir[3] = 1;
     }
   }
   else
   {    
     for(i = 0;i<=3;i++)
     {

       Set_Plus[i] = plus;
       if(i == 0 || i == 3)  Set_Dir[i] = 1;
       else   Set_Dir[i] = 0; 
     }
   }

   if(ms != -1)
   {
     systick_delay_ms(ms);
     car_stop();
   }
}


 

