

/* 包含头文件 -----------------------------------------------------------------*/
#include "include.h"
//#include "My_IIC.h"
//#include "time.h"
#include "QMC5883.h"
#include "math.h"
//#include "matrix.h"

extern uint8 QMC5883_print;
//extern float Angle;
//extern float angle[10];
/* 私有宏定义 -----------------------------------------------------------------*/
#define MAT_N  10
#define QMC5883_ADDR               0x1a
#define fabs(x)                    (x < 0 ? -x : x)
/* 私有变量 -------------------------------------------------------------------*/
int16 MagnetRawAd[3];
int16 X_Buff[6],Y_Buff[6],
Z_Buff[6],CalibrateCnt;
/* 私有函数 -------------------------------------------------------------------*/
/******************************************************************************
* @函数名称：void MPU6050_Init(void)
* @函数描述：MPU6050传感器初始化
* @输入参数：None
* @输出参数：None
*******************************************************************************/

uint8 QMC5883_ReadReg(uint8 Reg)
{
  uint8 RegVal = 0;
  //I2C_ReadByte(&RegVal,1,Reg,QMC5883_ADDR);
  i2c_read_reg(I2C0, QMC5883_ADDR, Reg);
  //RegVal=I2C_ReadByte( QMC5883_ADDR,Reg);//读一个字节的数据
  return RegVal;
}
/******************************************************************************
* @函数名称：void MPU6050_Init(void)
* @函数描述：MPU6050传感器初始化
* @输入参数：None
* @输出参数：None
*******************************************************************************/
void QMC5883_WriteReg(uint8 Val,uint8 Reg)
{
  //I2C_WriteByte(Val,Reg, QMC5883_ADDR);
  i2c_write_reg(I2C0, QMC5883_ADDR,Reg,Val);
  //I2C_WriteReg(QMC5883_ADDR,Reg ,Val);
}
/******************************************************************************
* @函数名称：void QMC5883_Init(void)
* @函数描述：程序入口
* @输入参数：None
* @输出参数：None
*******************************************************************************/

uint8 QMC5883_InitConfig(void)
{
  uint8 Temp=0;
  QMC5883_WriteReg(0x01, 0x0A);//RESET
  QMC5883_WriteReg(0x01, 0x0B);    
  
  //QMC5883_WriteReg(0x01, 0x21);
  QMC5883_WriteReg(0x0D, 0x09);//OSR=512,RNG=+/-2G,ODR=200Hz,MODE= continuous
  //while(!TIME_CheckTimeMs(TIME_DELAY,2));///DELAY 2ms
  DELAY_MS(2);
  Temp = QMC5883_ReadReg(0x09);
  while(Temp != 0x1a ) 
    ;               //configure faill	
  return 0;
}
/******************************************************************************
* @函数名称：void QMC5883_GetData(float *Magnet)
* @函数描述：获取地磁数据
* @输入参数：地磁buff
* @输出参数：None
*******************************************************************************/
//extern uint8 QMC5883_temp;
void QMC5883_GetData(float *angle)
{
  uint8 Buff[6]={0},i=0;
  float Magnet[3]={0};
  uint8 Temp;
  
   //=i2c_read_reg(I2C1, QMC5883_ADDR, 0x06);
  Temp=QMC5883_ReadReg(0x06);
 // QMC5883_temp=Temp;
  if (Temp&0x06)
  {
    //LOG("output data OVL or ERROR!")
    	return;    //reg 0x06   bit[1]=1 / bit[2]=1
  }
  //else if (Temp&0x00)
  else if(~(Temp|0xfe)==1)
  {
    //LOG("output data not ready!")
    return;//
  }
  
  //I2C_ReadByte(Buff,6,0x00,QMC5883_ADDR);
  for(i=0;i<6;i++)
    Buff[i]=i2c_read_reg(I2C1, QMC5883_ADDR, 0x00+i);
  MagnetRawAd[0] = ((int16)Buff[1] << 8) | Buff[0];
  MagnetRawAd[1] = ((int16)Buff[3] << 8) | Buff[2];
  MagnetRawAd[2] = ((int16)Buff[5] << 8) | Buff[4];
  
  for(i=0;i<3;i++)
  {
    Magnet[i] = (float)MagnetRawAd[i] / 120.f;//单位uT 
  }
  *angle= atan2((double)Magnet[1],(double)Magnet[0])*(180/3.14159265)+180;
 

}

void QMC5883_parameter_dispaly(void)
{
//    Dis_Float(  0,2,fabs(angle[0]),1);
//    Dis_Float(  0,4,fabs(angle[1]),1);
//    Dis_Float(  0,6,fabs(angle[2]),1);  
    //Dis_Float(  8,2,fabs(angle[1]),1);
 //   Dis_Float( 8,4,fabs(angle[0]),1);


}

/* 文件结束 --------------------------------------------------------------------*/
