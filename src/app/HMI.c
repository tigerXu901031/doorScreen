#include "../app/HMI.H"
#include "../drv/19296p1.H"


Machine_Parameters_Def Motor_Parameter;

uchar char_num[16];

void Display_Full_Line(uchar ucLine, uchar ucUpDown)
{
	uchar i;
	for(i=1;i<25;i++)
	{
		if(ucUpDown)	//up
		{
			display_8x8((i-1)*8+1,ucLine,Char0808_Up);
		}
		else					//Down
			{
				display_8x8((i-1)*8+1,ucLine,Char0808_Down);
			}
	}
}

void Display_Full_Vertical_Line(uchar line)
{
	uchar i;
	for(i=1;i<13;i++)
		{display_8x1(line,i,0xFF);}
}

void BUTTON_Display(uchar x, uchar HanziButton)		//Button Display
{
	if(x == 1)
		{
			switch(HanziButton)
			{
				case Hanzi_Fanxiang:
					Hanzi_Disp_16x16(1, 11, Hanzi_1616_XST_Fan1, 0x07);	//0000 0111
					Hanzi_Disp_16x16(17, 11, Hanzi_1616_XST_Xiang, 0x0B); //0000 1011
					break;
				case Hanzi_Zhengxiang:
					Hanzi_Disp_16x16(1, 11, Hanzi_1616_XST_Zheng, 0x07);	//0000 0100
					Hanzi_Disp_16x16(17, 11, Hanzi_1616_XST_Xiang, 0x0B); //00001000
					break;
				case Hanzi_Fanhui:
					Hanzi_Disp_16x16(1, 11, Hanzi_1616_XST_Fan, 0x07);	//0000 0100
					Hanzi_Disp_16x16(17, 11, Hanzi_1616_XST_Hui, 0x0B); //00001000
					break;
				case Hanzi_Xuexi:
					Hanzi_Disp_16x16(1, 11, Hanzi_1616_XST_Xue, 0x07);	//0000 0100
					Hanzi_Disp_16x16(17, 11, Hanzi_1616_XST_Xi, 0x0B); //00001000
					break;
				default: break;
			}
		}
		else {;}
	if(x == 2)
		{
			switch(HanziButton)
			{
				case Hanzi_Queren:
					Hanzi_Disp_16x16(161, 11, Hanzi_1616_XST_Que, 0x07);	//0000 0111
					Hanzi_Disp_16x16(177, 11, Hanzi_1616_XST_Ren, 0x0B); //0000 1011
					break;
				case Hanzi_Caidan:
					Hanzi_Disp_16x16(161, 11, Hanzi_1616_XST_Cai, 0x07);	//0000 0100
					Hanzi_Disp_16x16(177, 11, Hanzi_1616_XST_Dan, 0x0B); //00001000
					break;
				default: break;
			}
		}
		else {;}
}

void Number_Lookup_Table(uchar number)
{
		uchar i;
		for(i=0;i<16;i++)
		{char_num[i] = char_number_table[number*16+i];}
}

void Display_Encoder_Number(uchar x)
{
	uint encoder;
	uchar getDataBufH, getDataBufL;
	//	p1 = &getDataBufH;
	//	p2 = &getDataBufL;
	//void getNetworkData(unsigned char addL, unsigned char addH, unsigned char *dataL, unsigned char *dataH)
	//getNetworkData(MonitorPara_Position_AddrL, MonitorPara_Position_AddrH, &getDataBufL, &getDataBufH);
	Motor_Parameter.E_encoder = (getDataBufH >> 8) + getDataBufL;
	encoder = Motor_Parameter.E_encoder;
	encoder = 63408;		//example for display demo only
	
	if(encoder>10000)			//���λ
		{
			Motor_Parameter.E_Digit1 = encoder/10000;
			encoder %= 10000;	
		}
		else {Motor_Parameter.E_Digit1 = 0;}
		
	if(encoder>1000)		//��2λ
		{
			Motor_Parameter.E_Digit2 = encoder/1000;
			encoder %= 1000;	
		}
		else {Motor_Parameter.E_Digit2 = 0;}
		
	if(encoder>100)		//��3λ
		{
			Motor_Parameter.E_Digit3 = encoder/100;
			encoder %= 100;	
		}
		else {Motor_Parameter.E_Digit3 = 0;}
		
	if(encoder>10)		//��4λ
		{
			Motor_Parameter.E_Digit4 = encoder/10;
			encoder %= 10;	
		}
		else {Motor_Parameter.E_Digit4 = 0;}
			
	Motor_Parameter.E_Digit5 = encoder;		//��5λ
	
	Number_Lookup_Table(Motor_Parameter.E_Digit1);
	if(x==1)
		{
			CHAR_Display_16x8(57, 1,char_num, 1);
		}
		else if(x==2)
			{
				CHAR_Display_16x8(57, 2,char_num, 0);
			}
				else{;}
	
	Number_Lookup_Table(Motor_Parameter.E_Digit2);
	if(x==1)
		{
			CHAR_Display_16x8(57, 1,char_num, 1);
		}
		else if(x==2)
			{
				CHAR_Display_16x8(65, 2,char_num, 0);
			}
				else{;}	

	Number_Lookup_Table(Motor_Parameter.E_Digit3);
	if(x==1)
		{
			CHAR_Display_16x8(73, 1,char_num, 1);
		}
		else if(x==2)
			{
				CHAR_Display_16x8(73, 2,char_num, 0);
			}
				else{;}	

	Number_Lookup_Table(Motor_Parameter.E_Digit4);
	if(x==1)
		{
			CHAR_Display_16x8(81, 1,char_num, 1);
		}
		else if(x==2)
			{
				CHAR_Display_16x8(81, 2,char_num, 0);
			}
				else{;}		

	Number_Lookup_Table(Motor_Parameter.E_Digit5);
	if(x==1)
		{
			CHAR_Display_16x8(89, 1,char_num, 1);
		}
		else if(x==2)
			{
				CHAR_Display_16x8(89, 2,char_num, 0);
			}
				else{;}		 
}

