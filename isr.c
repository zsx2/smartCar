/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：isr.c
 * 描述         ：中断处理例程
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/


#include "common.h"
#include "include.h"
extern uint8   *ov7725_eagle_img_buff;
extern uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
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
//***摄像头采集和处理


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
     //清中断标志位
}

void UART0_IRQHandler()
{
    uart_rx_irq_dis(UART0);

    static int num = 0;
    char ch;
    
    
  /*
    uint8 cha[10];
    ch = uart_querystr (uratn, cha,  1000);
    uart_putstr  (UART0, "\n你发送的数据为：");
    uart_putstr  (UART0, cha);
  */

    if(UART_S1_REG(UARTN[0]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
        //用户需要处理接收数据
              uart_getchar (UART0,&ch);      //接收到一个数据
              str[num++] = ch;
              if(ch == '*') num = 0;
              if(str[0] == '1' && str[1] == '0') goter = Car1;    //接收者为1号车
              if(str[0] == '2' && str[1] == '0') goter = Car2;   // 接收者为2号车
              if(str[0] == '3' && str[1] == '0') goter = Center;  //接送者为中心
              if(str[2] == '1' && str[3] == '0') sender = From_Car1;   //发送者为1号车
              if(str[2] == '2' && str[3] == '0') sender = From_Car2;  //发送者为2号车
              if(str[2] == '3' && str[3] == '0') sender = From_Center;  //发送者为中心
              if(num>=7 && myname == goter && sender == From_Center)  //中心发送给自己的 
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
                     uart_putchar(UART0,str[5]);       //就发送出去
                     //uart_putchar (UART0, '\n');   //就发送出去
                     }
                       if(str[6])
                     {
                     uart_putstr(UART0,CT);
                     uart_putstr(UART0,Car_name);
                     uart_putstr(UART0,"putpos is");
                     uart_putchar(UART0,str[6]);       //就发送出去q
                     uart_putchar (UART0, '\n');   //就发送出去
                     }
                  }

                      num = 0;
                      CH = str[4];
                      if(mode != MODE_PTP && mode != MODE_MTM)
                      {
                      uart_putstr(UART0,CT);
                      uart_putstr(UART0,Car_name);
                      uart_putchar(UART0,CH);       //就发送出去
                      uart_putchar (UART0, '\n');   //就发送出去
                      }
              }
              if(num>=5 && myname == goter && sender == otherCar)  //另外一辆车发送给自己的 
              {
                  num = 0;
              }

    }

   // uart_querystr (UART0, &str, 3);
   // uart_putstr  (UART0, "\n你发送的数据为：");
    //uart_putstr  (UART0, (uint8 *)str);
   /* if( str[0]== '2') //meseg is from center
    {
      CH = str[2];
      uart_putstr(UART0,Cname);
      uart_putstr  (UART0, "\n Your send is ");
      uart_putchar (UART0, CH);       //就发送出去
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
    uint8  n ;    //引脚号
    static uint32 time;
    n =24;
   
    if(PORTA_ISFR & (1 << n))          //PTA29触发中断
    {
       PORTA_ISFR  |= (1 << n);        //写1清中断标志位
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
    uint8  n ;    //引脚号
    n =20;
    if(PORTB_ISFR & (1 << n))         //PTB20触发中断
    {
        PORTB_ISFR  |= (1 << n);
        disable_irq(PORTB_IRQn);
        PORTB_ISFR = 1 <<  PT18;            //清空PCLK标志位
        DMA_IRQ_CLEAN(DMA_CH4);
        DMA_EN(DMA_CH4);//使能通道CHn 硬件请求      (这样才能继续触发DMA传输)
        DMA_IRQ_EN(DMA_CH4) ;  //允许DMA通道传输
        DMA_DADDR(DMA_CH4) =(uint32)ov7725_eagle_img_buff;   //数据存储地址变化
        PORTB_ISFR = 1 <<  PT18;            //清空PCLK标志位
    }

}
//输入捕捉中断
void FTM2_IRQHandler()
{
    uint8 s = FTM2_STATUS;             //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.
    uint8 CHn;

    FTM2_STATUS = 0x00;             //清中断标志位

    CHn = 0;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_DIS(FTM1, CHn);     //禁止输入捕捉中断
        /*     用户任务       */
         

        /*********************/
        //FTM_IRQ_EN(FTM1, CHn); //开启输入捕捉中断

    }

    /* 这里添加 n=1 的模版，根据模版来添加 */
    CHn = 1;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_EN(FTM1, CHn); //开启输入捕捉中断
        /*     用户任务       */
        

        /*********************/
        //不建议在这里开启输入捕捉中断
        //FTM_IRQ_EN(FTM1, CHn); //开启输入捕捉中断
    }
}

/**** DMA4中断关通道，禁止中断，清除中断****/
void DMA4_IRQHandler()
{
  DMA_IRQ_CLEAN(DMA_CH4);
  DMA_IRQ_DIS(DMA_CH4);
  DMA_DIS(DMA_CH4); 
  img_to2z(CData,imgbuff,CAMERA_SIZE);//解压图像
  //removeNoise(Data); //去噪处理
//  imaged(img);
//  if(boma1||boma2|boma4)
  memcpy(DataCopy,CData,sizeof(CData));
  PORTB_ISFR  |= (1 << 20);
  enable_irq(PORTB_IRQn);
  //gpio_turn (PTA17);
}