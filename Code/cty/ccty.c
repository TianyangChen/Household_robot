#include <STC89Cxx.h>

#define uchar unsigned char      
/*****************************************************************************
* TH0和TL0是计数器0的高8位和低8位计数器，计算办法:TL0=(65536-C)%256;         *
* TH0=(65536-C)/256,其中C为所要计数的次数即多长时间产生一次中断；TMOD是计数器*
* 工作模式选择，0X01表示选用模式1,它有16位计数器，最大计数脉冲为65536,最长时 *
* 间为1ms*65536=65.536ms                                                     *
******************************************************************************/
                 

void init_sys(void);            /*系统初始化函数*/


unsigned char ZKB1,ZKB2,ZKB3;
unsigned char timer;

void main (void)
{
init_sys();
  ZKB1=20;            /*占空比初始值设定*/
  ZKB2=50;
  ZKB3=80;            /*占空比初始值设定*/
  while(1)
  {
    if (timer>100)
    {
      timer=0;
      /* code */
    }
  }
}

/******************************************************
*函数功能：对系统进行初始化，包括定时器初始化和变量初始化*/
void init_sys(void)            /*系统初始化函数*/
{
  /*定时器初始化*/
  TMOD=0x01;
  TH0=0xff;
  TL0=0xf6;
  TR0=1;
  ET0=1;
  EA=1;
}


//延时

/*中断函数*/
void timer0(void) interrupt 1 
{
  TH0=0xff;                                    /*恢复定时器初始值*/
  TL0=0xf6;
  if (timer<ZKB1)
    P11=1;
  else
    P11=0;

  if (timer<ZKB2)
    P12=1;
  else
    P12=0;

  if (timer<ZKB3)
    P13=1;
  else
    P13=0;
  
  timer++;
}