void Display_RunTime(void)
{
	uint runtime;
	uchar getDataBufH, getDataBufL;
	//	p1 = &getDataBufH;
	//	p2 = &getDataBufL;
	//void getNetworkData(unsigned char addL, unsigned char addH, unsigned char *dataL, unsigned char *dataH)
	//getNetworkData(FuncPara_AutoCloseDoor_DelayT_AddrL, FuncPara_AutoCloseDoor_DelayT_AddrH, &getDataBufL, &getDataBufH);
	Motor_Parameter.RT_runtime = (getDataBufH >> 8) + getDataBufL;
	runtime = Motor_Parameter.RT_runtime;
	runtime = 12356;		//example for display demo only
	
	if(runtime>10000)			//���λ
		{
			Motor_Parameter.RT_Digit1 = runtime/10000;
			runtime %= 10000;	
		}
		else {Motor_Parameter.RT_Digit1 = 0;}
		
	if(runtime>1000)		//��2λ
		{
			Motor_Parameter.RT_Digit2 = runtime/1000;
			runtime %= 1000;	
		}
		else {Motor_Parameter.RT_Digit2 = 0;}
		
	if(runtime>100)		//��3λ
		{
			Motor_Parameter.RT_Digit3 = runtime/100;
			runtime %= 100;	
		}
		else {Motor_Parameter.RT_Digit3 = 0;}
		
	if(runtime>10)		//��4λ
		{
			Motor_Parameter.RT_Digit4 = runtime/10;
			runtime %= 10;	
		}
		else {Motor_Parameter.RT_Digit4 = 0;}
			
	Motor_Parameter.RT_Digit5 = runtime;		//��5λ
	
	Number_Lookup_Table(Motor_Parameter.RT_Digit1);
	CHAR_Display_16x8(57, 3,char_num, 0); 

	Number_Lookup_Table(Motor_Parameter.RT_Digit2);
	CHAR_Display_16x8(65, 3,char_num, 0); 

	Number_Lookup_Table(Motor_Parameter.RT_Digit3);
	CHAR_Display_16x8(73, 3,char_num, 0); 

	Number_Lookup_Table(Motor_Parameter.RT_Digit4);
	CHAR_Display_16x8(81, 3,char_num, 0); 

	Number_Lookup_Table(Motor_Parameter.RT_Digit5);
	CHAR_Display_16x8(89, 3,char_num, 0); 		
}

void Display_I_current(void)
{
	uint current_data;
	uchar getDataBufH, getDataBufL;
	//	p1 = &getDataBufH;
	//	p2 = &getDataBufL;
	//void getNetworkData(unsigned char addL, unsigned char addH, unsigned char *dataL, unsigned char *dataH)
	//getNetworkData(MonitorPara_CurrentFBK_AddrL, MonitorPara_CurrentFBK_AddrH, &getDataBufL, &getDataBufH);
	Motor_Parameter.I_current = (getDataBufH >> 8) + getDataBufL;
	current_data = Motor_Parameter.I_current;
	current_data = 12345;		//example for display demo only
	
	if(current_data>10000)			//���λ
		{
			Motor_Parameter.I_Digit1 = current_data/10000;
			current_data %= 10000;	
		}
		else {Motor_Parameter.I_Digit1 = 0;}
		
	if(current_data>1000)		//��2λ
		{
			Motor_Parameter.I_Digit2 = current_data/1000;
			current_data %= 1000;	
		}
		else {Motor_Parameter.I_Digit2 = 0;}
		
	if(current_data>100)		//��3λ
		{
			Motor_Parameter.I_Digit3 = current_data/100;
			current_data %= 100;	
		}
		else {Motor_Parameter.I_Digit3 = 0;}
		
	if(current_data>10)		//��4λ
		{
			Motor_Parameter.I_Digit4 = current_data/10;
			current_data %= 10;	
		}
		else {Motor_Parameter.I_Digit4 = 0;}
			
	Motor_Parameter.I_Digit5 = current_data;		//��5λ
	
	Number_Lookup_Table(Motor_Parameter.I_Digit1);
	CHAR_Display_16x8(129, 1,char_num, 1); 			//���Ϻ�

	Number_Lookup_Table(Motor_Parameter.I_Digit2);
	CHAR_Display_16x8(137, 1,char_num, 1); 			//���Ϻ� 

	Number_Lookup_Table(Motor_Parameter.I_Digit3);
	CHAR_Display_16x8(145, 1,char_num, 1); 			//���Ϻ� 

	Number_Lookup_Table(Motor_Parameter.I_Digit4);
	CHAR_Display_16x8(161, 1,char_num, 1); 			//���Ϻ� 

	Number_Lookup_Table(Motor_Parameter.I_Digit5);
	CHAR_Display_16x8(169, 1,char_num, 1); 			//���Ϻ� 
}

