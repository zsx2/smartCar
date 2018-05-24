/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��isr.c
 * ����         ���жϴ�������
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/


#include "common.h"
#include "include.h"
extern uint8   *ov7725_eagle_img_buff;
extern uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
extern uint8 CData[CAMERA_H][CAMERA_W];
extern uint8 DataCopy[CAMERA_H][CAMERA_W];
uint32 g_time_count_Tms = 0 ;
uint32 g_Ltime_count = 0;
float Distance=0.0;
//extern uint8 img[CAMERA_H][CAMERA_W];
//extern uint8 imgM[CAMERA_H][CAMERA_W];
//extern uint8 pid_flage;
//extern uint8 blue_flage;
//extern uint8 sdFlage;
//***����ͷ�ɼ��ʹ���


extern uint16 PlusCount[4];
extern uint8 Dir[4];
uint8 BarFlag = 0;
uint8 CH = 0;
uint8 PTP = 0;
uint8 str[7];
char *CT = "30";
volatile MESG_SendTo goter = None;
volatile Mesg_Sender sender = None2;
extern volatile PARCEL_GET_POS_e get_pos ;
extern volatile PARCEL_PUT_POS_e put_pos ;
extern volatile MODE_STATUS_e  mode;

#if CAR1
char *Car_name = "10";
volatile Carname myname = Name_Car1;
volatile Carname otherCar = Name_Car2;
#else
char *Car_name = "20";
volatile Carname myname = Name_Car2;
volatile Carname otherCar = Name_Car1;
#endif

extern double  Angle_XZ, Angle_YZ,Angle_XY;

void PIT0_IRQHandler()
{
  PIT_Flag_Clear(PIT0);       
  static uint8 Time_count = 0;
  static uint8 Time_count2 = 0;
  g_time_count_Tms ++; 
  Time_count++;
  if(Time_count %4 == 0)
  {
    Time_count2++;
    Time_count = 0;
    PlusCount[0] = DMA_count_get(DMA_CH1);
    DMA_count_reset(DMA_CH1);
    Dir[0] = gpio_get(PTD13);
   
    PlusCount[1] = DMA_count_get(DMA_CH2);
    DMA_count_reset(DMA_CH2);
    Dir[1] = gpio_get(PTC10);
    
    PlusCount[2] = DMA_count_get(DMA_CH3);
    DMA_count_reset(DMA_CH3);
    Dir[2] = gpio_get(PTC9);
   
    PlusCount[3] = DMA_count_get(DMA_CH0);
    DMA_count_reset(DMA_CH0);
    Dir[3] = gpio_get(PTD10);

#if KAI
#else
    MotorPI();
#endif
  }
  if(Time_count2 %5 == 0)
  {
    Time_count2 = 0;
  }
}

void PIT2_IRQHandler()
{
    static uint8 count = 0;   
    PIT_Flag_Clear(PIT2);  
    g_Ltime_count ++; 
     //���жϱ�־λ
}

