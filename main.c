/*������ʵ���¶ȿ���ƽ̨���趨Ŀ���¶ȡ��������¹��ܡ�
Ӳ����ʹ��оƬSTC89C52	P37������DS18B20�¶ȴ�������P20�����Ӽ����źţ�����Ҫ����ʱ��Ϊ�ߵ�ƽ������Ϊ�͵�ƽ��P0������8λ8�ι�������ܣ�P22��P23��P24����
74HC138��������P1�����Ӿ�����̣�P30����K1������P31����K2����
����
1,�趨Ŀ���¶ȣ�����K1������ͨ�������������Ŀ���¶ȣ�����Ϊ16����4λ����2���Ʋ��룬��Χ��-999~999,������Χ�ᱨ���������ʾE00������Ŀ���¶�Ϊ32���϶ȣ�
��Ҫ����0020.������ɺ����4λ�������ʾĿ���¶ȣ��ұ�4λ�������ʾ��ǰ�¶ȣ���ǰ�¶Ƚ�����ʾʮλ����λ����Ŀ���¶ȸ��ڵ�ǰ�¶ȣ������ź�Ϊ�ߵ�ƽ�����ȡ�
2,�������¡�����K2�������Զ������������¶ȣ�ͣ��10s���ң�����1�ȣ���ͣ10s���ң��ظ���ֹ��

����˵����Heat�����źš�K1��K2����������ʱΪ0��LSA,LSB,LSCΪ���������롣Bit0��Bit7Ϊ8λ����ܴ���������ʾ�����֡��ַ���16Ϊ����ʾ��17Ϊ����
TemΪ��ǰ�¶ȣ�Tem_oldΪ��һ�β����¶ȣ�TemTargetΪĿ���¶ȣ�KeyValueΪ����һ�ξ��󰴼���ֵ��i_temperatureѭ��������TempVariable��ʱ����*/
//Bit4��Bit7��ǰ�ĸ�����ܣ��ǵ�ǰ�¶ȣ�ʮ������ʾ��


#include<math.h>
#include "reg52.h"	
#include "Ds18b20.h"
#include "lcd.h"

sbit fun1=P2^1;
			

					
sbit fun2=P2^5;


sbit k1=P3^0;	 
sbit k2=P3^1;	

u8 cTInteger1;
u8 cTInteger2;
u8 cTDec;

sbit Heat=P2^0;
float Tem =0;
float Tem_old =0;
short TemTarget;
int KeyValue;   
static  i_temperature=0;
static  i_CountTime=0;
int TempVariable;

void delay(int i_temperature);
void Display();
float datapros(int a);
int KeyDown();
void Timer0Init();
void Timer1Init();
void Heat2TargetTem();
void AutoSet();
void DetectTem();


void main(){
	Heat=0;

	fun1=0;
	fun2=0;
	P0=0x00;
	
	while(1){  
		if(k1==0){
		//function1
		 delay(1000);
		 if(k1==0) Heat2TargetTem();}
			 
		if(k2==0){	
		//function2
			delay(1000);
			if(k2==0) AutoSet();}}}

void Heat2TargetTem(){
	fun1=1;
	
	//Time init 
	Timer0Init();
	Timer1Init();
	
	TempVariable=KeyDown();
	TemTarget=TempVariable*16*16*16;
	TempVariable=KeyDown();
	TemTarget=TemTarget+TempVariable*16*16;
	TempVariable=KeyDown();
	TemTarget=TemTarget+TempVariable*16;
	TempVariable=KeyDown();
	TemTarget=TemTarget+TempVariable;

		
}

void AutoSet(){
	int i_CountTime=20;
	fun2=1;
	Timer1Init();
	Timer0Init();
	
	//adjust target temperature to be integer
	while(i_CountTime--){
		i_temperature=2;
		TF0=1;
		TemTarget=(int)ceil(Tem)+1;}

	//display target temperature

			
	while(1){
		//waitingphase
		Heat=0;
		
		//maintain temperature stage
		if(Tem-TemTarget>-0.2)
			Heat=1;
		if(Tem-TemTarget>0.2)
			Heat=0;

		if(i_CountTime>=1000){
			//Heatingphase
			Heat=1;
			TemTarget++;
			
			//display target temperature

			
			while(TemTarget>Tem)i_CountTime=0;}}}
		


void DetectTem() interrupt 1{
	i_temperature=i_temperature+1;
	TH0=0X00;
	TL0=0X00;
	
	//5 times per second
	if(i_temperature==3){
		Tem=datapros(Ds18b20ReadTemp());
		i_temperature=0;}}



void Timer0Init(){
	TMOD=0X11;
	TH0=0X00;
	TL0=0X00;	
	
	EA=1;
	ET0=1;
	TR0=1;}

void Timer1Init(){
	TMOD=0X11;
	TH1=0XD8;
	TL1=0XF0;	
	
	EA=1;
	ET1=1;
	TR1=1;}
int KeyDown(){
	char a=0;
	P1=0x0f;
	while(1){
		//ensure key is pressed
		if(P1!=0x0f)
	   {delay(1000);
		if(P1!=0x0f)
		 break;}}	 	
	
	//detect column
	P1=0X0F;
	switch(P1){
		case(0X07):	KeyValue=0;break;
		case(0X0b):	KeyValue=1;break;
		case(0X0d): KeyValue=2;break;
		case(0X0e):	KeyValue=3;break;}
	
	//detect row
	P1=0XF0;
	switch(P1){
		case(0X70):	KeyValue=KeyValue;break;
		case(0Xb0):	KeyValue=KeyValue+4;break;
		case(0Xd0): KeyValue=KeyValue+8;break; 
		case(0Xe0):	KeyValue=KeyValue+12;break;}
	
	//wait for uplift
	while(P1!=0xf0){
		delay(100);
		a++;}
	
	return KeyValue;}

void delay(int i_temperature){
	while(i_temperature--);}
float datapros(int a){ 
	//extract every number
  cTInteger1 = a % 10000 / 1000;
	cTInteger2 = a % 1000 / 100;
  cTDec = a %  100 / 10;
	return a/100;}