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

static  counter=0; // ���뾲̬����ʹ�жϳ���Ҳ��ʹ��
static u8 Timer1Counter;
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
	if(flag==1)
		heat=1;
	else
		heat=0;
}

void heatControl(){
	if(tem-temTar > 0.2){ //���£����Ƽ̵����������Ƽ��Ȱ�
			showArrowDown();
			setHeat(0);			
		}else if(temTar-tem > 0.2){
			showArrowUp(); 
			setHeat(1);
		}else {
			showHyphen(); 
			setHeat(0);
		}
}

void init(){
	setHeat(0); //��Ϊ����Ĭ���Ǹߵ�ƽ����Ҫ������Ϊ�͵�ƽ
	
	timer0Init(); //��ʱ��0��ʼ���������жϣ�ÿ0.2s��ȡһ���¶�
	LcdInit(); //LCD��ʼ��
	
	showLaunch(); //��ʾ����������1s
	Lcd1602_Delay1ms(1000); 
	
	clear(); //������ɾ����Ļ�ϵ����֣����ȴ�0.1s��LCD������������
	Lcd1602_Delay1ms(100);
	
	addCostomIcon();// ����Լ�������ַ������粽��ͼ�ꡢ���϶ȡ�Cͼ��
	showCelsiusDeg();
	showArrowRight();
	showHyphen();
	showColonIcon();

	setDesTemp(tem);
	showTemp();
	showTime();
}
void checkUpandDown(){
	if(up==0){	//����¶ȼ�1�����Ƿ񱻰���
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
		
	if(down==0){	//����¶ȼ�1�����Ƿ񱻰���
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
	if(temControl==0){//�����°����Ƿ񱻰���
		delay(500);
		if(temControl==0) temControlFun();
	}
			 
	if(steping==0){	//��ⲽ�������Ƿ񱻰���
		delay(500);
		if(steping==0) stepingFun();
	}
	
	checkUpandDown();	
}

void main(){
	init();
	
	while(1){
		showTemp();
		checkButtons();
		heatControl();
	}
}


float inputTem(){
	float tem1,tem2,tem3,tem4;
	setCursorPos(1,8);
	LcdWriteCom(0x0f);  //����ʾ����ʾ��ꡢ�����˸
	
	tem1 = KeyDown();
	LcdWriteData(tem1+0x30);

	tem2 = KeyDown();
	LcdWriteData(tem2+0x30);
	
	tem3 = KeyDown();
	LcdWriteData(tem3+0x30);
	LcdWriteData(0x2e);
	
	tem4 = KeyDown();
	LcdWriteData(tem4+0x30); 
	
	return tem1*100 + tem2*10 + tem3 + tem4/10;
}

void setStopWatch(){
	sprintf(mmStr, "%2.2d", mm);
	sprintf(ssStr, "%2.2d", ss);
}



void temControlFun(){
	setDesTemp(inputTem());
	LcdWriteCom(0x0c);  //����ʾ������ʾ��ꡢ�����˸
	//showTemp();
	
	//timer1Init();
			
		//mm=20;
		//ss=05;
		//setStopWatch();
	strcpy(mmStr,"20");
	strcpy(ssStr," 2");
//showArrowDown();
	while(1){
		
		if(tem-temTar > 0.2){ //���£����Ƽ̵����������Ƽ��Ȱ�
			showArrowDown(); setHeat(0);			
		}else if(temTar-tem > 0.2){
			showArrowUp(); setHeat(1);
		}else showHyphen(); setHeat(0);
		
		showTemp();
		checkUpandDown();
		showTime();		
	}
}

void stepingFun(){
	int i_CountTime=20;

	timer1Init();
	timer0Init();
	
	//adjust target temperature to be integer
	while(i_CountTime--){
		counter=2;
		TF0=1;
		temTar=(int)ceil(tem)+1;}

	//display target temperature

			
	while(1){
		//waitingphase
		heat=0;
		
		//maintain temperature stage
		if(tem-temTar>-0.2)
			heat=1;
		if(tem-temTar>0.2)
			heat=0;

		if(i_CountTime>=1000){
			//Heatingphase
			heat=1;
			temTar++;
			
			//display target temperature

			
			while(temTar>tem)i_CountTime=0;}}}
		


void DetectTem() interrupt 1{ //������0�жϣ�������������֪ͨcpuִ������	0.2s��ѯһ���¶�
	counter=counter+1;
	TH0=0X00;
	TL0=0X00;
	
	if(counter==3){
		tem=temParse(Ds18b20ReadTemp());
		counter=0;}
}

void timeAdd1s(){
	if(ss==59){
		ss=0;
		mm += 1;
		if(mm==60){
			mm=0;
			ss=0;
		}
	}else ss += 1;
}



//void Timer1() interrupt 3{ //������1�жϣ��������
//	Timer1Counter=Timer1Counter+1;
//	//TH0=0X00;
//	//TL0=0X00;
//	
//	if(counter==16){
//		TH1=0XBD;
//		TL1=0XC0;	
//		timeAdd1s();
//		setStopWatch();
//		counter=0;		
//	}
//}



void timer0Init(){ //��ʱ��0��ʼ��
	TMOD=0X11;
	TH0=0X00;
	TL0=0X00;	
	
	EA=1;
	ET0=1;
	TR0=1;
}

void timer1Init(){ //��ʱ��1��ʼ��
	TMOD=0X11;
	TH1=0XBD;
	TL1=0XC0;	
	
	EA=1;
	ET1=1;
	TR1=1;
}

int KeyDown(){
	int KeyValue;   
	char a=0;
	P1=0x0f;
	while(1){
		if(P1!=0x0f) //ȷ�ϰ��������� 
	    {delay(500);
		if(P1!=0x0f)
		 break;}
	}	 	
	
	//ȷ�������µ���
	P1=0X0F;
	switch(P1){
		case(0X07):	KeyValue=0;break;
		case(0X0b):	KeyValue=1;break;
		case(0X0d): KeyValue=2;break;
		case(0X0e):	KeyValue=3;break;}
	
	//ȷ�������µ���
	P1=0XF0;
	switch(P1){
		case(0X70):	KeyValue=KeyValue;break;
		case(0Xb0):	KeyValue=KeyValue+4;break;
		case(0Xd0): KeyValue=KeyValue+8;break; 
		case(0Xe0):	KeyValue=KeyValue+12;break;}
	
	//ȷ���������ͷ�
	while(P1!=0xf0){
		delay(100);
		a++;}
	
	return KeyValue;
}

void delay(int counter){
	while(counter--);}
