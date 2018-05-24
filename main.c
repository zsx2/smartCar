
#include "include.h"


uint8 imgbuff[CAMERA_SIZE];
uint8 CData[CAMERA_H][CAMERA_W]; 
uint8 DataCopy[CAMERA_H][CAMERA_W]; 


uint8 Certre_Point=40;//实际中心 

uint16 PlusCount[4];
uint8 Dir[4];   //1 轮 3 轮 往前为 1  2轮 4 轮往前为0
extern uint16 g_time_count_us ;
extern uint32 g_time_count_Tms ;
uint8 test;
uint8 START = 0;

uint8 BUF[8] = {0};
int X,Y,Z;
double  Angle_XZ, Angle_YZ,Angle_XY;
int ANGLE_GET ;
double ANGLE[4] = {0};
double ANGLEZ[4] = {0};
int DUx;
int DUy;
int lastDUx;
int lastDUy;

void get_Distance();
void ware_test();
void runing();
void image_sent();
void image_sent2();

void main()
{
   Init();
   /*获得摄像头采集的值初始化*/
    while(1)
    {
       static uint8 count = 0; 
       KeyRead();
       motor_control();
       //motor_testPI();
   
       //Multiple_Read_QMC5883(&Angle_XY,&Angle_XZ,&Angle_YZ);
/*
        Multiple_Read_QMC5883(&Angle_XY,&Angle_XZ,&Angle_YZ);
        ANGLEZ[count] = Angle_XZ;
        ANGLE[count++] = Angle_XY;
        
        if(count>=4)
        {
          ANGLE_GET = 1; 
          count = 0;
        }
      if(ANGLE_GET)
      {
        lastDUx = DUx;
        lastDUy = DUy;
        double sum = 0,sum1 = 0;
        uint8 n = 0,z=0;
        uint8 i ,j;
        for (i = 0,j = 0;i<4;i++,j++)
        {
          if(fabs(220-ANGLE[i])<= 50)
          {
            sum+=ANGLE[i];
            n++;
          }
          if(fabs(220-ANGLEZ[j])<= 50)
          {
            sum1+=ANGLEZ[j];
            z++;
          }
        }
     
        DUx = sum/n;
        DUy = sum1/z;
        if(fabs(DUx-lastDUx) <=30)
        {
          if(DUx >= lastDUx)
          {
            
          }
        }
        
        ANGLE_GET = 0;
        
      }
       */
       //systick_delay_ms(100);
       
       //ware_test();
       //motor_testPI();
    }
      
}
void image_sent()
{
        uint8 B_imgbuff[CAMERA_SIZE];
        img_fpress(B_imgbuff, DataCopy,sizeof(CData));
        vcan_sendimg(B_imgbuff, sizeof(B_imgbuff));//发送到上位机显示
}
void image_sent2()
{
        uint8 i,j;
        uart_putchar(UART0,0xff);//功能说明：串口发送一个字节        
        for(i=0;i<60;i++) 
           for(j=0;j<80;j++)
             {
                if(DataCopy[i][j]==0xff)
                 DataCopy[i][j]=0xfe;//改变像素值防止误判为通讯结束
                 uart_putchar(UART0,DataCopy[i][j]);
             }  
}
void get_Distance()
{
    test = gpio_get(Echo);
    gpio_set(Trig,1);
    systick_delay_us(20);
    gpio_set(Trig,0);
    systick_delay_ms(100);
    //gpio_set(PTC0,0);//指示灯亮
    /*
    g_time_count_us = 0;
    enable_irq(PIT1_IRQn);//打开PI1定时器
    while(g_time_count_us<=10);
    //disable_irq(PIT1_IRQn);           //关闭定时器
      gpio_set(Trig,0);
      //gpio_set(PTC0,1);//指示灯亮
 //     g_time_count_us=0; 
 //     test = gpio_get(Echo);
      while(gpio_get(Echo) == 0);
      enable_irq(PIT1_IRQn);//打开PI1定时器
      g_time_count_us=0; 
      while(gpio_get(Echo) == 1);
      disable_irq(PIT1_IRQn);           //关闭定时
      //while(!(gpio_get(Echo) == 1)); //等待echo输出上升沿
      //while(gpio_get(Echo)==0);
      //gpio_set(PTC0,0);//指示灯灭
      
      //while(!(gpio_get(Echo) == 0));  //等待echo输出下降沿
      distance = (g_time_count_us*5*340*0.000001)/2; //计算距离，单位CM
    */
    
}
void vcan_sendware(void *wareaddr, uint32_t waresize)
{
#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //串口调试 使用的前命令
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //串口调试 使用的后命令

    uart_putbuff(UART0, cmdf, sizeof(cmdf));    //先发送前命令
    uart_putbuff(UART0, (uint8_t *)wareaddr, waresize);    //发送数据
    uart_putbuff(UART0, cmdr, sizeof(cmdr));    //发送后命令

}

void ware_test()
{
  static int t_list[2]= {0};
  /*
  t_list[0] = PlusCount[0]*20;
  t_list[1] = PlusCount[1]*20;
  t_list[2] = PlusCount[2]*20;
  t_list[3] = PlusCount[3]*20; 
  t_list[4] = 5*20;
  */
  t_list[0] = DUx;
  t_list[1] = DUy;
  //t_list[1] = Angle_XZ;
  //t_list[2] = Angle_YZ;
  vcan_sendware(t_list,sizeof(t_list));
}






