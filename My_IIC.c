#include "include.h"
#include "My_IIC.h"

extern uint8 BUF[8];
void My_IIC_Port_Init()
{
  gpio_init(I2C_SCL_PORT,GPO,1);                     //��ʼ��SCL�ܽ�
  gpio_init(I2C_SDA_PORT,GPO,1);                     //��ʼ��SDA�ܽ�
  port_init_NoALT (I2C_SCL_PORT, PULLUP );
  port_init_NoALT (I2C_SDA_PORT, PULLUP );
}
/*
 * I2C_WriteReg
 * дI2C�豸�Ĵ���
 */
void I2C_WriteReg(uint8 dev_addr,uint8 reg_addr , uint8 data)
{		
  I2C_Start();
  I2C_SendByte(dev_addr<<1);         
  I2C_SendByte(reg_addr );   
  I2C_SendByte(data);   
  I2C_Stop(); 
}
//���Ĵ���
uint8 I2C_ReadByte(uint8 dev_addr,uint8 reg_addr)
{
  uint8 data;
  I2C_Start();
  I2C_SendByte( dev_addr<<1);//�ӻ���ַ+д�ź�
  I2C_SendByte( reg_addr );//�Ĵ�����ַ
  I2C_Start();
  I2C_SendByte((dev_addr<<1)+1);//�ӻ���ַ+���ź�
  data= I2C_ReceiveByte();
  I2C_NoAck();
  I2C_Stop();
  return data;
}

//���Ĵ���
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

//�����������������������
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
 * I2C��ʼ�źţ��ڲ�����
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
 * I2Cֹͣ�źţ��ڲ�����
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
 * I2CӦ���źţ��ڲ�����
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
 * I2C��Ӧ���źţ��ڲ�����
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
 * I2C�������ݣ��ڲ�����
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
 * I2C�������ݣ��ڲ�����
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
 * I2C��ʱ�������ڲ�����
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
      SDA = 0;                    //��-�̨���y?Y??
    SCL = 1;                    //��-??����?��??
    Delay_5us();                 //?������
    SDA = 1;                    //2��������?��y??
    Delay_5us();                 //?������
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
    QMC5883_Start();                  //?e��?D?o?
    QMC5883_SendByte(Slave_Address);   //����?��������?��??��+D��D?o?
    QMC5883_SendByte(REG_Address);    //?��2???��??�¦�??����???2????D??pdf 
    QMC5883_SendByte(REG_data);       //?��2???��??�¨�y?Y��???2????D??pdf
    QMC5883_Stop();                   //����?������?1D?o?
}

void Multiple_Read_QMC5883(double* valueXY,double* valueXZ,double *valueYZ )
{   
    uint8 i;
    int X,Y,Z;
    QMC5883_Start();                          //?e��?D?o?
    QMC5883_SendByte(Slave_Address);          //����?��������?��??��+D��D?o?
    QMC5883_SendByte(0x00);                   //����?����?���̣�?a��??����?�䨮0x00?a��?	
    QMC5883_Start();                          //?e��?D?o?
    QMC5883_SendByte(Slave_Address+1);        //����?��������?��??��+?��D?o?
	 for (i=0; i<6; i++)                      //��?D??����?6??��??����y?Y��?��?���?DBUF
    {
        BUF[i] = QMC5883_RecvByte();          //BUF[0]��?��騺y?Y
        if (i == 5)
        {
           QMC5883_SendACK(1);                //��?o����???��y?YD����a??��?��|��eNOACK
        }
        else
        {
          QMC5883_SendACK(0);                 //��|��eACK
       }
   }
    QMC5883_Stop();                           //����?1D?o?
    systick_delay_ms(5);
     X=BUF[1] << 8 | BUF[0]; //Combine MSB and LSB of X Data output register  ��???��DD��??
     Y=BUF[3] << 8 | BUF[2]; //Combine MSB and LSB of Y Data output register
     Z=BUF[5] << 8 | BUF[4]; 
      *valueXY= atan2((double)Y,(double)X) * (180 / 3.14159265) + 180; //????XY???????��
     //       *valueXY=*valueXY*10;
     // *valueXZ= atan2((double)Z,(double)X) * (180 / 3.14159265) + 180; //????XZ???????�� 
              // *valueXZ*=10;
      //*valueYZ= atan2((double)Z,(double)Y) * (180 / 3.14159265) + 180; //????YZ???????��
          // *valueYZ*=10;
    
}

//3?��??��QMC5883��??��?YD����a??2???pdf??DDDT??****
void Init_QMC5883()
{

	Single_Write_QMC5883(0x09,0x0d);  //??????��??��????
	Single_Write_QMC5883(0x0b,0x01);  //����????3y����????��??��
	Single_Write_QMC5883(0x20,0x40);  //
	Single_Write_QMC5883(0x21,0x01);  //	
}