void Display_F_frequency(void)
{
	uint frequency_data;
	Motor_Parameter.F_frequency = 62503;

	frequency_data = Motor_Parameter.F_frequency;

	if(frequency_data>10000)			//���λ
		{
			Motor_Parameter.F_Digit1 = frequency_data/10000;
			frequency_data %= 10000;	
		}
		else {Motor_Parameter.F_Digit1 = 0;}
		
	if(frequency_data>1000)		//��2λ
		{
			Motor_Parameter.F_Digit2 = frequency_data/1000;
			frequency_data %= 1000;	
		}
		else {Motor_Parameter.F_Digit2 = 0;}
		
	if(frequency_data>100)		//��3λ
		{
			Motor_Parameter.F_Digit3 = frequency_data/100;
			frequency_data %= 100;	
		}
		else {Motor_Parameter.F_Digit3 = 0;}
		
	if(frequency_data>10)		//��4λ
		{
			Motor_Parameter.F_Digit4 = frequency_data/10;
			frequency_data %= 10;	
		}
		else {Motor_Parameter.F_Digit4 = 0;}
			
	Motor_Parameter.F_Digit5 = frequency_data;		//��5λ
	
	Number_Lookup_Table(Motor_Parameter.F_Digit1);
	CHAR_Display_16x8(129, 3,char_num, 0); 			//���Ϻ�

	Number_Lookup_Table(Motor_Parameter.F_Digit2);
	CHAR_Display_16x8(137, 3,char_num, 0); 			//���Ϻ� 

	Number_Lookup_Table(Motor_Parameter.F_Digit3);
	CHAR_Display_16x8(145, 3,char_num, 0); 			//���Ϻ� 

	Number_Lookup_Table(Motor_Parameter.F_Digit4);
	CHAR_Display_16x8(161, 3,char_num, 0); 			//���Ϻ� 

	Number_Lookup_Table(Motor_Parameter.F_Digit5);
	CHAR_Display_16x8(169, 3,char_num, 0); 			//���Ϻ� 
}

void Motor_Direction_Update(void)
{
	if(Motor_Parameter.Motor_Direction == 1)
		{BUTTON_Display(1, Hanzi_Fanxiang);}
		else
			{	BUTTON_Display(1, Hanzi_Zhengxiang);}
}

void Display_Page00_00_ParaArea(void)		//Page00_00���������
{
	Hanzi_Disp_16x16(1, 2, Hanzi_1616_XST_Bian, 0x04);	//0000 0100
	Hanzi_Disp_16x16(17, 2, Hanzi_1616_XST_Ma, 0);	//0000 0100
	Hanzi_Disp_16x16(33, 2, Hanzi_1616_XST_Qi, 0);	//0000 0100
	CHAR_Display_16x8(49,2,char_cMaohao, 0);
}

void Display_Page00_01_ParaArea(void)		//Page00_01���������
{
	Display_Page00_00_ParaArea();
	CHAR_Display_16x8(113,1,char_capital_I, 1);
	CHAR_Display_16x8(121,1,char_cMaohao, 1);
	CHAR_Display_16x8(153,1,char_cDot, 1);
	CHAR_Display_16x8(113,3,char_capital_F, 0);
	CHAR_Display_16x8(121,3,char_cMaohao, 0);
	CHAR_Display_16x8(153,3,char_cDot, 0);
	CHAR_Display_16x8(177,1,char_capital_A, 1);
	CHAR_Display_16x8(177,3,char_capital_H, 0);
	CHAR_Display_16x8(185,3,char_z, 0);
}

void Display_Page00_03_ParaArea(void)	//��ʾ�̶���Ϣ
{
	Hanzi_Disp_16x16(1, 1, Hanzi_1616_XST_Bian, 1);	
	Hanzi_Disp_16x16(17, 1, Hanzi_1616_XST_Ma, 1);	
	Hanzi_Disp_16x16(33, 1, Hanzi_1616_XST_Qi, 1);	
	CHAR_Display_16x8(49,1,char_cMaohao, 1);
	Hanzi_Disp_16x16(1, 3, Hanzi_1616_XST_Yun, 0);	
	Hanzi_Disp_16x16(17, 3, Hanzi_1616_XST_Xing, 0);	
	Hanzi_Disp_16x16(33, 3, Hanzi_1616_XST_Shu, 0);	
	CHAR_Display_16x8(49,3,char_cMaohao, 0);
	
	CHAR_Display_16x8(113,1,char_capital_I, 1);
	CHAR_Display_16x8(121,1,char_cMaohao, 1);
	CHAR_Display_16x8(153,1,char_cDot, 1);
	CHAR_Display_16x8(177,1,char_capital_A, 1);
	CHAR_Display_16x8(113,3,char_capital_F, 0);
	CHAR_Display_16x8(121,3,char_cMaohao, 0);
	CHAR_Display_16x8(153,3,char_cDot, 0);
	CHAR_Display_16x8(177,3,char_capital_H, 0);
	CHAR_Display_16x8(185,3,char_z, 0);
	}

void Display_Page00_00_MotorDirt(void)	//Display "电机方向"
{
	disp_24x24(49,6,Hanzi_2424_HWZS_Dian);
	disp_24x24(73,6,Hanzi_2424_HWZS_Ji);
	disp_24x24(97,6,Hanzi_2424_HWZS_Fang);
	disp_24x24(121,6,Hanzi_2424_HWZS_Xiang);
}

void Display_Page00_03_CDDT(void)	//Display "close door delay time"
{
	disp_24x24(1,6,Hanzi_2424_HWZS_Guan);
	disp_24x24(25,6,Hanzi_2424_HWZS_Men);
	disp_24x24(49,6,Hanzi_2424_HWZS_Yan);
	disp_24x24(73,6,Hanzi_2424_HWZS_Shi);
}

void Display_Page00_01_LLStudy(void)		//����λѧϰ
{
	disp_24x24(33,6,Hanzi_2424_HWZS_Xia);
	disp_24x24(57,6,Hanzi_2424_HWZS_Xian);
	disp_24x24(81,6,Hanzi_2424_HWZS_Wei);
	disp_24x24(105,6,Hanzi_2424_HWZS_Xue);
	disp_24x24(129,6,Hanzi_2424_HWZS_Xi);
//	disp_96x24(49,6,Hanzi_9624_HWZS_DJFX);
}

