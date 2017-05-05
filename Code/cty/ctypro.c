
#include <STC89Cxx.h>

unsigned char str[]="Huazhong university of Sci. & Tech.!";  
unsigned char cnum[4]={ 1,1,1,1};  
unsigned char num2[]={ '2','2','2','2','\0'};
unsigned char num3[]={ 3,3,3,3,'\0'}; 
unsigned char i;
unsigned char send_finish;  
unsigned int timer=0;

void delay_ms(unsigned char xms)  
{  
	unsigned char x,y;  
	for(x=xms; x>0; x--)  
	for(y=248; y>0; y--);  
}   

void main_init()  
{   
//串口模式1
	SCON=0x50;  //SM0 SM1 SM2 REN TB8 RB8 TI RI 
//T2定时器相关 
	RCAP2H=0xff;  
	RCAP2L=0xfd;
//T1定时器相关
	TMOD=0X11;
	TH1=(65536-100)/256;	  //100US定时
	TL1=(65536-100)%256;
	ET1=1;
//串口开中断
	ES=1;
	EA=1; 
//开T1定时器
	TR1=1;  
//开T2定时器
	T2CON=0x34;  //TF2 EXF2 RCLK TCLK EXEN2 TR2 C/T2 CP/RL2


} 

void send(unsigned char d)		  //发送一个字节的数据，形参d即为待发送数据。
{
 SBUF=d; //将数据写入到串口缓冲
 send_finish=1;	 //设置发送标志
 while(send_finish); //等待发送完毕
}

void sendc(unsigned char * pd)
{
 while((*pd)!='\0') //发送字符串，直到遇到0才结束
 {
  send(*pd); //发送一个字符
  pd++;  //移动到下一个字符
 }
}

void main()  
{  
	main_init(); 
	while(1){
		// if (timer>=1000)
		// {
		// 	timer=0;
		// 	send('c');
		// 	/* code */
		// }
		if(P34==0)
		{
			delay_ms(8);
			if(P34==0)
			{
				while(!P34);
				send('g');		   
			}
		}
		if(P35==0)
		{
			delay_ms(8);
			if(P35==0)
			{
				while(!P35);
				send('r');			   
			}
		}
		if(P36==0)
		{
			delay_ms(8);
			if(P36==0)
			{
				while(!P36);
				send('b');		   
			}
		}
		if(P37==0)
		{
			delay_ms(8);
			if(P37==0)
			{
				while(!P37);
				
					sendc(str);		   
			}
		}
	} 	  
} 

void uart(void) interrupt 4		 //串口发送中断
{

  TI=0;
  send_finish=0;  //清正在发送标志
 
}

void time1() interrupt 3  
{	
     TH1=(65536-100)/256;	  //100US定时
	 TL1=(65536-100)%256;
	 timer++;				  //定时器100US为准。在这个基础上延时

}
