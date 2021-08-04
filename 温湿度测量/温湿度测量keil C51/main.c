#include<reg52.h>
#include<intrins.h>
 
typedef unsigned char uchar;
typedef unsigned int  uint;
 
 
#define DELAY_TIME 5
 
 
uchar value[5];
uint temp,hum; 
 
sbit w1=P2^0;
sbit w2=P2^1;
sbit w3=P2^2;
sbit w4=P2^3;
sbit Bus=P2^4;
//P3�¶ȣ�P1ʪ��
 
void Delay10us(void);//10us��ʱ����
void Delay20us(void);//20us��ʱ���� 
void Delay1ms(void);//1ms��ʱ����
void Read_value(void);//��ֵ�Ӻ���
void Show(uint x,y);//��ʾ�Ӻ���
 
 
 
void main()
{
	uint i;
	for(i=0;i<2000;i++)
	{
		Delay1ms();
	}
 
	Read_value();//��ֵ	
 
	hum=value[0]*256+value[1];//����ʪ��ֵ
	temp=(value[2]&0x7f)*256+value[3];//����¶�Ϊ��ֵ,��&0x3fȥ������λ���ټ��㣻��Ϊ��ֵ����Ӱ��
 
	Show(temp,hum);	
}
 
 
 
 
 
 
/***********************10us��ʱ����*****************************/
void Delay10us()
{
	uchar i;
	i=2;
	while(--i);
      	
}
/***********************20us��ʱ����*****************************/
void Delay20us(void)   //��� 0us
{
    unsigned char a,b;
    for(b=3;b>0;b--)
        for(a=1;a>0;a--);
}
 
 
 
/***********************1ms��ʱ����*****************************/
void Delay1ms()   //��� 0us
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=142;b>0;b--)
            for(a=2;a>0;a--);
}
/************************��ֵ����******************************/
void Read_value()
{	
	uchar i,j=0,mask;
	Bus=0;//����������������1ms(20ms)
	for(i=0;i<20;i++)
	{
		Delay1ms();
	}
	Bus=1;//������������20us
	while(Bus);//�ȴ��ӻ���������
	while(!Bus);//�ȹ��ӻ�����ʱ��
	while(Bus);//�ȹ��ӻ�����ʱ��
	
	while(j<5)
	{
		mask=0x80;
		for(i=0;i<8;i++)
		{
			while(!Bus);//�ȹ�����λ�ĵ͵�ƽ
			Delay20us();//�ӳ�40us����е�ƽ���
			Delay20us();		
			if(Bus==0)
			{
				value[j]&=(~mask);
			}
			else
			{
				value[j]|=mask;
			}
			mask>>=1;
			while(Bus);//�ȹ�ʣ��ߵ�ƽ�������1�Ļ���
		}
		j++;
	}
}
 
 
 
/**********************��ʾ����*************************************/
void Show(uint x,y)
{
	char duan1[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};//����
	char duan2[10]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
	while(1)
	{
		w1=1;//С��λ
		w2=0;
		w3=0;
		w4=0;
		Delay10us();
		P3=duan1[x%10];
		P1=duan1[y%10];
		Delay1ms();
		P3=0xff;
		P1=0xff;
 
 
		w1=0;//��λ
		w2=1;
		w3=0;
		w4=0;
		Delay10us();
		P3=duan2[(x/10)%10];
		P1=duan2[(y/10)%10];
		Delay1ms();
		P3=0xff;
		P1=0xff;
 
		w1=0;//ʮλ
		w2=0;
		w3=1;
		w4=0;
		Delay10us();
		P3=duan1[(x/100)%10];
		P1=duan1[(y/100)%10];
		Delay1ms();
		P3=0xff;
		P1=0xff;
 
		w1=0;//��λ
		w2=0;
		w3=0;
		w4=1;
		Delay10us();
		if(value[2]>127)//���¶����λΪ1����ʾ��ֵ
		{
			P3=0xbf;//��ʾ����
		}
		else
		{
			P3=duan1[(x/1000)%10];
		}
		P1=duan1[(y/1000)%10];
		Delay1ms();
		P3=0xff; 
		P1=0xff;
	}
}