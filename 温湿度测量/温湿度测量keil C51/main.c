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
//P3温度，P1湿度
 
void Delay10us(void);//10us延时函数
void Delay20us(void);//20us延时函数 
void Delay1ms(void);//1ms延时函数
void Read_value(void);//读值子函数
void Show(uint x,y);//显示子函数
 
 
 
void main()
{
	uint i;
	for(i=0;i<2000;i++)
	{
		Delay1ms();
	}
 
	Read_value();//读值	
 
	hum=value[0]*256+value[1];//计算湿度值
	temp=(value[2]&0x7f)*256+value[3];//如果温度为负值,则&0x3f去掉符号位后再计算；若为正值则无影响
 
	Show(temp,hum);	
}
 
 
 
 
 
 
/***********************10us延时函数*****************************/
void Delay10us()
{
	uchar i;
	i=2;
	while(--i);
      	
}
/***********************20us延时函数*****************************/
void Delay20us(void)   //误差 0us
{
    unsigned char a,b;
    for(b=3;b>0;b--)
        for(a=1;a>0;a--);
}
 
 
 
/***********************1ms延时函数*****************************/
void Delay1ms()   //误差 0us
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=142;b>0;b--)
            for(a=2;a>0;a--);
}
/************************读值函数******************************/
void Read_value()
{	
	uchar i,j=0,mask;
	Bus=0;//主机拉低总线至少1ms(20ms)
	for(i=0;i<20;i++)
	{
		Delay1ms();
	}
	Bus=1;//主机主动拉高20us
	while(Bus);//等待从机拉低总线
	while(!Bus);//度过从机拉低时间
	while(Bus);//度过从机拉高时间
	
	while(j<5)
	{
		mask=0x80;
		for(i=0;i<8;i++)
		{
			while(!Bus);//度过数据位的低电平
			Delay20us();//延迟40us后进行电平检测
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
			while(Bus);//度过剩余高电平（如果是1的话）
		}
		j++;
	}
}
 
 
 
/**********************显示函数*************************************/
void Show(uint x,y)
{
	char duan1[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};//段码
	char duan2[10]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
	while(1)
	{
		w1=1;//小数位
		w2=0;
		w3=0;
		w4=0;
		Delay10us();
		P3=duan1[x%10];
		P1=duan1[y%10];
		Delay1ms();
		P3=0xff;
		P1=0xff;
 
 
		w1=0;//个位
		w2=1;
		w3=0;
		w4=0;
		Delay10us();
		P3=duan2[(x/10)%10];
		P1=duan2[(y/10)%10];
		Delay1ms();
		P3=0xff;
		P1=0xff;
 
		w1=0;//十位
		w2=0;
		w3=1;
		w4=0;
		Delay10us();
		P3=duan1[(x/100)%10];
		P1=duan1[(y/100)%10];
		Delay1ms();
		P3=0xff;
		P1=0xff;
 
		w1=0;//百位
		w2=0;
		w3=0;
		w4=1;
		Delay10us();
		if(value[2]>127)//即温度最高位为1，表示负值
		{
			P3=0xbf;//显示负号
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