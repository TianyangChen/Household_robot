#include <STC89Cxx.h>
	
#define uchar  unsigned char 
#define uint   unsigned int


unsigned char buf,num,i,temp;
bit check,sending,send_flag;
unsigned char pdata pack[241];

void send(unsigned char d)      //发送一个字节的数据，形参d即为待发送数据。
{
 SBUF=d; //将数据写入到串口缓冲
 sending=1;  //设置发送标志
 while(sending); //等待发送完毕
}

void main (void)  
{ 
  unsigned char j,k; 
  RCAP2H=0xff;  
  RCAP2L=0xfd;
//串口相关设定
  SCON=0x50;     //串口工作在模式1
  ES=1;         //串行中断允许
  T2CON=0x34;  //TF2 EXF2 RCLK TCLK EXEN2 TR2 C/T2 CP/RL2    启动定时器2
  
  EA=1;         //允许中断  
  while(1){
    if (send_flag)
    {
      for (i = 0; i < 22; i++)
      {
        if (pack[i]==0xFA && pack[i+1]>=0xA0 && pack[i+1]<=0xF9)
        {
          temp=i;break;
        }
      }
      for (i = 0; i < 10; i++)
      {
        j=9*i;
        k=22*i+1;
          pack[j]=pack[temp+k];
          if (pack[temp+k+4]>0x18)
          {
            pack[j+1]=0;pack[j+2]=0;
          }else{
            pack[j+1]=pack[temp+k+3];pack[j+2]=pack[temp+k+4];
          }

          if (pack[temp+k+8]>0x18)
          {
            pack[j+3]=0;pack[j+4]=0;
          }else{
            pack[j+3]=pack[temp+k+7];pack[j+4]=pack[temp+k+8];
          }

          if (pack[temp+k+12]>0x18)
          {
            pack[j+5]=0;pack[j+6]=0;
          }else{
            pack[j+5]=pack[temp+k+11];pack[j+6]=pack[temp+k+12];
          }

          if (pack[temp+k+16]>0x18)
          {
            pack[j+7]=0;pack[j+8]=0;
          }else{
            pack[j+7]=pack[temp+k+15];pack[j+8]=pack[temp+k+16];
          }
      }
      for (i = 0; i < 90; i++)
      {
        send(pack[i]);
      }
      send_flag=0;
      REN=1;
    }
  }
}  


      
  

      // if (zeros1!=4)
      // {
      //   for (i = 0; i < 9; i++)
      //   {
      //     send(pack[i]);
      //   }
      // }
      //       if (zeros2!=4)
      // {
      //   for (i = 9; i < 18; i++)
      //   {
      //     send(pack[i]);
      //   }
      // }
      //       if (zeros3!=4)
      // {
      //   for (i = 18; i < 27; i++)
      //   {
      //     send(pack[i]);
      //   }
      // }
      // zeros1=0;
      // zeros2=0;
      // zeros3=0;


void  serial() interrupt 4 
{
  if (RI)
  {
     RI = 0;                //清除串行接受标志位
   buf = SBUF;            //从串口缓冲区取得数据
   pack[num]=buf;
   num++;
   if (num==241)
   {
    REN=0;
    num=0;
    
    send_flag=1;
   
   }
  
  }else{
    TI=0;
    sending=0;
  }
  

}


//----------------------------------------------  
