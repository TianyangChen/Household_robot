#include <STC89Cxx.h>

#define uchar unsigned char      
/*****************************************************************************
* TH0��TL0�Ǽ�����0�ĸ�8λ�͵�8λ������������취:TL0=(65536-C)%256;         *
* TH0=(65536-C)/256,����CΪ��Ҫ�����Ĵ������೤ʱ�����һ���жϣ�TMOD�Ǽ�����*
* ����ģʽѡ��0X01��ʾѡ��ģʽ1,����16λ������������������Ϊ65536,�ʱ *
* ��Ϊ1ms*65536=65.536ms                                                     *
******************************************************************************/
                 

void init_sys(void);            /*ϵͳ��ʼ������*/


unsigned char ZKB1,ZKB2,ZKB3;
unsigned char timer;

void main (void)
{
init_sys();
  ZKB1=20;            /*ռ�ձȳ�ʼֵ�趨*/
  ZKB2=50;
  ZKB3=80;            /*ռ�ձȳ�ʼֵ�趨*/
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
*�������ܣ���ϵͳ���г�ʼ����������ʱ����ʼ���ͱ�����ʼ��*/
void init_sys(void)            /*ϵͳ��ʼ������*/
{
  /*��ʱ����ʼ��*/
  TMOD=0x01;
  TH0=0xff;
  TL0=0xf6;
  TR0=1;
  ET0=1;
  EA=1;
}


//��ʱ

/*�жϺ���*/
void timer0(void) interrupt 1 
{
  TH0=0xff;                                    /*�ָ���ʱ����ʼֵ*/
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
