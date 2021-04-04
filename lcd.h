#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16 
#define u16 unsigned int
#endif


#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^7;
sbit LCD1602_RW=P2^5;
sbit LCD1602_RS=P2^6;

u8 launchMsg1[]="TemControl init";
u8 launchMsg2[]="welcome!";
u8 currentTemp[5]="123.5";
u8 destTemp[5]=" 89.0";
u8 mm[]="39";
u8 ss[]="25";
u8 stepingIcon[]={0x00,0x00,0x00,0x00,0x03,0x04,0x08,0x10,0x00,0x01,0x02,0x04,0x18,0x00,0x00,0x00}; //����ͼ��
u8 arrowRightIcon[]={0x00,0x00,0x04,0x02,0x1F,0x02,0x04,0x00}; //��ͷͼ��
u8 celsiusDegIcon[]={0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00}; //���϶�'Cͼ��
u8 keyDisableIcon[]={0x00,0x11,0x0A,0x04,0x0A,0x11,0x00,0x00}; //���ð�����ͼ�꣬����
u8 colonIcon[]={0x00,0x00,0x0C,0x0C,0x00,0x0C,0x0C,0x00}; //ð�ţ�ͼ�꣬��Ϊʱ��ķָ���
u8 arrowDownIcon[]={0x04,0x04,0x04,0x04,0x04,0x15,0x0E,0x04}; //�¼�ͷͼ�꣬��ʾ����
u8 arrowUpIcon[]={0x04,0x0E,0x15,0x04,0x04,0x04,0x04,0x04}; //�ϼ�ͷͼ�꣬��ʾ����


void Lcd1602_Delay1ms(u16 c){ //��ʱ������ms
  u8 a,b;
	for (; c>0; c--)	{
		 for (b=199;b>0;b--){
		  	for(a=1;a>0;a--);
		}      
	}  	
}


void LcdWriteCom(u8 com)	  { //д������
	LCD1602_E = 0;     //ʹ��
	LCD1602_RS = 0;	   //ѡ��������
	LCD1602_RW = 0;	   //ѡ��д��
	
	LCD1602_DATAPINS = com;     //��������
	Lcd1602_Delay1ms(1);		//�ȴ������ȶ�

	LCD1602_E = 1;	          //д��ʱ��
	Lcd1602_Delay1ms(5);	  //����ʱ��
	LCD1602_E = 0;
}


void LcdWriteData(u8 dat){ //д������
	LCD1602_E = 0;	//ʹ������
	LCD1602_RS = 1;	//ѡ����������
	LCD1602_RW = 0;	//ѡ��д��

	LCD1602_DATAPINS = dat; //д������
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //д��ʱ��
	Lcd1602_Delay1ms(5);   //����ʱ��
	LCD1602_E = 0;
}


void LcdInit()	  { //LCD��ʼ������
 	LcdWriteCom(0x38);  //����ʾ

	LcdWriteCom(0x0c);  //����ʾ����ʾ���

	LcdWriteCom(0x06);  //дһ��ָ���1

	LcdWriteCom(0x01);  //����

	LcdWriteCom(0x80);  //��������ָ�����
}



void setCursorPos(int row, int column){ //���ù��λ��
	u8 cursorPos;
	if(row==1){
		cursorPos = column -1 + 0x80;
	}
	if(row==2){
		cursorPos = 0x40 -1 + column + 0x80;
	}
	LcdWriteCom(cursorPos);
}

void clear(){LcdWriteCom(0x01);} //����


void addCostomIcon(){ //����Զ���ķ���
	u8 i;
	LcdWriteCom(0x40); //��������Զ���ͼ������

	for(i=0;i<8;i++){		LcdWriteData(celsiusDegIcon[i]);	} //������϶�'Cͼ��
	for(i=0;i<8;i++){		LcdWriteData(arrowRightIcon[i]);	} //��Ӽ�ͷͼ��
	for(i=0;i<8;i++){		LcdWriteData(arrowUpIcon[i]);	} //����ϼ�ͷͼ��
	for(i=0;i<8;i++){		LcdWriteData(arrowDownIcon[i]);	} //����¼�ͷͼ��
	for(i=0;i<8;i++){		LcdWriteData(colonIcon[i]);	} //���ð��ͼ��
	for(i=0;i<8;i++){		LcdWriteData(keyDisableIcon[i]);	} //��ӽ��ð�����ͼ��
	//��Ӳ���ͼ��
	for(i=0;i<8;i++){		LcdWriteData(stepingIcon[i]);	}
	for(i=8;i<16;i++){		LcdWriteData(stepingIcon[i]);	}
}


void showCelsiusDeg(){
	setCursorPos(1,6); //������ʾλ��
	LcdWriteData(0); //��ʾ
	
	setCursorPos(1,13); 
	LcdWriteData(0); 
}

void showArrowRight(){
	setCursorPos(1,7); 
	LcdWriteData(1);
}

void showArrowUp(){
	setCursorPos(1,16);
	LcdWriteData(2); 
}

void showArrowDown(){
	setCursorPos(1,16);
	LcdWriteData(3); 
}

void showColonIcon(){
	setCursorPos(2,3);
	LcdWriteData(4); 
}

void showKeyDisable(){
	setCursorPos(2,15);
	LcdWriteData(5); 
}

void showStepingIcon(){
	setCursorPos(2,12);
	LcdWriteData(6);
	
	setCursorPos(2,13);
	LcdWriteData(7);
}

void showTemp(){
	u8 i;
	
	setCursorPos(1,1);
	for(i=0; i<5; i++){
		LcdWriteData(currentTemp[i]);
	}
	
	setCursorPos(1,8);
	for(i=0; i<5; i++){
		LcdWriteData(destTemp[i]);
	}
}

void showTime(){
	u8 i;
	
	setCursorPos(2,1);
	for(i=0; i<2; i++){
		LcdWriteData(mm[i]);
	}
	
	setCursorPos(2,4);
	for(i=0; i<2; i++){
		LcdWriteData(ss[i]);
	}
}





void showLaunch(){ //��ʾ������
	u8 i;
	for(i=0;i<15;i++)	{
		LcdWriteData(launchMsg1[i]);	
	}
	setCursorPos(2,5);
	for(i=0;i<8;i++)	{
		LcdWriteData(launchMsg2[i]);	
	}
}
