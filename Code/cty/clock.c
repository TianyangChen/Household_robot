#include <REGX52.H>
#define LED_SELECT P2		
#define LED_DATA   P0
#define KEY_PIN    P3
//变量定义
unsigned char light[]={3,2,0,7,5,4,1,6};
static unsigned char count_1s=0	   ;
typedef enum 
{
	NULL, KEY_ENTER, KEY_UP, KEY_DOWN
}TYPEDEF_KEY; 
enum SHOW_STATE  				 
{
	SHOW_NORMAL, SHOW_SEC_ADJ, SHOW_MIN_ADJ,SHOW_HOUR_ADJ,
}ShowState; 	 
 unsigned char code LEDMAP[] =		//共阴极显示代码
{
	0xc0, 0xf9, 0xa4, 0xb0, 0x99, 	
	0x92, 0x82, 0xf8, 0x80, 0x90,	
	0xff,					//熄灭
};
enum   				 
{
	LED_GOOUT = 10,
}; 
unsigned char LEDBuffer[6]; 		   //显示缓冲区
unsigned char Second;			   
unsigned char Minute;
unsigned char Hour;	
unsigned char count;	
bit Timer1sFlg;	
//函数声明
void AT89S51Config(void); 
void VariableInitial(void);
void ShowNormal(void);
void ShowSecondAdj(void);
void ShowMinuteAdj(void);
void ShowHourAdj(void);	 
void Delay(unsigned int t);	   
void Display(unsigned char * buf, unsigned char number);  //动态显示
unsigned char GetKey(void);
/*——————————————————————————————主函数————————————————————————————————————————————
————————————————————————————————————————————————————————————————————————————————*/
main(void)
{ 	
	AT89S51Config(); 			 //AT89S51配置
	VariableInitial();   		 //变量初始化
	while(1)
	{	
		switch(ShowState)
		{
		case SHOW_NORMAL:
			ShowNormal();
			break;
		case SHOW_SEC_ADJ:
			ShowSecondAdj();
			break;
		case SHOW_MIN_ADJ:
			ShowMinuteAdj();
			break;
		case SHOW_HOUR_ADJ:
			ShowHourAdj();
			break;
		default:
			break;
		}
		Display(LEDBuffer,6); 				
	}	
}
/*——————————————————————————————配置函数————————————————————————————————————————————
————————————————————————————————————————————————————————————————————————————————*/
void AT89S51Config(void)
{ 	TMOD|=0x20;	   //串口配置————————————————————————————————————————
    PCON&=0x7f;
    TH1=TL1=0xfd;
    TR1=1;
    SCON=0x50;	   //串口配置————————————————————————————————————————
	RCAP2H=0x0F;//T2定时器初值
    RCAP2L=0x00;
    ET2=1;//允许T2定时器中断
    TR2=1;//启动T2定时器
    EA=1;//开总中断
}
/*——————————————————————————————变量初始化————————————————————————————————————————————
————————————————————————————————————————————————————————————————————————————————*/
void VariableInitial(void)
{ 
	ShowState = 0;
	Second = 0;			   
	Minute = 0;
	Hour   = 0;
	Timer1sFlg = 0;	
	count=0;
}	  
/*——————————————————————————————延时函数————————————————————————————————————————————
————————————————————————————————————————————————————————————————————————————————*/
void Delay(unsigned int t)
{ 
	for (t; t>0; t--) ;	
}
/*——————————————————————————————正常显示界面————————————————————————————————————————————
————————————————————————————————————————————————————————————————————————————————*/
void ShowNormal(void)
{   unsigned char i;
	TYPEDEF_KEY key;

	//1秒钟到，显示值更新
	if(Timer1sFlg)
	{
		Timer1sFlg = 0;

		Second++;		
		if(Second >= 60)
		{
			Second = 0;
			Minute ++;	
			if(Minute >= 60)
			{
				Minute = 0;
				Hour ++;	
				if(Hour >= 24)
				{
					Hour = 0;			
				}
			}
		}  	
	for(i=0;i<6;i++)
	  {
	    if(TI==0)   //——————————给串口送数据，  既给 matlab 送数据———————————————————————————
	    { 				    	    //———————————————————————————————————————————————————————            
	      SBUF=LEDBuffer[5-i];
		  while (!TI );  						   //操作对象是TI
          TI=0 ;
	    }
	  }		
	}
	LEDBuffer[0] = Second % 10;		//秒个位送显示缓冲区
	LEDBuffer[1] = Second / 10;		//秒十位送显示缓冲区
	LEDBuffer[2] = Minute % 10;
	LEDBuffer[3] = Minute / 10;
	LEDBuffer[4] = Hour   % 10;
	LEDBuffer[5] = Hour   / 10;
	
	key = GetKey();
	if(key == KEY_ENTER)
	{
		ShowState = SHOW_SEC_ADJ;
	} 	
	if(RI==1)
	{ 
	  RI=0;
      TI=0;
	  ShowState = SHOW_SEC_ADJ;
   	}	
}
/*——————————————————————————————秒钟调整————————————————————————————————————————————
————————————————————————————————————————————————————————————————————————————————*/
void ShowSecondAdj(void)
{	unsigned char i;
	TYPEDEF_KEY key;
	LEDBuffer[0] = Second % 10;		//秒个位送显示缓冲区
	LEDBuffer[1] = Second / 10;		//秒十位送显示缓冲区
	LEDBuffer[2] = LED_GOOUT;	//熄灭
	LEDBuffer[3] = LED_GOOUT;	//熄灭
	LEDBuffer[4] = LED_GOOUT;	//熄灭
	LEDBuffer[5] = LED_GOOUT;	//熄灭
	Display(LEDBuffer,6);
	key = GetKey();
	switch(key)
	{
	case KEY_ENTER:
		ShowState = SHOW_MIN_ADJ;
		break;
	case KEY_UP:
		Second++;
		if(60 == Second)
		{
			Second = 0;
		}
		break;
	case KEY_DOWN:		
		if(0 == Second)
		{
			Second = 59;
		}
		else
		{
			Second--;
		}		
		break;
	}
	for(i=0;i<6;i++)
	  {
	    LEDBuffer[2]=0;
		LEDBuffer[3]=0;
		LEDBuffer[4]=0;
		LEDBuffer[5]=0;
	    if(TI==0)   //——————————给串口送数据，  既给 matlab 送数据———————————————————————————
	    { 				    	    //———————————————————————————————————————————————————————            
	      SBUF=LEDBuffer[5-i];
		  while (!TI );  						   //操作对象是TI
          TI=0 ;
	    }
	  }
	if(RI==1)
	{ RI=0;
	  Second=SBUF;
      TI=0;
	  LEDBuffer[0] = Second % 10;		//秒个位送显示缓冲区
	  LEDBuffer[1] = Second / 10;		//秒十位送显示缓冲区
	  ShowState = SHOW_MIN_ADJ;
	  for(i=0;i<100;i++)
	  {Display(LEDBuffer,6);}
   	}		 		
}
/*——————————————————————————————分钟调整————————————————————————————————————————————
————————————————————————————————————————————————————————————————————————————————*/
void ShowMinuteAdj(void)
{	 	 
    unsigned char i;

	TYPEDEF_KEY key;
	LEDBuffer[0] = LED_GOOUT;	//熄灭
	LEDBuffer[1] = LED_GOOUT;	//熄灭
	LEDBuffer[2] = Minute % 10;		//分个位送显示缓冲区
	LEDBuffer[3] = Minute / 10;		//分十位送显示缓冲区
	LEDBuffer[4] = LED_GOOUT;	//熄灭
	LEDBuffer[5] = LED_GOOUT;	//熄灭
	Display(LEDBuffer,6);
	key = GetKey();
	switch(key)
	{
	case KEY_ENTER:
		ShowState = SHOW_HOUR_ADJ;
		break;
	case KEY_UP:
		Minute++;
		if(60 == Minute)
		{
			Minute = 0;
		}
		break;
	case KEY_DOWN:		
		if(0 == Minute)
		{
			Minute = 59;
		}
		else
		{
			Minute--;
		}		
		break;
	}
	for(i=0;i<6;i++)
	  {
	    LEDBuffer[0] = Second % 10;		//秒个位送显示缓冲区
	    LEDBuffer[1] = Second / 10;		//秒十位送显示缓冲区
		LEDBuffer[4]=0;
		LEDBuffer[5]=0;
	    if(TI==0)   //——————————给串口送数据，  既给 matlab 送数据———————————————————————————
	    { 				    	    //———————————————————————————————————————————————————————            
	      SBUF=LEDBuffer[5-i];
		  while (!TI );  						   //操作对象是TI
          TI=0 ;
	    }
	  }
	if(RI==1)
	{ RI=0;
	  Minute=SBUF;
	  LEDBuffer[2] = Minute % 10;		//分个位送显示缓冲区
	  LEDBuffer[3] = Minute / 10;		//分十位送显示缓冲区
      TI=0;
	  Display(LEDBuffer,6);	
	 
	 ShowState = SHOW_HOUR_ADJ;
	 for(i=0;i<100;i++)
	{Display(LEDBuffer,6);
	}
   	 }	 		
}
/*——————————————————————————————时钟调整————————————————————————————————————————————
————————————————————————————————————————————————————————————————————————————————*/
void ShowHourAdj(void)
{	unsigned char i;
	TYPEDEF_KEY key;
	LEDBuffer[0] = LED_GOOUT;	//熄灭
	LEDBuffer[1] = LED_GOOUT;	//熄灭
	LEDBuffer[2] = LED_GOOUT;	//熄灭
	LEDBuffer[3] = LED_GOOUT;	//熄灭
	LEDBuffer[4] = Hour % 10;		//小时个位送显示缓冲区
	LEDBuffer[5] = Hour / 10;		//小时十位送显示缓冲区
	Display(LEDBuffer,6);
	key = GetKey();
	switch(key)
	{
	case KEY_ENTER:
		ShowState = SHOW_NORMAL;
		break;
	case KEY_UP:
		Hour++;
		if(24 == Hour)
		{
			Hour = 0;
		}
		break;
	case KEY_DOWN:		
		if(0 == Hour)
		{
			Hour = 23;
		}
		else
		{
			Hour--;
		}		
		break;
	}
	for(i=0;i<6;i++)
	  {
	    LEDBuffer[0] = Second % 10;		//秒个位送显示缓冲区
	    LEDBuffer[1] = Second / 10;		//秒十位送显示缓冲区
	    LEDBuffer[2] = Minute % 10;
	    LEDBuffer[3] = Minute / 10;
	    if(TI==0)   //——————————给串口送数据，  既给 matlab 送数据———————————————————————————
	    { 			    	    //———————————————————————————————————————————————————————            
	      SBUF=LEDBuffer[5-i];
		  while (!TI );  						   //操作对象是TI
          TI=0 ;
	    }
	  }
	if(RI==1)
	{ RI=0;
	  Hour=SBUF;
      TI=0;
	  LEDBuffer[4] = Hour % 10;		//小时个位送显示缓冲区
	  LEDBuffer[5] = Hour / 10;		//小时十位送显示缓冲区
	  	
	  ShowState = SHOW_NORMAL;
	  for(i=0;i<100;i++)
	 {Display(LEDBuffer,6);;}
	 }	 		
}
/*——————————————————————————————显示函数————————————————————————————————————————————
————————————————————————————————————————————————————————————————————————————————*/
void Display(unsigned char * buf, unsigned char number)
{
	unsigned char i;
	for (i = 0; i < number; i++)
	{
		LED_SELECT = 0x00;				//关所有LED （防止出现闪烁现象）
		LED_DATA = LEDMAP[*(buf + i)];	//送数据	
		LED_SELECT = ~(1<<light[i]); 				//显示某1位数码管，其余关闭	
		Delay(90);		 				// 延时约1ms,参数与晶振大小有关
	}
}
/*——————————————————————————————定时器五————————————————————————————————————————————
————————————————————————————————————————————————————————————————————————————————*/
Timer2_server(void)  interrupt 5			 //* 
{
    TF2=0;
    count_1s++;
	if(20 == count_1s)
	{
		Timer1sFlg = 1;
	    count_1s =0;		
    }
}
/*——————————————————————————————得到按键值————————————————————————————————————————————
————————————————————————————————————————————————————————————————————————————————*/
TYPEDEF_KEY GetKey(void)
{	
	TYPEDEF_KEY key = NULL;		
	if( ~KEY_PIN & 0x1c)	    //有键按下
	{
		Delay(1000);			//去抖动			
		if(~KEY_PIN & 0X04)
		{
			key = KEY_ENTER;
		}
		else if(~KEY_PIN & 0X08)
		{
			key = KEY_UP;
		}
		else if(~KEY_PIN & 0X10)
		{
			key = KEY_DOWN;
		}		
		else
		{
			return(NULL);
		}								
		while(KEY_PIN != 0XFF)	//等待键抬起
		{
			Delay(2);			
		} 							 				
		return(key);					
	}	
	return(NULL);	
}
