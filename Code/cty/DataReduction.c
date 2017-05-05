#include <STC89Cxx.h>
unsigned char buf,times,va1,num,save_times,sending,timer1,timer2;
unsigned int posit;
unsigned char laser[720];
unsigned char pack[23];
void send();
void sendc();
void init();
void save_data();



void main()
{
	init();
	while(1);
}

void reduction(){

}

void init(void)				//串口初始化
{
	EA=0; //暂时关闭中断
//T2定时器相关设定
	RCAP2H=0xff;  
	RCAP2L=0xfd;
//串口相关设定
 	SCON=0x50;     //串口工作在模式1
 	ES=0;         //串行中断允许
//定时器T0,T1设定
 	TMOD=0X11;
	TH1=(65536-100)/256;	  //100US定时
	TL1=(65536-100)%256;
	TH0=0;
	TL0=0;  
//定时器T0,T1开中断
	ET1= 1;
	ET0= 1;
//启动定时器T1,T2
	TR1= 1;		//启动定时器1
	T2CON=0x34;  //TF2 EXF2 RCLK TCLK EXEN2 TR2 C/T2 CP/RL2    启动定时器2
//开总中断
 	EA=1;         //允许中断
//外部中断初始化
 	EX0=1;	   //开启外部中断0
    IT0=1;	   //下降沿有效
	IE0=0;
} 

void save_data(){
	posit=(pack[1]-160)*8;
	if (pack[5]>=0x18)
	{
		laser[posit]=0;laser[posit+1]=0;
	}else{
		laser[posit]=pack[4];laser[posit+1]=pack[5];
	}
	if (pack[9]>=0x18)
	{
		laser[posit+2]=0;laser[posit+3]=0;
	}else{
		laser[posit+2]=pack[8];laser[posit+3]=pack[9];
	}
	if (pack[13]>=0x18)
	{
		laser[posit+4]=0;laser[posit+5]=0;
	}else{
		laser[posit+4]=pack[12];laser[posit+5]=pack[13];
	}
	if (pack[17]>=0x18)
	{
		laser[posit+6]=0;laser[posit+7]=0;
	}else{
		laser[posit+6]=pack[16];laser[posit+7]=pack[17];
	}
}

void  serial() interrupt 4 
{
	if (RI)
	{
		RI=0;
		buf=SBUF;
		times++;
		if (buf==0xFA)
		{
			va1=1;
			times=1;
		}
		if (va1==1&&times==2)
		{
			if (buf>=0xA0&&buf<=0xF9)
			{
				va1=0;
				num=0;
			}
		}
		num++;
		pack[num]=buf;
		if (num==22)
		{
			num=0;
			save_data();
			save_times++;
		}
		if (save_times>=90)
		{
			sendc(laser);
			ES=0;
			TR1=1;

		}
	}

	if (TI)
	{
		
  	TI=0;
  	sending=0;
	}

}

void send(unsigned char d)		  //发送一个字节的数据，形参d即为待发送数据。
{
 SBUF=d; //将数据写入到串口缓冲
 sending=1;	 //设置发送标志
 while(sending); //等待发送完毕
}

void sendc(unsigned char * pd)
{
 while((*pd)!='\0') //发送字符串，直到遇到0才结束
 {
  send(*pd); //发送一个字符
  pd++;  //移动到下一个字符
 }
}

void time1() interrupt 3  
{	
    TH1=(65536-100)/256;	  //100US定时
	TL1=(65536-100)%256;

	 // pwm_val_left++;
	 // pwm_Servomoto();

	timer1++;				 //20MS扫一次数码管
	if(timer1>=200)
	{
	 	timer2++;
	 	if (timer2>=200)//4s
	 	{
	 		ES=1;
	 		TR1=0;
	 		timer2=0;
	 	}
		timer1=0;
	}
}