void Display_Page00_01_ULStudy(void)		//����λѧϰ
{
	disp_24x24(33,6,Hanzi_2424_HWZS_Shang);
	disp_24x24(57,6,Hanzi_2424_HWZS_Xian);
	disp_24x24(81,6,Hanzi_2424_HWZS_Wei);
	disp_24x24(105,6,Hanzi_2424_HWZS_Xue);
	disp_24x24(129,6,Hanzi_2424_HWZS_Xi);
//	disp_96x24(49,6,Hanzi_9624_HWZS_DJFX);
}

void Display_Page00_03_ZDKJ(void)			//�µ�Ƽ�
{
	disp_24x24(49,6,Hanzi_2424_HWZS_Zhi);
	disp_24x24(73,6,Hanzi_2424_HWZS_Dian);
	disp_24x24(97,6,Hanzi_2424_HWZS_Ke);
	disp_24x24(121,6,Hanzi_2424_HWZS_Ji1);
//	disp_96x24(49,6,Hanzi_9624_HWZS_DJFX);
}

void Disp_Page_MotorDirtSet(void)		// Motor Direction Setting
{
	clear_screen();
	Display_Full_Line(1, Up);
	Display_Full_Line(5, Up);
	Display_Full_Line(12, Down);
	Display_Full_Vertical_Line(1);
	Display_Full_Vertical_Line(192);
	Display_Page00_00_ParaArea();	

	BUTTON_Display(1, Hanzi_Fanxiang);
//	Motor_Direction_Update();
	BUTTON_Display(2, Hanzi_Queren);		
	
	Display_Page00_00_MotorDirt(); 
//	Motor_Parameter.E_encoder = 00001;
//	getdata(MonitorPara_Position_AddrH,MonitorPara_Position_AddrL);
	Display_Encoder_Number(2);
}

void Disp_Page_DownLMT_STD(void)		//����λѧϰ
{
	clear_screen();
	Display_Full_Line(1, Up);
	Display_Full_Line(5, Up);
	
	Display_Page00_01_ParaArea();	//��ʾ�̶���Ϣ
	
	Display_Full_Line(12, Down);
	Display_Full_Vertical_Line(1);
	Display_Full_Vertical_Line(192);
	
	Display_Page00_01_LLStudy();
	
	BUTTON_Display(1, Hanzi_Fanhui);
	BUTTON_Display(2, Hanzi_Queren);		

}

void Disp_Page_UpLMT_STD(void)		//����λѧϰ
{
	clear_screen();
	Display_Full_Line(1, Up);
	Display_Full_Line(5, Up);

	Display_Page00_01_ParaArea();	//��ʾ�̶���Ϣ
	
	Display_Full_Line(12, Down);
	Display_Full_Vertical_Line(1);
	Display_Full_Vertical_Line(192);
	
	Display_Page00_01_ULStudy();
	
	BUTTON_Display(1, Hanzi_Fanhui);
	BUTTON_Display(2, Hanzi_Queren);		
}

void Disp_Page_CloseDoor_DelayT(void)
{
	clear_screen();
	Display_Full_Line(1, Up);
	Display_Full_Line(5, Up);
	Display_Page00_03_ParaArea();
	
	Display_Full_Line(12, Down);
	Display_Page00_03_CDDT();
	Display_Full_Vertical_Line(1);
	Display_Full_Vertical_Line(192);
	
	BUTTON_Display(1, Hanzi_Fanhui);
	BUTTON_Display(2, Hanzi_Queren);	
}

void Disp_Page_RunningMode(void)
{
	Display_Full_Line(1, Up);
	Display_Full_Line(5, Up);
	Motor_Parameter.RT_runtime = 00002;
	
	Display_Page00_03_ParaArea();	//��ʾ�̶���Ϣ
	
	
	Display_Full_Line(12, Down);
	Display_Full_Vertical_Line(1);
	Display_Full_Vertical_Line(192);
	Display_Page00_03_ZDKJ();
	
	BUTTON_Display(1, Hanzi_Xuexi);
	BUTTON_Display(2, Hanzi_Caidan);		

}


void Disp_Page_01_00(void)
{
	Display_Full_Line(1, Up);
	Display_Full_Line(5, Up);
	
	Display_Page00_01_ParaArea();	//��ʾ�̶���Ϣ
	
	BUTTON_Display(1, Hanzi_Fanhui);
	BUTTON_Display(2, Hanzi_Queren);		

	Display_Full_Line(12, Down);
	Display_Full_Vertical_Line(1);
	Display_Full_Vertical_Line(192);
	
	
	
}

void Disp_Page_01_01(void)
{
	Display_Full_Line(1, Up);
	Display_Full_Line(5, Up);
	
	Display_Page00_01_ParaArea();	//��ʾ�̶���Ϣ
	
	BUTTON_Display(1, Hanzi_Fanhui);
	BUTTON_Display(2, Hanzi_Queren);		

	Display_Full_Line(12, Down);
	Display_Full_Vertical_Line(1);
	Display_Full_Vertical_Line(192);
	

}

void Disp_Page_01_02(void)
{
	Display_Full_Line(1, Up);
	Display_Full_Line(5, Up);
	
	Display_Page00_01_ParaArea();	//��ʾ�̶���Ϣ
	
	BUTTON_Display(1, Hanzi_Fanhui);
	BUTTON_Display(2, Hanzi_Queren);		

	Display_Full_Line(12, Down);
	Display_Full_Vertical_Line(1);
	Display_Full_Vertical_Line(192);
}

