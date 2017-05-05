/*****************HJ-4WD慧净四驱蓝牙智能小车例程*****************
*  平台：HJ-4WD/HJ-E/HL-1智能小车 + Keil U4 + STC89C52
*  名称：
*  公司：慧净电子科技有限公司
*  淘宝：http://shop37031453.taobao.com       
*  网站：www.hjmcu.com	智能小车专用网站：WWW.HLMCU.COM
*  编写：
*  日期：2008-8-08
*  交流:智能车QQ:1526248688  开发板QQ:398115088
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析，有问题直接到慧净论坛交流
*  论坛：http://hjdz.haotui.com 
*  免费结缘：5星好评赠送18个资料包，追加评价后再加送20G资料，网上网盘下载
*  视频教程：本小车配套详细视频教程，有需要请到慧净网站免费下载
******************************************************************/
    // 本程序不输入PWM，J21 PWM 4个跳线帽不要装上，把J20(IN)/J22(EN) 8个跳线帽装上，这样小车就能全速度运行。	
	//注意程序只做参考之用，要达到最理想的效果，还需要同学们细心调试。	
	//注意超声波接线，急性之人经常接错线，烧坏模块，请一定要核对好再通电试验，模块接错线烧坏不保修，不换货的。
	/****************************************************************************
	 硬件连接
	 舵机有三条线定义：

     暗灰： GND

     红色： VCC 4.8-5.2V

     橙黄线： 脉冲输入

	 把舵机插入单片机J9接口中	 --注意不要接错线，否则烧坏舵机，不保修，不换货。
	 超声波用杜邦线接到J6口中
	 自己安装好舵机，还有超声波固定架，小车测试时小心不要碰撞到物体，否则容易断固定支架
	****************************************************************************/

#include <STC89Cxx.h>
#include <intrins.h>

	#define Sevro_moto_pwm     P27	   //接舵机信号端输入PWM信号调节速度

	#define  ECHO  P24				   //超声波接口定义
	#define  TRIG  P25				   //超声波接口定义

	//HJ-4WD小车驱动接线定义
    #define Right_moto_go      {P14=1,P15=0,P16=1,P17=0;}    //左边两个电机向前走
	#define Right_moto_back    {P14=0,P15=1,P16=0,P17=1;}    //左边两个电机向后转
	#define Right_moto_Stop    {P14=0,P15=0,P16=0,P17=0;}    //左边两个电机停转                     
	#define Left_moto_go     {P10=1,P11=0,P12=1,P13=0;}	//右边两个电机向前走
	#define Left_moto_back   {P10=0,P11=1,P12=0,P13=1;}	//右边两个电机向后走
	#define Left_moto_Stop   {P10=0,P11=0,P12=0,P13=0;}	//右边两个电机停转  


	unsigned char const discode[] ={ 0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF,0xff/*-*/};
	unsigned char const positon[3]={ 0xfe,0xfd,0xfb};
	unsigned char disbuff[4]	  ={ 0,0,0,0,};
    unsigned char posit=0;
    unsigned char status;			//待显示字符。
	volatile unsigned char sending;

   	unsigned char pwm_val_left  = 0;//变量定义
	unsigned char push_val_left =14;//舵机归中，产生约，1.5MS 信号
    unsigned long S=0;
	unsigned long S1=0;
	unsigned long S2=0;
	unsigned long S3=0;
	unsigned long S4=0;
	unsigned int  time=0;		    //时间变量
	unsigned int  timer=0;			//延时基准变量
	unsigned char timer1=0;			//扫描时间变量					
