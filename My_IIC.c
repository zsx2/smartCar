#include "include.h"
#include "My_IIC.h"

extern uint8 BUF[8];
void My_IIC_Port_Init()
{
  gpio_init(I2C_SCL_PORT,GPO,1);                     //初始化SCL管脚
  gpio_init(I2C_SDA_PORT,GPO,1);                     //初始化SDA管脚
  port_init_NoALT (I2C_SCL_PORT, PULLUP );
  port_init_NoALT (I2C_SDA_PORT, PULLUP );
}
/*
 * I2C_WriteReg
 * 写I2C设备寄存器
 */
void I2C_WriteReg(uint8 dev_addr,uint8 reg_addr , uint8 data)
{		
  I2C_Start();
  I2C_SendByte(dev_addr<<1);         
  I2C_SendByte(reg_addr );   
  I2C_SendByte(data);   
  I2C_Stop(); 
}
//读寄存器
uint8 I2C_ReadByte(uint8 dev_addr,uint8 reg_addr)
{
  uint8 data;
  I2C_Start();
  I2C_SendByte( dev_addr<<1);//从机地址+写信号
  I2C_SendByte( reg_addr );//寄存器地址
  I2C_Start();
  I2C_SendByte((dev_addr<<1)+1);//从机地址+读信号
  data= I2C_ReceiveByte();
  I2C_NoAck();
  I2C_Stop();
  return data;
}

//读寄存器
int16 I2C_ReadWord(uint8 dev_addr,uint8 reg_addr)
{
  uint8 h,l;
  I2C_Start();
  I2C_SendByte( dev_addr<<1); 
  I2C_SendByte( reg_addr);
  I2C_Start();
  I2C_SendByte((dev_addr<<1)+1);
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_NoAck();
  I2C_Stop();
  return (h<<8)+l;
}

//这是连续读了两个轴的数据
void I2C_ReadGryo(uint8 dev_addr,uint8 reg_addr,int16 *x,int16 *y)
{
  char h,l;
  I2C_Start();
  I2C_SendByte( dev_addr<<1); 
  I2C_SendByte( reg_addr); 
  I2C_Start();	
  I2C_SendByte((dev_addr<<1)+1); 
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_Ack();
  *x=(h<<8)+l;
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_Ack();
  h= I2C_ReceiveByte();
  I2C_Ack();
  l= I2C_ReceiveByte();
  I2C_NoAck();
  *y=(h<<8)+l;
  I2C_Stop();
}
/*
 * I2C_Start
 * I2C起始信号，内部调用
 */
void I2C_Start(void)
{
  I2C_SDA_OUT();
  I2C_DELAY();
  I2C_DELAY();
  I2C_SDA_O=1; 
  I2C_SCL=1;
  I2C_DELAY();
  I2C_DELAY();
  I2C_SDA_O=0; 
  I2C_DELAY();
  I2C_DELAY();
  I2C_SCL=0;
  I2C_DELAY();
  I2C_DELAY();
}

/*
 * I2C_Stop
 * I2C停止信号，内部调用
 */
 void I2C_Stop(void)
{ 
    I2C_SDA_O=0;
    I2C_SCL=0; 
  I2C_DELAY();
  I2C_DELAY();
    I2C_SCL=1;
  I2C_DELAY();
  I2C_DELAY();
    I2C_SDA_O=1;
  I2C_DELAY();
  I2C_DELAY();
    I2C_SCL=0;
}

/*
 * I2C_Stop
 * I2C应答信号，内部调用
 */
void I2C_Ack(void)
{
  I2C_SCL=0;
  I2C_DELAY();
  
  I2C_SDA_O=0;
  I2C_DELAY();
  
  I2C_SCL=1;
  I2C_DELAY();
  
  I2C_SCL=0;
  I2C_DELAY();
}

/*
 * I2C_NoAck
 * I2C无应答信号，内部调用
 */
 void I2C_NoAck(void)
{
  I2C_SCL=0;
  I2C_DELAY();
  I2C_SDA_O=1;
  I2C_DELAY();
  I2C_SCL=1;
  I2C_DELAY();
  I2C_SCL=0;
  I2C_DELAY();
}


/*
 * I2C_SendByte
 * I2C发送数据，内部调用
 */
void I2C_SendByte(uint8 data)
{
  uint8 i=8;
  while(i--)
  {
    I2C_SCL=0;
    if(data&0x80)
      I2C_SDA_O=1; 
    else 
      I2C_SDA_O=0;
    data<<=1;
    I2C_DELAY();
    I2C_SCL=1;
    I2C_DELAY();
  }
  I2C_SCL=0;
  I2C_DELAY();
  I2C_SDA_I=1;
  I2C_DELAY();
  I2C_SCL=1; 
  I2C_DELAY();
  I2C_SCL=0;
}

/*
 * I2C_SendByte
 * I2C接收数据，内部调用
 */
 uint8 I2C_ReceiveByte(void)  
{ 
  uint8 i=8;
  uint8 ReceiveByte=0;
  
  I2C_SDA_O=1;	
  I2C_SDA_IN();	
  
  while(i--)
  {
    ReceiveByte<<=1;      
    I2C_SCL=0;
    I2C_DELAY();
    I2C_SCL=1;
    I2C_DELAY();	
    if(I2C_SDA_I)
    {
      ReceiveByte|=0x01;
    }

  }
  I2C_SDA_OUT();
  I2C_SCL=0;
  return ReceiveByte;
}

/*
 * I2C_SendByte
 * I2C延时函数，内部调用
 */
