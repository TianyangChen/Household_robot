	#include <STC89Cxx.h>
	#include <intrins.h>

	#define  ECHO  P24				   //超声波接口定义
	#define  TRIG  P25				   //超声波接口定义

    unsigned char timer2;			//待显示字符。
    unsigned char count1,o_o;
    unsigned char buf,num,i,temp,movechange;
    bit send_flag;
	volatile unsigned char sending;
	unsigned char pdata pack[241];

    unsigned long S=0;
	unsigned int  time=0;		 
	unsigned int  timer=0;			
						
/************************************************************************/
//  		void delay(unsigned int k)	  //延时函数
// {    
//      unsigned int x,y;
// 	   for(x=0;x<k;x++) 
// 	     for(y=0;y<2000;y++);
// }
/************************************************************************/
	void init(void)				//串口初始化
{
	EA=0; //暂时关闭中断
//T2定时器相关设定
	RCAP2H=0xff;  
	RCAP2L=0xfd;
//串口相关设定
 	SCON=0x40;     //串口工作在模式1
 	ES=1;         //串行中断允许
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

/************************************************************************/
    void  StartModule() 		      //启动测距信号，给TRIG一个15us的高电平
{
	TRIG=1;			                
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	TRIG=0;
}
/***************************************************/
	void Count(void)		   //计算距离
{
	while(!ECHO);		       //当RX为零时等待
	TR0=1;			       //开启计数
	while(ECHO);			   //当RX为1计数并等待
	TR0=0;				   //关闭计数
	time=TH0*256+TL0;		   //读取脉宽长度
	TH0=0;
	TL0=0;
	S=(time*1.7)/100;        //算出来是CM
}

	void send(unsigned char d)		  //发送一个字节的数据，形参d即为待发送数据。
{
 
 	SBUF=d; //将数据写入到串口缓冲
 	sending=1;	 //设置发送标志
 	while(sending); //等待发送完毕
}
/***************************************************/
//外部中断0中断函数
   void intersvr1(void) interrupt 0 
{
    count1++;	
}
/***************************************************/
//T0定时器中断程序
 	void timer0() interrupt 1 
{	
   	
 }
/***************************************************/
//T1定时器中断程序
 	void time1()interrupt 3  
{	
    TH1=(65536-100)/256;	  //100US定时
	TL1=(65536-100)%256;
	timer++;				  //定时器100US为准。在这个基础上延时
	if (o_o==3)//前行
	{
	 	if (timer2<7)
	 	{
	 		P10=1;P12=1;P14=1;P16=1;
	 	}else{
	 		P10=0;P12=0;P14=0;P16=0;
	 	}
	 	
	} else if (o_o==2)//右转
	{
	 	if (timer2<5)
	 	{
	 		P10=1;P12=1;P15=1;P17=1;
	 	}else{
	 		P10=0;P12=0;P15=0;P17=0;
	 	}
	 	
	} else if (o_o==1){//后退
	 		P11=1;P13=1;P15=1;P17=1;
	}
	
	timer2++;
	if (timer2>10)
	{
	 	timer2=0;
	}
}
/***************************************************/
//串口中断函数
 	void uart(void) interrupt 4		
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
  	}else
  	{
    	TI=0;
    	sending=0;
  	}
}
/***************************************************/
	void main(void)
{
	unsigned char j,k,m;
	init();
	while(1)		      
	{
		if(timer>=1000)	  //100MS检测启动检测一次	 1000
		{
	    	timer=0;
			StartModule(); //启动检测
        	Count();		  //计算距离	  							   														  
        	if (S<20)//后退
        	{
        		o_o=1;
           		P10=0;P12=0;P14=0;P16=0;
           		send(count1);
           		send('b');	
        	}
        	else if (S<40)//转向
        	{
           		o_o=2;
           		P11=0;P13=0;P14=0;P16=0;
           		send(count1);
           		send('r');   		
        	}
        	else//前行
        	{
           		o_o=3;
           		P11=0;P13=0;P15=0;P17=0;
           		send(count1);
           		send('g');
        	}
        	count1=0;		 //清计数	
        	movechange++;
		}
		if (movechange==10)
		{
	   		EX0=0;TR1=0;//关外部中断，关T1
	   		P10=0;P12=0;P14=0;P16=0;
	   		P11=0;P13=0;P15=0;P17=0;//停车
	   		movechange=0;
	   		REN=1;
	   		for (m = 0; m < 11; m++)
	   		{
	   			while(!send_flag);
	   			for (i = 0; i < 22; i++)
      			{
        			if (pack[i]==0xFA && pack[i+1]>=0xA0 && pack[i+1]<=0xF9)
        			{
          				temp=i;break;
        			}
      			}//寻找起始数据包
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
      			}//数据包整理
      			for (i = 0; i < 90; i++)
      			{
        			send(pack[i]);
      			}
      			send_flag=0;
      			REN=1;
	   		} 
	   		REN=0;
	   		EX0=1;TR1=1;  		
		}
	}   
}

	