void Disp_Page_01_03(void)
{
	Display_Full_Line(1, Up);
	Display_Full_Line(5, Up);
	
	Display_Page00_01_ParaArea();	//��ʾ�̶���Ϣ
	
	BUTTON_Display(1, Hanzi_Fanhui);
	BUTTON_Display(2, Hanzi_Queren);		

	Display_Full_Line(12, Down);
	Display_Full_Vertical_Line(1);
	Display_Full_Vertical_Line(192);
	
}

void LCD_Diaplay_Page(uchar page, uchar level)
{
	clear_screen();
	if(page == 0)
		{
			switch(level)
			{
				case 0:	Display_Page00_00_MotorDirt();break;
				case 1:	Disp_Page_DownLMT_STD();break;
				case 2:	Disp_Page_UpLMT_STD();break;
				case 3:	Disp_Page_RunningMode();break;
				default: break;
			}
		}
		else
			{;}
	if(page == 1)
		{
			switch(level)
			{
				case 0:	Disp_Page_01_00();break;
				case 1:	Disp_Page_01_01();break;
				case 2:	Disp_Page_01_02();break;
				case 3:	Disp_Page_01_03();break;
				default: break;
			}
		}
		else
			{;}
}




//汉字取模设置：阴码，列行式，顺向
uchar code Hanzi_1616_ST_Dian[]={
0x00,0x00,0x1F,0x11,0x11,0x11,0x11,0xFF,0x11,0x11,0x11,0x11,0x1F,0x00,0x00,0x00,
0x00,0x00,0xF8,0x10,0x10,0x10,0x10,0xFE,0x11,0x11,0x11,0x11,0xF9,0x01,0x0F,0x00};/*"电",0*/

uchar code Hanzi_1616_ST_Ji[]={
0x08,0x08,0x0B,0xFF,0x09,0x08,0x00,0x7F,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,0x00,
0x20,0xC0,0x00,0xFF,0x00,0xC1,0x06,0xF8,0x00,0x00,0x00,0xFC,0x02,0x02,0x1E,0x00};/*"机",1*/


uchar code Hanzi_1616_HWXH_Dian[]={
0x00,0x00,0x0F,0x09,0x09,0x09,0x09,0x3F,0x09,0x09,0x09,0x09,0x09,0x0F,0x00,0x00,
0x00,0x00,0xF0,0x20,0x20,0x20,0x20,0xFE,0x22,0x22,0x22,0x22,0x22,0xF2,0x0C,0x00};/*"电",0*/

uchar code Hanzi_1616_HWXH_Ji[]={
0x00,0x04,0x04,0x05,0x3F,0x05,0x04,0x04,0x1F,0x10,0x10,0x10,0x1F,0x00,0x00,0x00,
0x00,0x30,0x60,0x80,0xFE,0x02,0xC2,0x4C,0xF0,0x00,0x00,0x00,0xFE,0x02,0x06,0x04};/*"机",1*/

uchar code Hanzi_1616_XST_Fan[]={
0x00,0x02,0x02,0x42,0x33,0x00,0x00,0x3F,0x24,0x25,0x24,0x44,0x44,0xC5,0x46,0x00,
0x00,0x00,0x02,0x04,0xF8,0x04,0x1A,0xE2,0x0A,0x12,0xA2,0x42,0xA2,0x12,0x0A,0x02};/*"返",0*/

uchar code Hanzi_1616_XST_Hui[]={
0x00,0x00,0x00,0x7F,0x40,0x40,0x4F,0x48,0x48,0x48,0x4F,0x40,0x40,0x7F,0x00,0x00,
0x00,0x00,0x00,0xFE,0x04,0x04,0xE4,0x24,0x24,0x24,0xE4,0x04,0x04,0xFE,0x00,0x00};/*"回",0*/

uchar code Hanzi_1616_XST_Que[]={
0x20,0x21,0x27,0x3A,0x22,0x23,0x04,0x08,0x17,0xE4,0x24,0x27,0x2C,0x34,0x07,0x00,
0x40,0x80,0xFE,0x08,0x08,0xFC,0x01,0x06,0xF8,0x90,0x90,0xFC,0x92,0x91,0xFE,0x00};/*"确",0*/

uchar code Hanzi_1616_XST_Ding[]={
0x08,0x30,0x22,0x22,0x22,0x22,0xA2,0x63,0x22,0x22,0x22,0x22,0x22,0x28,0x30,0x00,
0x01,0x02,0x04,0x78,0x04,0x02,0x02,0xFE,0x22,0x22,0x22,0x22,0x22,0x02,0x02,0x00};/*"定",0*/

uchar code Hanzi_1616_XST_Ren[]={
0x02,0x02,0x42,0x33,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xFC,0x09,0x12,0x04,0x18,0xE0,0x00,0xE0,0x18,0x04,0x02,0x01,0x00};/*"认",0*/

uchar code Hanzi_1616_XST_Fan1[]={
0x00,0x00,0x00,0x3F,0x24,0x24,0x27,0x24,0x24,0x44,0x44,0x44,0xC5,0x46,0x00,0x00,
0x00,0x02,0x0C,0xF1,0x01,0x02,0x02,0xC4,0x28,0x10,0x28,0x44,0x82,0x02,0x01,0x01};/*"反",0*/

uchar code Hanzi_1616_XST_Zheng[]={
0x00,0x00,0x40,0x40,0x43,0x40,0x40,0x40,0x7F,0x41,0x41,0x41,0x41,0x41,0x40,0x00,
0x00,0x02,0x02,0x02,0xFE,0x02,0x02,0x02,0xFE,0x02,0x02,0x02,0x02,0x02,0x02,0x02};/*"正",0*/

uchar code Hanzi_1616_XST_Xiang[]={
0x00,0x1F,0x10,0x10,0x30,0x53,0x92,0x12,0x12,0x13,0x10,0x10,0x10,0x1F,0x00,0x00,
0x00,0xFF,0x00,0x00,0x00,0xF8,0x10,0x10,0x10,0xF8,0x00,0x02,0x01,0xFE,0x00,0x00};/*"向",0*/

