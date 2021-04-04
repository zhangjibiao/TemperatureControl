#include <math.h>
#include <stdio.h>
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
float tem =0;
float Tem_old =0;
short TemTarget;
int KeyValue;   
static  counter=0; // 放入静态区，使中断程序也能使用
static  i_CountTime=0;
int TempVariable;

void delay(int counter);
void Display();
float temParse(int a);
int KeyDown();
void timer0Init();
void timer1Init();
void Heat2TargetTem();
void AutoSet();
void DetectTem();

void setHeat(bit flag){
	if(flag==1)
		Heat=1;
	else
		Heat=0;
}

void init(){
	setHeat(0); //因为串口默认是高电平，需要将其设为低电平
	
	timer0Init(); //计时器0初始化，采用中断，每0.2s获取一次温度
	lcdInit(); //LCD初始化
	
	showLaunch(); //显示启动屏并等1s
	Lcd1602_Delay1ms(1000); 
	
	clear(); //清屏，删除屏幕上的文字，并等待0.1s让LCD处理清屏命令
	Lcd1602_Delay1ms(100);
	
	addCostomIcon();
	showCelsiusDeg();
	showArrowRight();
	showArrowDown();
	showColonIcon();
	showKeyDisable();
	showStepingIcon();
	showTemp();
	showTime();
	
	while(1){
		showCurTemp();
		Lcd1602_Delay1ms(100);//等1s刷新屏幕
		sprintf(currentTemp, "%5.1f", tem);
	}
}


void main(){
	init();
}

void Heat2TargetTem(){
	fun1=1;
	
	//Time init 
	timer0Init();
	timer1Init();
	
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
	timer1Init();
	timer0Init();
	
	//adjust target temperature to be integer
	while(i_CountTime--){
		counter=2;
		TF0=1;
		TemTarget=(int)ceil(tem)+1;}

	//display target temperature

			
	while(1){
		//waitingphase
		Heat=0;
		
		//maintain temperature stage
		if(tem-TemTarget>-0.2)
			Heat=1;
		if(tem-TemTarget>0.2)
			Heat=0;

		if(i_CountTime>=1000){
			//Heatingphase
			Heat=1;
			TemTarget++;
			
			//display target temperature

			
			while(TemTarget>tem)i_CountTime=0;}}}
		


void DetectTem() interrupt 1{ //计数器0中断，计数器计完后会通知cpu执行任务，	0.2s查询一次温度
	counter=counter+1;
	TH0=0X00;
	TL0=0X00;
	
	if(counter==3){
		tem=temParse(Ds18b20ReadTemp());
		counter=0;}
}



void timer0Init(){ //计时器0初始化
	TMOD=0X11;
	TH0=0X00;
	TL0=0X00;	
	
	EA=1;
	ET0=1;
	TR0=1;
}

void timer1Init(){ //计时器0初始化
	TMOD=0X11;
	TH1=0XD8;
	TL1=0XF0;	
	
	EA=1;
	ET1=1;
	TR1=1;
}

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

void delay(int counter){
	while(counter--);}
