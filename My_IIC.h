#ifndef MY_I2C_H_
#define MY_I2C_H_

#define I2C_SCL_PORT  PTA12
#define I2C_SDA_PORT  PTA13
#define I2C_SCL       PTXn_T(I2C_SCL_PORT,OUT)
#define I2C_SDA_I     PTXn_T(I2C_SDA_PORT,IN)
#define I2C_SDA_O     PTXn_T(I2C_SDA_PORT,OUT)
//����SDA�������
#define I2C_SDA_OUT() PTXn_T(I2C_SDA_PORT,DDR)=1
#define I2C_SDA_IN()  PTXn_T(I2C_SDA_PORT,DDR)=0
#define I2C_DELAY()   systick_delay_us(5)

#define	Slave_Address   0x1a

void My_IIC_Port_Init();

void I2C_Delay(uint16);

void I2C_WriteReg(uint8 dev_addr,uint8 reg_addr , uint8 data);

uint8 I2C_ReadByte(uint8 dev_addr,uint8 reg_addr);//��һ���ֽڵ�����

int16 I2C_ReadWord(uint8 dev_addr,uint8 reg_addr);//�������ֽڵ�����

void I2C_ReadGryo(uint8 dev_addr,uint8 reg_addr,int16 *x,int16 *y);

void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
void I2C_SendByte(uint8);
uint8 I2C_ReceiveByte(void);

void Init_QMC5883();
void Multiple_Read_QMC5883(double* valueXY,double* valueXZ,double *valueYZ );



#endif