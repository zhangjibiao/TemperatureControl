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

	setDesTemp(tem);
	showTemp();
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
}

void main(){
	init();

	while(1){
		showTemp(); //�¶���Ҫ����
		checkButtons();
	}
}


float inputTem(){
	float tem1,tem2,tem3,tem4;
	setCursorPos(1,8);
	LcdWriteCom(0x0f);  //����ʾ����ʾ��ꡢ�����˸
	
	tem1 = KeyDown();//�����λ��
	while(tem1>10){
		tem1 = KeyDown();
	}
	if (tem1==10){
		tem1 = 0;
	}
	LcdWriteData(tem1+0x30);

	tem2 = KeyDown();//����ʮλ��
	while(tem2>10){
		tem2 = KeyDown();
	}
	if (tem2==10){
		tem2 = 0;
	}
	LcdWriteData(tem2+0x30);
	
	tem3 = KeyDown();//�����λ��
	while(tem3>10){
		tem3 = KeyDown();
	}
	if (tem3==10){
		tem3 = 0;
	}
	LcdWriteData(tem3+0x30);
	LcdWriteData(0x2e);
	
	tem4 = KeyDown();//�����һλС����
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
	LcdWriteCom(0x0c);  //����ʾ������ʾ��ꡢ�����˸

	while(1){
		showTemp();
		checkUpandDown();	
		
		if(tem-temTar > -0.4){ //���£����Ƽ̵����������Ƽ��Ȱ�
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
		//���Ƚ׶�
		desTem += 1;
		setDesTemp(desTem);
		setHeat(1);	
		showArrowUp();
		while(tem-temTar<=0){//�ȴ��������
			showTemp();
		}
		
		//�����¶�20s
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
		


void DetectTem() interrupt 1{ //������0�жϣ�������������֪ͨcpuִ������	0.2s��ѯһ���¶�
	counter=counter+1;
	TH0=0X00;
	TL0=0X00;
	
	if(counter==3){
		Time_Point2s=Time_Point2s+1;
		tem=temParse(Ds18b20ReadTemp());
		counter=0;
	}
}







void timer0Init(){ //��ʱ��0��ʼ��
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
	
	return KeyValue+1;
}

void delay(int counter){
	while(counter--);}
