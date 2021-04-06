#ifndef u1
#define u1 bit
#endif

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
u8 mmStr[]="00";
u8 ssStr[]="50";
u8 stepingIcon[]={0x00,0x00,0x00,0x00,0x03,0x04,0x08,0x10,0x00,0x01,0x02,0x04,0x18,0x00,0x00,0x00}; //步进图标
u8 arrowRightIcon[]={0x00,0x00,0x04,0x02,0x1F,0x02,0x04,0x00}; //箭头图标
u8 celsiusDegIcon[]={0x10,0x06,0x09,0x08,0x08,0x09,0x06,0x00}; //摄氏度'C图标
u8 keyDisableIcon[]={0x00,0x11,0x0A,0x04,0x0A,0x11,0x00,0x00}; //禁用按键的图标，×号
u8 colonIcon[]={0x00,0x00,0x0C,0x0C,0x00,0x0C,0x0C,0x00}; //冒号：图标，作为时间的分隔符
u8 arrowDownIcon[]={0x04,0x04,0x04,0x04,0x04,0x15,0x0E,0x04}; //下箭头图标，表示降温
u8 arrowUpIcon[]={0x04,0x0E,0x15,0x04,0x04,0x04,0x04,0x04}; //上箭头图标，表示升温


void Lcd1602_Delay1ms(u16 c){ //延时函数，ms
  u8 a,b;
	for (; c>0; c--)	{
		 for (b=199;b>0;b--){
		  	for(a=1;a>0;a--);
		}      
	}  	
}


void LcdWriteCom(u8 com)	  { //写入命令
	LCD1602_E = 0;     //使能
	LCD1602_RS = 0;	   //选择发送命令
	LCD1602_RW = 0;	   //选择写入
	
	LCD1602_DATAPINS = com;     //放入命令
	Lcd1602_Delay1ms(1);		//等待数据稳定

	LCD1602_E = 1;	          //写入时序
	Lcd1602_Delay1ms(5);	  //保持时间
	LCD1602_E = 0;
}


void LcdWriteData(u8 dat){ //写入数据
	LCD1602_E = 0;	//使能清零
	LCD1602_RS = 1;	//选择输入数据
	LCD1602_RW = 0;	//选择写入

	LCD1602_DATAPINS = dat; //写入数据
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //写入时序
	Lcd1602_Delay1ms(5);   //保持时间
	LCD1602_E = 0;
}






void setCursorPos(char row, char column){ //设置光标位置
	u8 cursorPos;
	if(row==1){
		cursorPos = column -1 + 0x80;
	}
	if(row==2){
		cursorPos = 0x40 -1 + column + 0x80;
	}
	LcdWriteCom(cursorPos);
}

void setDesTemp(float temp){ //将传入的温度（浮点数）设置为目标温度
	temTar = temp;
	sprintf(destTemp, "%5.1f", temTar);
}

void clear(){LcdWriteCom(0x01);} //清屏


void addCostomIcon(){ //添加自定义的符号
	u8 i;
	LcdWriteCom(0x40); //发送添加自定义图标命令

	for(i=0;i<8;i++){		LcdWriteData(celsiusDegIcon[i]);	} //添加摄氏度'C图标
	for(i=0;i<8;i++){		LcdWriteData(arrowRightIcon[i]);	} //添加箭头图标
	for(i=0;i<8;i++){		LcdWriteData(arrowUpIcon[i]);	} //添加上箭头图标
	for(i=0;i<8;i++){		LcdWriteData(arrowDownIcon[i]);	} //添加下箭头图标
	for(i=0;i<8;i++){		LcdWriteData(colonIcon[i]);	} //添加冒号图标
	for(i=0;i<8;i++){		LcdWriteData(keyDisableIcon[i]);	} //添加禁用按键的图标
	//添加步进图标
	for(i=0;i<8;i++){		LcdWriteData(stepingIcon[i]);	}
	for(i=8;i<16;i++){		LcdWriteData(stepingIcon[i]);	}
}


void showCelsiusDeg(){
	setCursorPos(1,6); //设置显示位置
	LcdWriteData(0); //显示
	
	setCursorPos(1,13); 
	LcdWriteData(0); 
}

void showArrowRight(){
	setCursorPos(1,7); 
	LcdWriteData(1);
}

void showHyphen(){ //显示连字符 - ，代表处于稳定状态，不加热也不降温
	setCursorPos(1,16);
	LcdWriteData(0x2d);
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

void showCurTemp(){
	u8 i;
	
	setCursorPos(1,1);
	sprintf(currentTemp, "%5.1f", tem); //更新当前温度值
	for(i=0; i<5; i++){
		LcdWriteData(currentTemp[i]);
	}
}

void showTemp(){
	u8 i;
	showCurTemp();
	
	setCursorPos(1,8);
	for(i=0; i<5; i++){
		LcdWriteData(destTemp[i]);
	}
}

void showTime(){
	u8 i;
	
	setCursorPos(2,1);
	for(i=0; i<2; i++){
		LcdWriteData(mmStr[i]);
	}
	
	setCursorPos(2,4);
	for(i=0; i<2; i++){
		LcdWriteData(ssStr[i]);
	}
}





void showLaunch(){ //显示启动屏
	u8 i;
	for(i=0;i<15;i++)	{
		LcdWriteData(launchMsg1[i]);	
	}
	setCursorPos(2,5);
	for(i=0;i<8;i++)	{
		LcdWriteData(launchMsg2[i]);	
	}
}

void showMsg(u8 row, u8 column, u8 msg){//传入1个字符串，显示到指定位置
	setCursorPos(row,column);
	LcdWriteData(msg);
}

void LcdInit()	  { //LCD初始化程序
 	LcdWriteCom(0x38);  //开显示

	LcdWriteCom(0x0c);  //开显示不显示光标

	LcdWriteCom(0x06);  //写一个指针加1

	LcdWriteCom(0x01);  //清屏

	LcdWriteCom(0x80);  //设置数据指针起
}

