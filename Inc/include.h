/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��include.h
 * ����         ������ģ��ͷ�ļ�
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#ifndef __INCLUDE_H__
#define __INCLUDE_H__

typedef enum
{
    MODE_WAIT= 0,
    MODE_AUTO,
    MODE_HAND,
    MODE_LEARNING,
    MODE_PTP,
    MODE_MTM,
    MODE_PID
} MODE_STATUS_e;

typedef enum
{
  GET_POS_UNF = 0,
  GET_POS1 ,
  GET_POS2 ,
  GET_POS3 ,
  GET_POS4 ,
} PARCEL_GET_POS_e;

typedef enum
{
  PUT_POS_UNF = 0,
  PUT_POS1 ,
  PUT_POS2 ,
  PUT_POS3 ,
  PUT_POS4 ,
} PARCEL_PUT_POS_e;

typedef enum
{
  CAR_POS_UNF = 0,
  CAR_POS1 ,
  CAR_POS2,
  CAR_POS3,
  CAR_POS4,
} CAR_POS_e;

typedef enum
{
  None=0,
  Car1,
  Car2,
  Center
} MESG_SendTo;

typedef enum
{
  None1 = 0,
  Name_Car1,
  Name_Car2,
} Carname;

typedef enum
{
  None2 = 0,
  From_Car1,
  From_Car2,
  From_Center,
} Mesg_Sender;

#define CAR1 1   //1Ϊ1�ų�  2Ϊ2�ų�
#define KAI 0

#define Trig PTC0
#define Echo PTA24




#include  "common.h"
#include "isr.h"
#include "Init.h"
#include "LQ_OLED.h"
#include "VCAN_SCCB.h"
#include "VCAN_camera.h"
#include "VCAN_computer.h"
#include "VCAN_OV7725_Eagle.h"
#include "control.h"
#include "debug.h"
#include "image_deal.h"
#include "key.h"

#include "My_IIC.h"
//#include "QMC5883.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO�ڲ���
#include  "MK60_uart.h"     //����
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //�͹��Ķ�ʱ��(��ʱ)
#include  "MK60_i2c.h"      //I2C
#include  "MK60_spi.h"      //SPI
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_rtc.h"      //RTC
#include  "MK60_adc.h"      //ADC
#include  "MK60_dac.h"      //DAC
#include  "MK60_dma.h"      //DMA
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_can.h"      //CAN
#include  "MK60_sdhc.h"     //SDHC
#include  "ff.h"                //FatFs





#endif  //__INCLUDE_H__

