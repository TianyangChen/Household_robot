	#include <STC89Cxx.h>
	#include <intrins.h>

	#define  ECHO  P24				   //�������ӿڶ���
	#define  TRIG  P25				   //�������ӿڶ���

    unsigned char timer2;			//����ʾ�ַ���
    unsigned char count1,o_o;
	volatile unsigned char sending;

    unsigned long S=0;
	unsigned int  time=0;		 
	unsigned int  timer=0;			
						
/************************************************************************/
//  		void delay(unsigned int k)	  //��ʱ����
// {    
//      unsigned int x,y;
// 	   for(x=0;x<k;x++) 
// 	     for(y=0;y<2000;y++);
// }
/************************************************************************/
	void init(void)				//���ڳ�ʼ��
{
	EA=0; //��ʱ�ر��ж�
//T2��ʱ������趨
	RCAP2H=0xff;  
	RCAP2L=0xfd;
//��������趨
 	SCON=0x40;     //���ڹ�����ģʽ1
 	ES=1;         //�����ж�����
//��ʱ��T0,T1�趨
 	TMOD=0X11;
	TH1=(65536-100)/256;	  //100US��ʱ
	TL1=(65536-100)%256;
	TH0=0;
	TL0=0;  
//��ʱ��T0,T1���ж�
	ET1= 1;
	ET0= 1;
//������ʱ��T1,T2
	TR1= 1;		//������ʱ��1
	T2CON=0x34;  //TF2 EXF2 RCLK TCLK EXEN2 TR2 C/T2 CP/RL2    ������ʱ��2
//�����ж�
 	EA=1;         //�����ж�
//�ⲿ�жϳ�ʼ��
 	EX0=1;	   //�����ⲿ�ж�0
    IT0=1;	   //�½�����Ч
	IE0=0;
}

/************************************************************************/
    void  StartModule() 		      //��������źţ���TRIGһ��15us�ĸߵ�ƽ
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
	void Conut(void)		   //�������
{
	while(!ECHO);		       //��RXΪ��ʱ�ȴ�
	TR0=1;			       //��������
	while(ECHO);			   //��RXΪ1�������ȴ�
	TR0=0;				   //�رռ���
	time=TH0*256+TL0;		   //��ȡ������
	TH0=0;
	TL0=0;
	S=(time*1.7)/100;        //�������CM
}

	void send(unsigned char d)		  //����һ���ֽڵ����ݣ��β�d��Ϊ���������ݡ�
{
 
 	SBUF=d; //������д�뵽���ڻ���
 	sending=1;	 //���÷��ͱ�־
 	while(sending); //�ȴ��������
}
/***************************************************/
//�ⲿ�ж�0�жϺ���
   void intersvr1(void) interrupt 0 
{
    count1++;	
}
/***************************************************/
//T0��ʱ���жϳ���
 	void timer0() interrupt 1 
{	
   	
 }
/***************************************************/
//T1��ʱ���жϳ���
 	void time1()interrupt 3  
{	
    TH1=(65536-100)/256;	  //100US��ʱ
	TL1=(65536-100)%256;
	timer++;				  //��ʱ��100USΪ׼���������������ʱ
	if (o_o==3)//ǰ��
	{
	 	if (timer2<7)
	 	{
	 		P10=1;P12=1;P14=1;P16=1;
	 	}else{
	 		P10=0;P12=0;P14=0;P16=0;
	 	}
	 	
	} else if (o_o==2)//��ת
	{
	 	if (timer2<5)
	 	{
	 		P10=1;P12=1;P15=1;P17=1;
	 	}else{
	 		P10=0;P12=0;P15=0;P17=0;
	 	}
	 	
	} else if (o_o==1){//����
	 		P11=1;P13=1;P15=1;P17=1;
	}
	
	timer2++;
	if (timer2>10)
	{
	 	timer2=0;
	}
}
/***************************************************/
//�����жϺ���
 	void uart(void) interrupt 4		
{
  	TI=0;
  	sending=0;  //�����ڷ��ͱ�־
}
/***************************************************/
	void main(void)
{
	init();
	while(1)		      
	{ 
	 if(timer>=1000)	  //100MS����������һ��	 1000
	   {
	        timer=0;
		    StartModule(); //�������
            Conut();		  //�������	  							   														  
        	if (S<20)//����
        	{
        		o_o=1;
           		P10=0;P12=0;P14=0;P16=0;
           		send(count1);
           		send('b');
           		
           		
	 			
           	}
           	else if (S<40)//ת��
           	{
           		o_o=2;
           		P11=0;P13=0;P14=0;P16=0;
           		send(count1);
           		send('r');
           		
           	}
           	else//ǰ��
           	{
           		o_o=3;
           		P11=0;P13=0;P15=0;P17=0;
           		send(count1);
           		send('g');
           		
           	}
           	count1=0;		 //�����	
	   	}
	}   
}

	