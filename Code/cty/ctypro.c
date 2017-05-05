
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
//����ģʽ1
	SCON=0x50;  //SM0 SM1 SM2 REN TB8 RB8 TI RI 
//T2��ʱ����� 
	RCAP2H=0xff;  
	RCAP2L=0xfd;
//T1��ʱ�����
	TMOD=0X11;
	TH1=(65536-100)/256;	  //100US��ʱ
	TL1=(65536-100)%256;
	ET1=1;
//���ڿ��ж�
	ES=1;
	EA=1; 
//��T1��ʱ��
	TR1=1;  
//��T2��ʱ��
	T2CON=0x34;  //TF2 EXF2 RCLK TCLK EXEN2 TR2 C/T2 CP/RL2


} 

void send(unsigned char d)		  //����һ���ֽڵ����ݣ��β�d��Ϊ���������ݡ�
{
 SBUF=d; //������д�뵽���ڻ���
 send_finish=1;	 //���÷��ͱ�־
 while(send_finish); //�ȴ��������
}

void sendc(unsigned char * pd)
{
 while((*pd)!='\0') //�����ַ�����ֱ������0�Ž���
 {
  send(*pd); //����һ���ַ�
  pd++;  //�ƶ�����һ���ַ�
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

void uart(void) interrupt 4		 //���ڷ����ж�
{

  TI=0;
  send_finish=0;  //�����ڷ��ͱ�־
 
}

void time1() interrupt 3  
{	
     TH1=(65536-100)/256;	  //100US��ʱ
	 TL1=(65536-100)%256;
	 timer++;				  //��ʱ��100USΪ׼���������������ʱ

}