void UART0_IRQHandler()
{
    uart_rx_irq_dis(UART0);

    static int num = 0;
    char ch;
    
    
  /*
    uint8 cha[10];
    ch = uart_querystr (uratn, cha,  1000);
    uart_putstr  (UART0, "\n�㷢�͵�����Ϊ��");
    uart_putstr  (UART0, cha);
  */

    if(UART_S1_REG(UARTN[0]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
    {
        //�û���Ҫ�����������
              uart_getchar (UART0,&ch);      //���յ�һ������
              str[num++] = ch;
              if(ch == '*') num = 0;
              if(str[0] == '1' && str[1] == '0') goter = Car1;    //������Ϊ1�ų�
              if(str[0] == '2' && str[1] == '0') goter = Car2;   // ������Ϊ2�ų�
              if(str[0] == '3' && str[1] == '0') goter = Center;  //������Ϊ����
              if(str[2] == '1' && str[3] == '0') sender = From_Car1;   //������Ϊ1�ų�
              if(str[2] == '2' && str[3] == '0') sender = From_Car2;  //������Ϊ2�ų�
              if(str[2] == '3' && str[3] == '0') sender = From_Center;  //������Ϊ����
              if(num>=7 && myname == goter && sender == From_Center)  //���ķ��͸��Լ��� 
              {
                   if (mode == MODE_PTP || mode == MODE_MTM)
                  {
                     if(str[5] == '1') get_pos = GET_POS1;  
                     if(str[5] == '2') get_pos = GET_POS2;
                     if(str[5] == '3') get_pos = GET_POS3;
                     if(str[5] == '4') get_pos = GET_POS4;
                     if(str[5] == 'S') {mode = MODE_WAIT;get_pos = GET_POS_UNF;put_pos = PUT_POS_UNF;car_stop();}
                     if(str[6] == 'A') put_pos = PUT_POS1;  
                     if(str[6] == 'B') put_pos = PUT_POS2;
                     if(str[6] == 'C') put_pos = PUT_POS3;
                     if(str[6] == 'D') put_pos = PUT_POS4;
                     if(str[6] == 'S') {mode = MODE_WAIT;get_pos = GET_POS_UNF;put_pos = PUT_POS_UNF;car_stop();} 
                     PTP = 1;
                     if(str[5])
                     {
                        uart_putstr(UART0,CT);
                      uart_putstr(UART0,Car_name);
                     uart_putstr(UART0,"getpos is");
                     uart_putchar(UART0,str[5]);       //�ͷ��ͳ�ȥ
                     //uart_putchar (UART0, '\n');   //�ͷ��ͳ�ȥ
                     }
                       if(str[6])
                     {
                     uart_putstr(UART0,CT);
                     uart_putstr(UART0,Car_name);
                     uart_putstr(UART0,"putpos is");
                     uart_putchar(UART0,str[6]);       //�ͷ��ͳ�ȥq
                     uart_putchar (UART0, '\n');   //�ͷ��ͳ�ȥ
                     }
                  }

                      num = 0;
                      CH = str[4];
                      if(mode != MODE_PTP && mode != MODE_MTM)
                      {
                      uart_putstr(UART0,CT);
                      uart_putstr(UART0,Car_name);
                      uart_putchar(UART0,CH);       //�ͷ��ͳ�ȥ
                      uart_putchar (UART0, '\n');   //�ͷ��ͳ�ȥ
                      }
              }
              if(num>=5 && myname == goter && sender == otherCar)  //����һ�������͸��Լ��� 
              {
                  num = 0;
              }

    }

   // uart_querystr (UART0, &str, 3);
   // uart_putstr  (UART0, "\n�㷢�͵�����Ϊ��");
    //uart_putstr  (UART0, (uint8 *)str);
   /* if( str[0]== '2') //meseg is from center
    {
      CH = str[2];
      uart_putstr(UART0,Cname);
      uart_putstr  (UART0, "\n Your send is ");
      uart_putchar (UART0, CH);       //�ͷ��ͳ�ȥ
    }
    if(CH == 'S')
    {
      uart_putstr(UART0,Cname);
      uart_putstr  (UART0, "\n Car_Stop! ");
      car_stop();
    }
    */

    uart_rx_irq_en(UART0);
}
void PORTA_IRQHandler()
{
    uint8  n ;    //���ź�
    static uint32 time;
    n =24;
   
    if(PORTA_ISFR & (1 << n))          //PTA29�����ж�
    {
       PORTA_ISFR  |= (1 << n);        //д1���жϱ�־λ
      if(gpio_get(Echo)==1)
      {
        PIT_TCTRL1 &= ~PIT_TCTRL_TEN_MASK;
        PIT_LDVAL1 =0xFFFFFFFF;
        PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK;
      }
      if(gpio_get(Echo)==0)
      {
        time = (0xFFFFFFFF-PIT_CVAL1)/85;
        Distance = time*340/10000;;
        PIT_TCTRL1 &= ~PIT_TCTRL_TEN_MASK;
      }

    }
       if(Distance<=30 && Distance != 0)
       {
            BarFlag  = 1;
       }
}
void PORTB_IRQHandler()
{
    uint8  n ;    //���ź�
    n =20;
    if(PORTB_ISFR & (1 << n))         //PTB20�����ж�
    {
        PORTB_ISFR  |= (1 << n);
        disable_irq(PORTB_IRQn);
        PORTB_ISFR = 1 <<  PT18;            //���PCLK��־λ
        DMA_IRQ_CLEAN(DMA_CH4);
        DMA_EN(DMA_CH4);//ʹ��ͨ��CHn Ӳ������      (�������ܼ�������DMA����)
        DMA_IRQ_EN(DMA_CH4) ;  //����DMAͨ������
        DMA_DADDR(DMA_CH4) =(uint32)ov7725_eagle_img_buff;   //���ݴ洢��ַ�仯
        PORTB_ISFR = 1 <<  PT18;            //���PCLK��־λ
    }

}
//���벶׽�ж�
void FTM2_IRQHandler()
{
    uint8 s = FTM2_STATUS;             //��ȡ��׽�ͱȽ�״̬  All CHnF bits can be checked using only one read of STATUS.
    uint8 CHn;

    FTM2_STATUS = 0x00;             //���жϱ�־λ

    CHn = 0;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_DIS(FTM1, CHn);     //��ֹ���벶׽�ж�
        /*     �û�����       */
         

        /*********************/
        //FTM_IRQ_EN(FTM1, CHn); //�������벶׽�ж�

    }

    /* ������� n=1 ��ģ�棬����ģ������� */
    CHn = 1;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_EN(FTM1, CHn); //�������벶׽�ж�
        /*     �û�����       */
        

        /*********************/
        //�����������￪�����벶׽�ж�
        //FTM_IRQ_EN(FTM1, CHn); //�������벶׽�ж�
    }
}

/**** DMA4�жϹ�ͨ������ֹ�жϣ�����ж�****/
void DMA4_IRQHandler()
{
  DMA_IRQ_CLEAN(DMA_CH4);
  DMA_IRQ_DIS(DMA_CH4);
  DMA_DIS(DMA_CH4); 
  img_to2z(CData,imgbuff,CAMERA_SIZE);//��ѹͼ��
  //removeNoise(Data); //ȥ�봦��
//  imaged(img);
//  if(boma1||boma2|boma4)
  memcpy(DataCopy,CData,sizeof(CData));
  PORTB_ISFR  |= (1 << 20);
  enable_irq(PORTB_IRQn);
  //gpio_turn (PTA17);
}