uchar code Hanzi_1616_XST_Bian[]={
0x00,0x04,0x0C,0x35,0xC6,0x0C,0x00,0x3F,0x24,0xA4,0x64,0x24,0x24,0x24,0x3C,0x00,
0x00,0x44,0xE6,0x44,0x48,0x4A,0x1C,0xE0,0xFF,0x90,0xFE,0x90,0xFC,0x91,0xFF,0x00};/*"编",0*/

uchar code Hanzi_1616_XST_Ma[]={
0x20,0x21,0x27,0x3A,0x22,0x23,0x00,0x40,0x4F,0x41,0x41,0x41,0x7F,0x01,0x01,0x00,
0x40,0x80,0xFE,0x08,0x08,0xFC,0x00,0x10,0x10,0x10,0x10,0x12,0x11,0x02,0xFC,0x00};/*"码",0*/

uchar code Hanzi_1616_XST_Qi[]={
0x01,0x01,0x79,0x49,0x49,0x49,0x79,0x07,0x01,0x79,0x4D,0x4B,0x49,0x79,0x01,0x00,
0x10,0x10,0x2F,0x29,0x49,0x49,0x8F,0x00,0x80,0x4F,0x49,0x29,0x29,0x1F,0x10,0x00};/*"器",0*/

uchar code Hanzi_1616_XST_Yun[]={
0x02,0x02,0x42,0x33,0x00,0x04,0x44,0x44,0x45,0x46,0x44,0x44,0x44,0x04,0x00,0x00,
0x00,0x02,0x04,0xF8,0x04,0x22,0x72,0xA2,0x22,0x22,0x22,0xA2,0x62,0x32,0x02,0x00};/*"运",0*/

uchar code Hanzi_1616_XST_Xing[]={
0x00,0x08,0x11,0x23,0xCC,0x00,0x02,0x42,0x42,0x42,0x43,0x42,0x42,0x42,0x02,0x00,
0x40,0x80,0x00,0xFF,0x00,0x00,0x00,0x00,0x02,0x01,0xFE,0x00,0x00,0x00,0x00,0x00};/*"行",0*/

uchar code Hanzi_1616_XST_Shu[]={
0x09,0x4A,0x2C,0x08,0xFF,0x08,0x2C,0x4A,0x01,0x0E,0xF1,0x10,0x10,0x1F,0x10,0x00,
0x41,0x59,0x6A,0xC6,0x44,0x4A,0x71,0x00,0x01,0x02,0xCC,0x30,0xCC,0x02,0x01,0x00};/*"数",0*/

uchar code Hanzi_1616_XST_Cai[]={
0x20,0x20,0x22,0x23,0xF2,0x22,0x22,0x23,0x24,0x24,0xF4,0x2D,0x24,0x20,0x20,0x00,
0x02,0x22,0x24,0x24,0xA8,0x30,0x20,0x7F,0x20,0x30,0xA8,0x24,0x24,0x22,0x02,0x00};/*"菜",0*/

uchar code Hanzi_1616_XST_Dan[]={
0x00,0x00,0x1F,0x92,0x52,0x32,0x12,0x1F,0x12,0x32,0x52,0x92,0x1F,0x00,0x00,0x00,
0x08,0x08,0xC8,0x48,0x48,0x48,0x48,0xFF,0x48,0x48,0x48,0x48,0xC8,0x08,0x08,0x00};/*"单",0*/

uchar code Hanzi_1616_XST_Xue[]={
0x02,0x0C,0x88,0x69,0x09,0x09,0x89,0x69,0x09,0x09,0x19,0x28,0xC8,0x0A,0x0C,0x00,
0x20,0x20,0x20,0x20,0x20,0x22,0x21,0x7E,0x60,0xA0,0x20,0x20,0x20,0x20,0x20,0x00};/*"学",0*/

uchar code Hanzi_1616_XST_Xi[]={
0x00,0x40,0x40,0x40,0x48,0x44,0x43,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,
0x00,0x10,0x18,0x10,0x20,0x20,0x20,0x40,0x40,0x82,0x81,0x02,0xFC,0x00,0x00,0x00};/*"习",0*/


uchar code char_number_table[]={
0x00,0x00,0x0F,0x1F,0x10,0x16,0x1F,0x0F,0x00,0x00,0xE0,0xF0,0xD0,0x10,0xF0,0xE0,/*"0",0*/
0x00,0x04,0x04,0x0C,0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xF0,0x00,0x00,/*"1",1*/
0x00,0x0C,0x1C,0x10,0x11,0x1F,0x0E,0x00,0x00,0x30,0x70,0xD0,0x90,0x10,0x10,0x00,/*"2",2*/
0x00,0x0C,0x1C,0x11,0x11,0x1F,0x0E,0x00,0x00,0x60,0x70,0x10,0x10,0xF0,0xE0,0x00,/*"3",3*/
0x00,0x00,0x1F,0x1F,0x00,0x07,0x07,0x00,0x00,0xC0,0xC0,0x40,0x40,0xF0,0xF0,0x40,/*"4",4*/
0x00,0x1F,0x1F,0x11,0x11,0x11,0x10,0x00,0x00,0x10,0x10,0x10,0x30,0xE0,0xC0,0x00,/*"5",5*/
0x00,0x03,0x07,0x1E,0x1A,0x13,0x01,0x00,0x00,0xE0,0xF0,0x10,0x10,0xF0,0xE0,0x00,/*"6",6*/
0x00,0x10,0x10,0x11,0x17,0x1E,0x18,0x00,0x00,0x00,0x70,0xF0,0x80,0x00,0x00,0x00,/*"7",7*/
0x00,0x0E,0x1F,0x13,0x11,0x1F,0x0E,0x00,0x00,0xE0,0xF0,0x10,0x90,0xF0,0xE0,0x00,/*"8",8*/
0x00,0x0F,0x1F,0x10,0x10,0x1F,0x0F,0x00,0x00,0x00,0x90,0xB0,0xF0,0xC0,0x80,0x00/*"9",9*/
};