/************************************************************************/
 		void delay(unsigned int k)	  //延时函数
{    
     unsigned int x,y;
	   for(x=0;x<k;x++) 
	     for(y=0;y<2000;y++);
}
/************************************************************************/
    void Display(void)				  //扫描数码管
	{ 
	 P0=discode[disbuff[posit]];

	  if(posit==0)
	  {P21=0;P22=1;P23=1;}
	  if(posit==1)
	  {P21=1;P22=0;P23=1;}
	  if(posit==2)
	  {P21=1;P22=1;P23=0;}
	  if(++posit>=3)
	  {posit=0;}
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
	  void Conut(void)		   //计算距离
	{
	  while(!ECHO);		       //当RX为零时等待
	  TR0=1;			       //开启计数
	  while(ECHO);			   //当RX为1计数并等待
	  TR0=0;				   //关闭计数
	  time=TH0*256+TL0;		   //读取脉宽长度
	  TH0=0;
	  TL0=0;
	  S=(time*1.7)/100;        //算出来是CM
	  disbuff[0]=S%1000/100;   //更新显示
	  disbuff[1]=S%1000%100/10;
	  disbuff[2]=S%1000%100%10;
	}
/************************************************************************/
//前速前进
     void  run(void)
{				   
	 Left_moto_go ;     //左电机往前走
	 Right_moto_go ;    //右电机往前走
}
/************************************************************************/
//前速后退
     void  backrun(void)
{
	 Left_moto_back ;   //左电机后走
	 Right_moto_back ;  //右电机往后走
}
/************************************************************************/
//左转
     void  leftrun(void)
{
	 Left_moto_back ;   //左电机往后走
 	 Right_moto_go ;   //右电机往前走
}
/************************************************************************/
//右转
     void  rightrun(void)
{
	 Left_moto_go ;     //左电机往前走
	 Right_moto_back ;  //右电机往后走
}
/************************************************************************/
//STOP
     void  stoprun(void)
{
	 Left_moto_Stop ;   //左电机停走
	 Right_moto_Stop ;  //右电机停走
}
/************************************************************************/
//  void  COMM( void ) 		      
//   {
  	     
		 
// 		  push_val_left=5;	  //舵机向左转90度
// 		  timer=0;
// 		  while(timer<=4000); //延时400MS让舵机转到其位置		 4000
// 		  StartModule();	  //启动超声波测距
//           Conut();	 		  //计算距离
// 		  S2=S;  
  
// 		  push_val_left=23;	  //舵机向右转90度
// 		  timer=0;
// 		  while(timer<=4000); //延时400MS让舵机转到其位置
// 		  StartModule();	  //启动超声波测距
//           Conut();			  //计算距离
// 		  S4=S; 	
	

// 		  push_val_left=14;	  //舵机归中
// 		  timer=0;
// 		  while(timer<=4000); //延时400MS让舵机转到其位置

// 		  StartModule();	  //启动超声波测距
//           Conut();			  //计算距离
// 		  S1=S; 	

//           if((S2<20)||(S4<20)) //只要左右各有距离小于，20CM小车后退
// 		  {
// 		  backrun();		   //后退
// 		  timer=0;
// 		  while(timer<=4000);
// 		  }
		   
// 		  if(S2>S4)		 
// 		     {
// 				rightrun();  	//车的左边比车的右边距离小	右转	
// 		        timer=0;
// 		        while(timer<=4000);
// 		     }				      
// 		       else
// 		     {
// 		       leftrun();		//车的左边比车的右边距离大	左转
// 		       timer=0;
// 		       while(timer<=4000);
// 		     }
		  			   

// }

/************************************************************************/
/*                    PWM调制电机转速                                   */
/************************************************************************/
/*                    左电机调速                                        */
/*调节push_val_left的值改变电机转速,占空比            */
// 		void pwm_Servomoto(void)
// {  
 
//     if(pwm_val_left<=push_val_left)
// 	       Sevro_moto_pwm=1; 
// 	else 
// 	       Sevro_moto_pwm=0;
// 	if(pwm_val_left>=200)
// 	pwm_val_left=0;
 
// }
/***************************************************/
///*TIMER1中断服务子函数产生PWM信号*/
 	void time1()interrupt 3   using 2
{	
     TH1=(65536-100)/256;	  //100US定时
	 TL1=(65536-100)%256;
	 timer++;				  //定时器100US为准。在这个基础上延时
	 pwm_val_left++;
	 // pwm_Servomoto();

	 timer1++;				 //2MS扫一次数码管
	 if(timer1>=20)
	 {
	 timer1=0;
	 Display();	
	 }
 }
/***************************************************/
///*TIMER0中断服务子函数产生PWM信号*/
 	void timer0()interrupt 1   using 0
{	
   	
 }

/***************************************************/

	void init(void)				//串口初始化
{
	EA=0; //暂时关闭中断
//T2定时器相关设定
 	T2CON&=0x0F;
 	T2CON|=0x30;
 	RCAP2H=255;
 	RCAP2L=255;
 	TL2=255;
 	TH2=255;
//串口相关设定
 	SCON=0x50;     //串口工作在模式1
 	PCON|=0x80;    //串口波特率加倍
 	ES=1;         //串行中断允许
//定时器T0,T1设定
 	TMOD=0X11;
	TH1=(65536-100)/256;	  //100US定时
	TL1=(65536-100)%256;
	TH0=0;
	TL0=0;  

	ET1= 1;
	ET0= 1;

	TR1= 1;		//启动定时器1
	TR2=1;      //启动定时器2

 	EA=1;         //允许中断
}

	void send(unsigned char d)		  //发送一个字节的数据，形参d即为待发送数据。
{
 
 	SBUF=d; //将数据写入到串口缓冲
 	sending=1;	 //设置发送标志
 	while(sending); //等待发送完毕
}

	void uart(void) interrupt 4		 //串口发送中断
{

  	TI=0;
  	sending=0;  //清正在发送标志
 
}
 /***************************************************/

	void main(void)
{


	while(1)		       /*无限循环*/
	{ 
	   leftrun();
	

	}
	  
	  
    
}

	