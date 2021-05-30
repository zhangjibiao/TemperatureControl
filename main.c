#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reg52.h"	
#include "Ds18b20.h"
#include "lcd.h"
#include "button.h"




sbit heat=P2^0;

float Tem_old =0;
u8 mm=0,ss=0;

static  counter=0; // 放入静态区，使中断程序也能使用
static Time_Point2s=0;
static  i_CountTime=0;
int TempVariable;

void delay(int counter);
void Display();
float temParse(int a);
int KeyDown();
void timer0Init();
void timer1Init();
void temControlFun();
void stepingFun();
void DetectTem();

void setHeat(bit flag){
	if(flag==1){
		heat=0;}
	else{
		heat=1;}
}



void init(){
	setHeat(0); //因为串口默认是高电平，需要将其设为低电平
	
	timer0Init(); //计时器0初始化，采用中断，每0.2s获取一次温度
	LcdInit(); //LCD初始化
	
	showLaunch(); //显示启动屏并等1s
	Lcd1602_Delay1ms(1000); 
	
	clear(); //清屏，删除屏幕上的文字，并等待0.1s让LCD处理清屏命令
	Lcd1602_Delay1ms(100);
	
	addCostomIcon();// 添加自己定义的字符，比如步进图标、摄氏度‘C图标
	showCelsiusDeg();
	showArrowRight();
	showHyphen();

	setDesTemp(tem);
	showTemp();
}
void checkUpandDown(){
	if(up==0){	//检测温度加1按键是否被按下
		delay(1500);
		if(up==0){
			setDesTemp(floor(temTar)+1);
			showTemp();
			Lcd1602_Delay1ms(500);
			while(up==0){
					Lcd1602_Delay1ms(300);
					setDesTemp(temTar+1);
					showTemp();
			}
		}
	}
		
	if(down==0){	//检测温度减1按键是否被按下
		delay(1500);
		if(down==0){
			setDesTemp(ceil(temTar)-1);
			showTemp();
			Lcd1602_Delay1ms(500);
			while(down==0){
					Lcd1602_Delay1ms(300);
					setDesTemp(temTar-1);
					showTemp();
			}
		}
	}
}

void checkButtons(){
	if(temControl==0){//检测控温按键是否被按下
		delay(500);
		if(temControl==0) temControlFun();
	}
			 
	if(steping==0){	//检测步进按键是否被按下
		delay(500);
		if(steping==0) stepingFun();
	}
}

void main(){
	init();

	while(1){
		showTemp(); //温度需要更新
		checkButtons();
	}
}


float inputTem(){
	float tem1,tem2,tem3,tem4;
	setCursorPos(1,8);
	LcdWriteCom(0x0f);  //开显示、显示光标、光标闪烁
	
	tem1 = KeyDown();//输入百位数
	while(tem1>10){
		tem1 = KeyDown();
	}
	if (tem1==10){
		tem1 = 0;
	}
	LcdWriteData(tem1+0x30);

	tem2 = KeyDown();//输入十位数
	while(tem2>10){
		tem2 = KeyDown();
	}
	if (tem2==10){
		tem2 = 0;
	}
	LcdWriteData(tem2+0x30);
	
	tem3 = KeyDown();//输入个位数
	while(tem3>10){
		tem3 = KeyDown();
	}
	if (tem3==10){
		tem3 = 0;
	}
	LcdWriteData(tem3+0x30);
	LcdWriteData(0x2e);
	
	tem4 = KeyDown();//输入第一位小数数
	while(tem4>10){
		tem4 = KeyDown();
	}
	if (tem4==10){
		tem4 = 0;
	}	
	LcdWriteData(tem4+0x30); 
	
	return tem1*100 + tem2*10 + tem3 + tem4/10;
}





void temControlFun(){
	setDesTemp(inputTem());
	LcdWriteCom(0x0c);  //开显示、不显示光标、光标闪烁

	while(1){
		showTemp();
		checkUpandDown();	
		
		if(tem-temTar > -0.4){ //控温，控制继电器进而控制加热板
			showArrowDown();
			setHeat(0);	
		}else {
			showArrowUp();
			setHeat(1);
		}
	}
}

void stepingFun(){
	int desTem=floor(tem);
	showKeyDisable();
	showStepingIcon();
	
	while(1){
		//加热阶段
		desTem += 1;
		setDesTemp(desTem);
		setHeat(1);	
		showArrowUp();
		while(tem-temTar<=0){//等待加热完成
			showTemp();
		}
		
		//保持温度20s
		Time_Point2s=0;
		showHyphen();	
		while(Time_Point2s<=100){
			showTemp();
			if(tem-temTar>-0.4){
				setHeat(0);
			}else{
				setHeat(1);
			}
		}
	}
}
		


void DetectTem() interrupt 1{ //计数器0中断，计数器计完后会通知cpu执行任务，	0.2s查询一次温度
	counter=counter+1;
	TH0=0X00;
	TL0=0X00;
	
	if(counter==3){
		Time_Point2s=Time_Point2s+1;
		tem=temParse(Ds18b20ReadTemp());
		counter=0;
	}
}







void timer0Init(){ //计时器0初始化
	TMOD=0X11;
	TH0=0X00;
	TL0=0X00;	
	
	EA=1;
	ET0=1;
	TR0=1;
}


int KeyDown(){
	int KeyValue;   
	char a=0;
	P1=0x0f;
	while(1){
		if(P1!=0x0f) //确认按键被按下 
	    {delay(500);
		if(P1!=0x0f)
		 break;}
	}	 	
	
	//确定被按下的列
	P1=0X0F;
	switch(P1){
		case(0X07):	KeyValue=0;break;
		case(0X0b):	KeyValue=1;break;
		case(0X0d): KeyValue=2;break;
		case(0X0e):	KeyValue=3;break;}
	
	//确定被按下的行
	P1=0XF0;
	switch(P1){
		case(0X70):	KeyValue=KeyValue;break;
		case(0Xb0):	KeyValue=KeyValue+4;break;
		case(0Xd0): KeyValue=KeyValue+8;break; 
		case(0Xe0):	KeyValue=KeyValue+12;break;}
	
	//确定按键被释放
	while(P1!=0xf0){
		delay(100);
		a++;}
	
	return KeyValue+1;
}

void delay(int counter){
	while(counter--);}
