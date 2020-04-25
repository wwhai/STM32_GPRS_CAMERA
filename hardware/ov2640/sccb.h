#ifndef __SCCB_H__
#define __SCCB_H__

#include "sys.h"

#define SCCB_SDA_IN()  {GPIOC->CRH&=0X00FFFFFFFFFF;GPIOC->CRH|=0X0800000000000;}
#define SCCB_SDA_OUT() {GPIOC->CRH&=0X00FFFFFFFFFF;GPIOC->CRH|=0X0300000000000;}
 
#define SCCB_SCL    		PCout(9)	 	//SCL
#define SCCB_SDA    		PCout(11) 		//SDA	 

#define SCCB_READ_SDA    	PCin(11)
#define SCCB_ID   			0X60

void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);

u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);

#endif