uchar code char_cMaohao[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8,0xD8,0x00,0x00,0x00,0x00};/*":",0*/

uchar code char_cDot[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x00,0x00};/*".",0*/

uchar code char_capital_A[]={
0x00,0x07,0x0F,0x18,0x18,0x0F,0x07,0x00,0x00,0xF0,0xF0,0x80,0x80,0xF0,0xF0,0x00};/*"A",0*/

uchar code char_capital_H[]={
0x00,0x1F,0x1F,0x01,0x01,0x1F,0x1F,0x00,0x00,0xF0,0xF0,0x00,0x00,0xF0,0xF0,0x00};/*"H",0*/

uchar code char_capital_I[]={
0x00,0x00,0x10,0x1F,0x1F,0x10,0x00,0x00,0x00,0x00,0x10,0xF0,0xF0,0x10,0x00,0x00};/*"I",0*/

uchar code char_capital_F[]={
0x00,0x1F,0x1F,0x11,0x11,0x11,0x10,0x00,0x00,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00};/*"F",0*/

uchar code char_z[]={
0x00,0x04,0x04,0x04,0x05,0x07,0x06,0x00,0x00,0x30,0x70,0xD0,0x90,0x10,0x10,0x00};/*"z",0*/



uchar code Hanzi_2424_HWZS_Dian[]={
0x00,0x00,0x00,0x00,0x03,0x03,0x01,0x01,0x01,0x01,0x01,0x1F,0x1F,0x01,0x01,0x01,0x01,0x03,
0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x11,0x11,0x11,0x11,0x11,0xFF,
0xFF,0x11,0x11,0x11,0x11,0xFF,0xFF,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,
0x00,0x00,0x00,0x00,0x00,0xF8,0xFC,0x0C,0x0C,0x0C,0x0C,0x8C,0x8C,0x0C,0x1C,0xFC,0x18,0x00};/*"电",0*/

uchar code Hanzi_2424_HWZS_Ji[]={
0x00,0x00,0x00,0x00,0x00,0x1F,0x0F,0x00,0x01,0x01,0x01,0x00,0x0F,0x0F,0x04,0x04,0x04,0x0F,
0x0F,0x04,0x00,0x00,0x00,0x00,0x00,0x80,0x83,0x8E,0xFC,0xFF,0xFF,0x90,0x88,0x8E,0x8C,0x00,
0xFF,0xFF,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x03,0x00,0x40,0x80,0x00,0x00,0x00,0xFC,
0xFC,0x00,0x04,0x08,0x10,0x60,0xC0,0x00,0x00,0x00,0x00,0xF8,0xF8,0x18,0x18,0x38,0xF8,0x10};/*"机",0*/

uchar code Hanzi_2424_HWZS_Fang[]={
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x11,0x19,0x1D,0x0F,0x01,0x01,0x01,0x01,0x01,
0x01,0x03,0x07,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFC,
0x90,0x10,0x10,0x10,0x10,0x3F,0x3F,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x08,
0x18,0x30,0xE0,0xC0,0x80,0x10,0x10,0x08,0x0C,0x0C,0x3C,0xF8,0xE0,0x00,0x00,0x00,0x00,0x00};/*"方",0*/

uchar code Hanzi_2424_HWZS_Xiang[]={
0x00,0x00,0x00,0x03,0x01,0x01,0x01,0x01,0x07,0x1D,0x19,0x11,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x7F,0x3F,0x21,0x21,0x21,
0x21,0x7F,0x3F,0x20,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0x00,
0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x80,0x80,0x00,0x08,0x08,0x0C,0xFC,0xF8,0x00,0x00,0x00};/*"向",0*/

