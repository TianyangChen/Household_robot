#include <STC89Cxx.h>
unsigned char buf,check,num;

unsigned char laser[9];
unsigned char pack[23];
bit sending,check_passed,send_flag;
void send(unsigned char d);

void init();
void save_data();



void main()
{
	unsigned char i;
	init();
	while(1){
		if (send_flag)
		{
      send_flag=0;

			for (i = 0; i < 22; i++)
			{
					send(laser[i]);	
			}
		  SCON=0x50;
	
		}
	}

}



void init(void)				//���ڳ�ʼ��
{
	EA=0; //��ʱ�ر��ж�
//T2��ʱ������趨
	RCAP2H=0xff;  
	RCAP2L=0xfd;
//��������趨
 	SCON=0x50;     //���ڹ�����ģʽ1
 	ES=0;         //�����ж�����
	T2CON=0x34;  //TF2 EXF2 RCLK TCLK EXEN2 TR2 C/T2 CP/RL2    ������ʱ��2
//�����ж�
 	EA=1;         //�����ж�

} 

// void save_data(){
// 	laser[row][0]=pack[1];
	
// 	if (pack[5]>=0x18)
// 	{
// 		laser[row][1]=0;laser[row][2]=0;zeros++;
// 	}else{
// 		laser[row][1]=pack[4];laser[row][2]=pack[5];
// 	}
// 	if (pack[9]>=0x18)
// 	{
// 		laser[row][3]=0;laser[row][4]=0;zeros++;
// 	}else{
// 		laser[row][3]=pack[8];laser[row][4]=pack[9];
// 	}
// 	if (pack[13]>=0x18)
// 	{
// 		laser[row][5]=0;laser[row][6]=0;zeros++;
// 	}else{
// 		laser[row][5]=pack[12];laser[row][6]=pack[13];
// 	}
// 	if (pack[17]>=0x18)
// 	{
// 		laser[row][7]=0;laser[row][8]=0;zeros++;
// 	}else{
// 		laser[row][7]=pack[16];laser[row][8]=pack[17];
// 	}
// }

void  serial() interrupt 4 
{
	if (RI)
	{
		RI=0;
		buf=SBUF;
		if(check==0&&buf==0xFA)//ͬʱ�ж�count���յ�������  
      {  
          check=1;
        
      }  
      else if(check==1&&buf>=0xA0&&buf<=0xF9)  
        {
          check=2; check_passed=1; P10=0;
            
        }  else 
        {
          check=0; P10=1;
        } 

        if (check_passed)
        {
          num++;
          pack[num]=buf;

          if (num==21)
          {
            SCON=0x40;
            check_passed=0;
            send_flag=1;
            /* code */
          }

          /* code */
        }		
	}else{
		TI=0;
		sending=0;
	}

}

void send(unsigned char d)		  //����һ���ֽڵ����ݣ��β�d��Ϊ���������ݡ�
{
 SBUF=d; //������д�뵽���ڻ���
 sending=1;	 //���÷��ͱ�־
 while(sending); //�ȴ��������
}