void I2C_Delay(uint16 i)
{	
  while(i) 
    i--;
}

void QMC5883_SendACK(uint8 ack)
{
    if(ack)  I2C_SDA_O=1; 
    else    I2C_SDA_O=0; 
    I2C_SCL=1;
    I2C_DELAY();
    I2C_SCL=0;
    I2C_DELAY();
}
uint8 QMC5883_RecvByte()
{
    uint8 i=8;
  uint8 ReceiveByte=0;
  
  I2C_SDA_O=1;	
  I2C_SDA_IN();	
  
  while(i--)
  {
    ReceiveByte<<=1;      
    I2C_SCL=0;
    I2C_DELAY();
    I2C_SCL=1;
    I2C_DELAY();	
    if(I2C_SDA_I)
    {
      ReceiveByte|=0x01;
    }

  }
  I2C_SDA_OUT();
  I2C_SCL=0;
  return ReceiveByte;
}
void QMC5883_SendByte(uint8 data)
{
  uint8 i=8;
  while(i--)
  {
    I2C_SCL=0;
    if(data&0x80)
      I2C_SDA_O=1; 
    else 
      I2C_SDA_O=0;
    data<<=1;
    I2C_DELAY();
    I2C_SCL=1;
    I2C_DELAY();
  }
  I2C_SCL=0;
  I2C_DELAY();
  I2C_SDA_I=1;
  I2C_DELAY();
  I2C_SCL=1; 
  I2C_DELAY();
  I2C_SCL=0;
}

void QMC5883_Stop()
{
  /*
      SDA = 0;                    //à-μíêy?Y??
    SCL = 1;                    //à-??ê±?ó??
    Delay_5us();                 //?óê±
    SDA = 1;                    //2úéúé?éy??
    Delay_5us();                 //?óê±
  */
    
  I2C_SDA_O=0;
  I2C_SCL=0; 
  I2C_DELAY();
  I2C_DELAY();
  I2C_SCL=1;
  I2C_DELAY();
  I2C_DELAY();
  I2C_SDA_O=1;
  I2C_DELAY();
  I2C_DELAY();
  I2C_SCL=0;
}
void QMC5883_Start()
{
  I2C_SDA_OUT();
  I2C_DELAY();
  I2C_DELAY();
  I2C_SDA_O=1; 
  I2C_SCL=1;
  I2C_DELAY();
  I2C_DELAY();
  I2C_SDA_O=0; 
  I2C_DELAY();
  I2C_DELAY();
  I2C_SCL=0;
  I2C_DELAY();
  I2C_DELAY();
}

void Single_Write_QMC5883(uint8 REG_Address,uint8 REG_data)
{
    QMC5883_Start();                  //?eê?D?o?
    QMC5883_SendByte(Slave_Address);   //·￠?íéè±?μ??·+D′D?o?
    QMC5883_SendByte(REG_Address);    //?ú2???′??÷μ??·￡???2????D??pdf 
    QMC5883_SendByte(REG_data);       //?ú2???′??÷êy?Y￡???2????D??pdf
    QMC5883_Stop();                   //·￠?íí￡?1D?o?
}

void Multiple_Read_QMC5883(double* valueXY,double* valueXZ,double *valueYZ )
{   
    uint8 i;
    int X,Y,Z;
    QMC5883_Start();                          //?eê?D?o?
    QMC5883_SendByte(Slave_Address);          //·￠?íéè±?μ??·+D′D?o?
    QMC5883_SendByte(0x00);                   //·￠?í′?′￠μ￥?aμ??·￡?′ó0x00?aê?	
    QMC5883_Start();                          //?eê?D?o?
    QMC5883_SendByte(Slave_Address+1);        //·￠?íéè±?μ??·+?áD?o?
	 for (i=0; i<6; i++)                      //á?D??áè?6??μ??·êy?Y￡?′?′￠?DBUF
    {
        BUF[i] = QMC5883_RecvByte();          //BUF[0]′?′￠êy?Y
        if (i == 5)
        {
           QMC5883_SendACK(1);                //×?oóò???êy?YDèòa??·?ó|′eNOACK
        }
        else
        {
          QMC5883_SendACK(0);                 //ó|′eACK
       }
   }
    QMC5883_Stop();                           //í￡?1D?o?
    systick_delay_ms(5);
     X=BUF[1] << 8 | BUF[0]; //Combine MSB and LSB of X Data output register  ×???óDD§??
     Y=BUF[3] << 8 | BUF[2]; //Combine MSB and LSB of Y Data output register
     Z=BUF[5] << 8 | BUF[4]; 
      *valueXY= atan2((double)Y,(double)X) * (180 / 3.14159265) + 180; //????XY???????è
     //       *valueXY=*valueXY*10;
     // *valueXZ= atan2((double)Z,(double)X) * (180 / 3.14159265) + 180; //????XZ???????è 
              // *valueXZ*=10;
      //*valueYZ= atan2((double)Z,(double)Y) * (180 / 3.14159265) + 180; //????YZ???????è
          // *valueYZ*=10;
    
}

//3?ê??ˉQMC5883￡??ù?YDèòa??2???pdf??DDDT??****
void Init_QMC5883()
{

	Single_Write_QMC5883(0x09,0x0d);  //??????′??÷????
	Single_Write_QMC5883(0x0b,0x01);  //éè????3yê±????′??÷
	Single_Write_QMC5883(0x20,0x40);  //
	Single_Write_QMC5883(0x21,0x01);  //	
}