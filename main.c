
#include "include.h"


uint8 imgbuff[CAMERA_SIZE];
uint8 CData[CAMERA_H][CAMERA_W]; 
uint8 DataCopy[CAMERA_H][CAMERA_W]; 


uint8 Certre_Point=40;//ʵ������ 

uint16 PlusCount[4];
uint8 Dir[4];   //1 �� 3 �� ��ǰΪ 1  2�� 4 ����ǰΪ0
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
   /*�������ͷ�ɼ���ֵ��ʼ��*/
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
        vcan_sendimg(B_imgbuff, sizeof(B_imgbuff));//���͵���λ����ʾ
}
void image_sent2()
{
        uint8 i,j;
        uart_putchar(UART0,0xff);//����˵�������ڷ���һ���ֽ�        
        for(i=0;i<60;i++) 
           for(j=0;j<80;j++)
             {
                if(DataCopy[i][j]==0xff)
                 DataCopy[i][j]=0xfe;//�ı�����ֵ��ֹ����ΪͨѶ����
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
    //gpio_set(PTC0,0);//ָʾ����
    /*
    g_time_count_us = 0;
    enable_irq(PIT1_IRQn);//��PI1��ʱ��
    while(g_time_count_us<=10);
    //disable_irq(PIT1_IRQn);           //�رն�ʱ��
      gpio_set(Trig,0);
      //gpio_set(PTC0,1);//ָʾ����
 //     g_time_count_us=0; 
 //     test = gpio_get(Echo);
      while(gpio_get(Echo) == 0);
      enable_irq(PIT1_IRQn);//��PI1��ʱ��
      g_time_count_us=0; 
      while(gpio_get(Echo) == 1);
      disable_irq(PIT1_IRQn);           //�رն�ʱ
      //while(!(gpio_get(Echo) == 1)); //�ȴ�echo���������
      //while(gpio_get(Echo)==0);
      //gpio_set(PTC0,0);//ָʾ����
      
      //while(!(gpio_get(Echo) == 0));  //�ȴ�echo����½���
      distance = (g_time_count_us*5*340*0.000001)/2; //������룬��λCM
    */
    
}
void vcan_sendware(void *wareaddr, uint32_t waresize)
{
#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

    uart_putbuff(UART0, cmdf, sizeof(cmdf));    //�ȷ���ǰ����
    uart_putbuff(UART0, (uint8_t *)wareaddr, waresize);    //��������
    uart_putbuff(UART0, cmdr, sizeof(cmdr));    //���ͺ�����

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






