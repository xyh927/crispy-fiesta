#include <REGX52.H>

//引脚配置：
sbit LCD_RS=P2^6;
sbit LCD_RW=P2^5;
sbit LCD_EN=P2^7;
#define LCD_DataPort P0

unsigned char code CGRAMData[]={
0x07,0x05,0x07,0x00,0x00,0x00,0x00,0x00,	//摄氏度的圆圈
};

//函数定义：
/**
  * @brief  LCD1602私有延时函数，11.0592MHz调用可延时101us
  * @param  无
  * @retval 无
  */
void LCD_Delay101us(void)
{
	unsigned char i;

	i=44;
	while(--i);
}

/**
  * @brief  LCD1602延时函数，11.0592MHz调用可延时2ms
  * @param  无
  * @retval 无
  */
void LCD_Delay2ms(void)
{
	unsigned char i, j;

	i=4;
	j=146;
	do
	{
		while(--j);
	}while(--i);
}

/**
  * @brief  LCD1602写指令
  * @param  Command 要写入的指令
  * @retval 无
  */
void LCD_WriteCommand(unsigned char Command)
{
	LCD_RS=0;
	LCD_RW=0;
	LCD_DataPort=Command;
	LCD_EN=1;
	LCD_Delay101us();
	LCD_EN=0;
	LCD_Delay101us();
}

/**
  * @brief  LCD1602写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void LCD_WriteData(unsigned char Data)
{
	LCD_RS=1;
	LCD_RW=0;
	LCD_DataPort=Data;
	LCD_EN=1;
	LCD_Delay101us();
	LCD_EN=0;
	LCD_Delay101us();
}

/**
  * @brief  LCD1602设置光标位置
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @retval 无
  */
void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		LCD_WriteCommand(0x80|(Column-1));
	}
	else if(Line==2)
	{
		LCD_WriteCommand(0x80|(Column-1+0x40));
	}
}

/**
  * @brief  LCD1602初始化函数
  * @param  无
  * @retval 无
  */
void LCD_Init()
{
	LCD_WriteCommand(0x38);	//八位数据接口，两行显示，5*7点阵
	LCD_WriteCommand(0x0C);	//显示开，光标关，闪烁关
	LCD_WriteCommand(0x06);	//数据读写操作后，光标自动加一，画面不动
	LCD_WriteCommand(0x01);	//光标复位，清屏
	LCD_Delay2ms();	//清屏指令执行需要较长时间，需要较长的延时
}

/**
  * @brief  在LCD1602指定位置上显示一个字符
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的字符
  * @retval 无
  */
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	LCD_SetCursor(Line,Column);
	LCD_WriteData(Char);
}

/**
  * @brief  在LCD1602指定位置开始显示所给字符串
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串
  * @retval 无
  */
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=0;String[i]!='\0';i++)
	{
		LCD_WriteData(String[i]);
	}
}

/**
  * @brief  返回值=X的Y次方
  */
int LCD_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}

/**
  * @brief  在LCD1602指定位置开始显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~65535
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(10,i-1)%10+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始以有符号十进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-32768~32767
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length)
{
	unsigned char i;
	unsigned int Number1;
	LCD_SetCursor(Line,Column);
	if(Number>=0)
	{
		LCD_WriteData('+');
		Number1=Number;
	}
	else
	{
		LCD_WriteData('-');
		Number1=-Number;
	}
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number1/LCD_Pow(10,i-1)%10+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始以十六进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFF
  * @param  Length 要显示数字的长度，范围：1~4
  * @retval 无
  */
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i,SingleNumber;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		SingleNumber=Number/LCD_Pow(16,i-1)%16;
		if(SingleNumber<10)
		{
			LCD_WriteData(SingleNumber+'0');
		}
		else
		{
			LCD_WriteData(SingleNumber-10+'A');
		}
	}
}

/**
  * @brief  在LCD1602指定位置开始以二进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(2,i-1)%2+'0');
	}
}

/**
  * @brief  在LCD1602的CGRAM中写入8个自定义字符的64个字节，每个5*8点阵横向取模，使用每个字节的低5位
  * @param  无
  * @retval 无
  */
void LCD_MakeChar(void)
{
	unsigned char i;
	LCD_WriteCommand(0x40);	//CGRAM起始地址为0x40
	for(i=0;i<8;i++)	//01XX XXXX，最多可以在CGRAM中写入64个数据，一个自定义字符对应8个数据
	{
		LCD_WriteData(CGRAMData[i]);
	}
}

/**
  * @brief  LCD1602的光标复位，清屏
  * @param  无
  * @retval 无
  */
void LCD_Clear(void)
{
	LCD_WriteCommand(0x01);
	LCD_Delay2ms();
}

/**
  * @brief  LCD1602的屏幕向左移动一个字符位，光标不动
  * @param  无
  * @retval 无
  */
void LCD_MoveLeft(void)
{
	LCD_WriteCommand(0x18);
}

/**
  * @brief  LCD1602的屏幕向左移动一个字符位，光标不动
  * @param  无
  * @retval 无
  */
void LCD_MoveRight(void)
{
	LCD_WriteCommand(0x1C);
}