uchar code Hanzi_2424_HWZS_Xia[]={
0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x07,0x07,0x04,0x04,0x04,0x04,0x04,0x04,
0x04,0x0C,0x0C,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
0x20,0x20,0x10,0x18,0x1C,0x0E,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/*"下",0*/

uchar code Hanzi_2424_HWZS_Xian[]={
0x00,0x00,0x00,0x0F,0x0F,0x08,0x08,0x0B,0x0E,0x0C,0x04,0x0F,0x0F,0x08,0x08,0x08,0x08,0x08,
0x08,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x01,0xE1,0xB1,0x1F,0x0F,0x00,0xFF,
0xFF,0x90,0x90,0x9E,0x93,0x90,0x91,0xFB,0xF6,0x06,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0x00,
0x80,0x80,0x80,0x00,0x08,0xFC,0xF8,0x18,0x10,0x20,0xA0,0xC0,0x60,0x30,0x38,0x18,0x10,0x00};/*"限",0*/

uchar code Hanzi_2424_HWZS_Wei[]={
0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x1E,0x0C,0x08,0x00,0x00,0x00,0x10,0x18,0x0E,0x06,0x00,
0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x04,0x08,0x10,0x60,0xFF,0x7F,0x40,0x00,0x80,0xB0,0x9E,
0x87,0x81,0x80,0x80,0x80,0x9F,0xFE,0xF0,0xA0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,
0xFC,0x08,0x08,0x08,0x08,0x08,0xC8,0xC8,0x08,0x08,0xF8,0x88,0x08,0x08,0x18,0x38,0x18,0x08};/*"位",0*/

uchar code Hanzi_2424_HWZS_Xue[]={
0x00,0x00,0x00,0x00,0x00,0x08,0x0E,0x07,0x03,0x00,0x10,0x0C,0x0F,0x07,0x00,0x00,0x03,0x1E,
0x0C,0x08,0x00,0x00,0x00,0x00,0x00,0x08,0x19,0x39,0xF1,0x41,0x51,0x51,0x51,0x51,0x51,0x51,
0x57,0x53,0x55,0xD9,0x59,0x71,0x59,0x51,0xE3,0xE7,0x63,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x08,0x08,0x0C,0xFC,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/*"学",0*/

uchar code Hanzi_2424_HWZS_Xi[]={
0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
0x0F,0x0F,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x81,0x83,0xC3,0xF3,0x72,
0x26,0x04,0x04,0x04,0x08,0x08,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,
0x80,0x80,0x00,0x00,0x00,0x00,0x10,0x10,0x10,0x18,0x1C,0x38,0xF8,0xF0,0x00,0x00,0x00,0x00};/*"习",0*/

uchar code Hanzi_2424_HWZS_Shang[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x0F,0x0F,0x08,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
0xFF,0x20,0x20,0x20,0x20,0x20,0x60,0xE0,0x60,0x20,0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0xF8,0xF8,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x18,0x38,0x18,0x08,0x00};/*"上",0*/

uchar code Hanzi_2424_HWZS_Zhi[]={
0x00,0x04,0x04,0x04,0x04,0x05,0x07,0x06,0x04,0x04,0x0C,0x1C,0x0C,0x04,0x03,0x1F,0x1C,0x08,
0x00,0x00,0x00,0x03,0x01,0x00,0x00,0x22,0x32,0x72,0xF2,0xA2,0x3F,0x3F,0xA2,0xE2,0x66,0x76,
0x0A,0x70,0xFC,0x87,0x81,0x8F,0xFF,0xF8,0xC0,0x80,0x80,0x80,0x00,0x10,0x18,0x18,0x18,0x10,
0xF0,0xF0,0x20,0x24,0x24,0x28,0x08,0x10,0x30,0xE0,0xE0,0xF0,0x38,0x38,0x1C,0x08,0x08,0x08};/*"致",0*/

uchar code Hanzi_2424_HWZS_Ke[]={
0x00,0x00,0x02,0x04,0x04,0x04,0x07,0x0F,0x0C,0x18,0x08,0x00,0x00,0x03,0x03,0x01,0x00,0x1F,
0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x43,0x4E,0x7C,0xFF,0xFF,0x50,0xC8,0xCE,0xC4,
0x20,0x19,0x9D,0x81,0x01,0xFF,0xFF,0x02,0x06,0x06,0x06,0x00,0x00,0x40,0x80,0x00,0x00,0x00,
0xFC,0xFC,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00};/*"科",0*/

uchar code Hanzi_2424_HWZS_Ji1[]={
0x00,0x00,0x00,0x00,0x00,0x1F,0x0F,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x1F,0x1F,0x01,0x01,
0x01,0x03,0x03,0x03,0x00,0x00,0x00,0x84,0x86,0x86,0x8C,0xFF,0xFF,0x98,0x90,0x90,0xB0,0x18,
0x17,0x11,0xF0,0xF0,0x11,0x17,0x3E,0x38,0x10,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x0C,0xFC,
0xF8,0x00,0x04,0x04,0x08,0x08,0x10,0xF0,0xE0,0xE0,0xB0,0x38,0x18,0x1C,0x1C,0x08,0x00,0x00};/*"技",0*/

uchar code Hanzi_2424_HWZS_Null[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/*" ",0*/

uchar code Hanzi_2424_HWZS_Guan[]={
0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x21,0x19,0x1D,0x01,0x01,0x01,0x03,0x0D,0x79,0x31,0x01,
0x01,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x05,0xFF,
0xE4,0x06,0x05,0x04,0x04,0x04,0x04,0x04,0x0C,0x0C,0x04,0x00,0x00,0x00,0x02,0x02,0x04,0x04,
0x08,0x10,0x30,0x60,0x80,0x00,0x00,0x00,0x80,0x60,0x30,0x18,0x0C,0x0C,0x06,0x04,0x04,0x00};/*"关",0*/

uchar code Hanzi_2424_HWZS_Men[]={
0x00,0x00,0x00,0x00,0x03,0x02,0x20,0x1C,0x1C,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x1F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x04,0x06,0xFC,0xF8,0x00,0x00,0x00};/*"门",0*/

uchar code Hanzi_2424_HWZS_Yan[]={
0x00,0x00,0x08,0x08,0x08,0x09,0x0B,0x0C,0x08,0x00,0x00,0x04,0x04,0x04,0x04,0x08,0x0F,0x08,
0x08,0x18,0x18,0x08,0x00,0x00,0x00,0x00,0x00,0x32,0x61,0x90,0x10,0x1F,0x38,0x00,0x00,0x40,
0x3F,0x00,0x00,0x00,0xFF,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0x00,0x02,0x02,0x04,0x88,0x70,
0xE0,0x90,0x10,0x08,0x08,0x4C,0xCC,0x44,0x44,0x44,0xC4,0x44,0x44,0x44,0xC4,0x48,0x08,0x00};/*"延",0*/

uchar code Hanzi_2424_HWZS_Shi[]={
0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0x0F,0x0F,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x3F,
0x21,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0xFF,0x10,0x10,0x10,0x10,0xFF,0xFF,0x00,0x00,0x10,
0x0E,0x06,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x20,0x20,0x20,
0x20,0xF0,0xE0,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x06,0xFC,0x00,0x00,0x00,0x00,0x00,0x00};/*"时",0*/