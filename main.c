/*本程序实现温度控制平台的设定目标温度、步进升温功能。
硬件：使用芯片STC89C52	P37口连接DS18B20温度传感器，P20口连接加热信号，当需要加热时，为高电平，否则为低电平。P0口连接8位8段共阴数码管，P22、P23、P24连接
74HC138译码器，P1口连接矩阵键盘，P30连接K1按键、P31连接K2按键
现象：
1,设定目标温度：按下K1按键，通过矩阵键盘输入目标温度，输入为16进制4位数，2进制补码，范围在-999~999,超过范围会报错，数码管显示E00。例如目标温度为32摄氏度，
需要输入0020.输入完成后左边4位数码管显示目标温度，右边4位数码管显示当前温度，当前温度仅能显示十位、各位。当目标温度高于当前温度，加热信号为高电平，加热。
2,步进升温。按下K2按键，自动加热至整数温度，停留10s左右，升温1度，再停10s左右，重复不止。

变量说明：Heat加热信号。K1，K2按键，按下时为0。LSA,LSB,LSC为译码器输入。Bit0至Bit7为8位数码管从左至右显示的数字、字符，16为不显示，17为负号
Tem为当前温度，Tem_old为上一次步进温度，TemTarget为目标温度，KeyValue为按下一次矩阵按键的值，i_temperature循环变量，TempVariable临时变量*/
//Bit4到Bit7是前四个数码管，是当前温度，十进制显示，


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