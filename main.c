#include <REGX52.H>
#include "Delay.h"
#include "Key.h"
#include "DS1302.h"
#include "LCD1602.h"
#include "DHT11.h"
#include "AT24C02.h"
#include "Time.h"
#include "Relay.h"
#include "Timer0.h"
#include "UART.h"
#include "stdio.h"
#include "string.h"

void PutString(unsigned char *TXStr);

unsigned char KeyNum;	// 按键键值变量 (存储当前按下的按键编号)
unsigned char ReadTimeFlag;	// 读取时间标志位 (1=需要读取DS1302时间)
unsigned char ReadFlag;	// 读取温湿度标志位 (1=需要读取DHT11传感器)
unsigned char DHTCheckFlag;	// DHT11数据校验标志 (1=读取成功且校验正确，0=失败)
unsigned char buffer_printf[20];
bit TempValid = 0;   // 温湿度数据有效标志 (0=无效/未读取，1=已读取有效)

void main()
{
	unsigned int temp,humi;
	EA=1;
	UART_Init();
	LCD_Init();	// 初始化LCD1602液晶显示屏
	DS1302_Init();	// 初始化DS1302实时时钟模块
	Timer0_Init();	// 初始化定时器0 (用于按键扫描和定时任务)
	LCD_MakeChar();	// 向LCD1602的CGRAM写入自定义字符 (温度符号等)
	DHT11_Init();	// 初始化DHT11温湿度传感器
	Relay_Init();	// 初始化继电器控制模块

	// 上电后首次写入时间初始值 (写入DS1302_Time数组中的数值到DS1302芯片)
	DS1302_SetTime();
	Delay(500);	// 延时等待传感器稳定 (等待DHT11上电稳定)

	StaticShowFlag=1;	// 上电后进入正常显示模式，需要显示静态字符
	while(1)
	{
		KeyNum=Key();	// 读取按键扫描值
		if(KeyNum)	// 如果有按键按下
		{
			if(KeyNum==1)	// 如果K1按下 (短按/长按检测)
			{
				TimeMode++;	// 切换工作模式
				TimeMode%=3;	// 模式循环 (0->1->2->0)
				if(TimeMode){StaticShowFlag=1;}	// 切换到时间设置或阈值设置模式时刷新显示
				TimeSelect=0;	// 时间选择位清零
				THSelect=0;	// 温湿度选择位清零
				if(TimeMode!=2){StaticShowFlag=1;}
			}	
			if(KeyNum==2)	// 如果K2按下
			{
				if(TimeMode==1)	// 如果在时间设置模式
				{
					TimeSelect++;	// 时间选择位加1 (切换设置项)
					TimeSelect%=7;	// 循环选择 (年/月/日/星期/时/分/秒)
				}
				if(TimeMode==2)	// 如果在温湿度阈值设置模式
				{
					THSelect++;	// 阈值选择位加1
					THSelect%=4;	// 循环选择 (温度上限/下限，湿度上限/下限)
				}
			}
			if(KeyNum==3)	// 如果K3按下 (减/下)
			{
				if(TimeMode==1)	// 如果在时间设置模式
				{
					switch(TimeSelect)	// 根据选择的时间项减1
					{
						case 0:DS1302_Time[0]--;break;	// 年减1
						case 1:DS1302_Time[1]--;break;	// 月减1
						case 2:DS1302_Time[2]--;break;	// 日减1
						case 3:DS1302_Time[6]--;break;	// 星期减1
						case 4:DS1302_Time[3]--;break;	// 时减1
						case 5:DS1302_Time[4]--;break;	// 分减1
						case 6:DS1302_Time[5]=0;break;	// 秒清零 (按下K3清零秒)
					}
					TimeCheck();	// 检查时间值是否越界并修正
					TimeUpdate();	// 更新显示
					Delay(250);	// 延时防抖 (防止长按连续触发)
				}
				if(TimeMode==2)	// 如果在阈值设置模式
				{
					switch (THSelect)
					{
						case 0: Relay_SetTempHigh(Relay_GetTempHigh()-1); break;	// 温度上限减1
						case 1: Relay_SetTempLow (Relay_GetTempLow ()-1); break;	// 温度下限减1
						case 2: Relay_SetHumiHigh(Relay_GetHumiHigh()-1); break;	// 湿度上限减1
						case 3: Relay_SetHumiLow (Relay_GetHumiLow ()-1); break;	// 湿度下限减1
					}
					Delay(250);	// 延时防抖
				}
			}
			if(KeyNum==4)	// 如果K4按下 (加/上)
			{
				if(TimeMode==1)	// 如果在时间设置模式
				{
					switch(TimeSelect)	// 根据选择的时间项加1
					{
						case 0:DS1302_Time[0]++;break;	// 年加1
						case 1:DS1302_Time[1]++;break;	// 月加1
						case 2:DS1302_Time[2]++;break;	// 日加1
						case 3:DS1302_Time[6]++;break;	// 星期加1
						case 4:DS1302_Time[3]++;break;	// 时加1
						case 5:DS1302_Time[4]++;break;	// 分加1
						case 6:DS1302_Time[5]=0;break;	// 秒清零
					}
					TimeCheck();	// 检查时间值是否越界
					TimeUpdate();	// 更新显示
					Delay(250);	// 延时防抖
				}
				if(TimeMode==2)	// 如果在阈值设置模式
				{
					switch (THSelect)
					{
						case 0: Relay_SetTempHigh(Relay_GetTempHigh() + 1); break;	// 温度上限加1
						case 1: Relay_SetTempLow (Relay_GetTempLow () + 1); break;	// 温度下限加1
						case 2: Relay_SetHumiHigh(Relay_GetHumiHigh() + 1); break;	// 湿度上限加1
						case 3: Relay_SetHumiLow (Relay_GetHumiLow () + 1); break;	// 湿度下限加1
					}
					Delay(250);	// 延时防抖
				}
			}
			if(KeyNum==11 || KeyNum==12)	// 如果K3或K4松开 (短按释放)
			{
				if(TimeMode==1)	// 如果在时间设置模式
				{
					DS1302_SetTime();	// 将修改后的时间写入DS1302芯片
				}
			}
		}

		if((TimeMode==0 || TimeMode==1) && KeyNum!=7 && KeyNum!=8)	// 正常显示模式或时间设置模式
		{
			if(StaticShowFlag)	// 切换模式时需要重新显示静态字符
			{
				StaticShowFlag=0;	// 清除静态显示标志
				LCD_Clear();	// 清屏
				LCD_ShowString(1,1,"  -  -  ");	// 显示日期格式静态字符
				LCD_ShowString(2,1,"  :  :  ");	// 显示时间格式静态字符
				LCD_ShowChar(1,16,'%');	// 显示湿度单位%
				LCD_ShowChar(2,15,0);	// 显示自定义字符0 (温度符号℃)
				LCD_ShowChar(2,16,'C');
			}
			if(ReadTimeFlag && KeyNum!=3 && KeyNum!=4)	// 每100ms读取一次时间 (设置时不读取避免冲突)
			{
				ReadTimeFlag=0;	// 清除读取标志
				DS1302_ReadTime();	// 从DS1302读取当前时间
				TimeShow();	// 在LCD上显示时间
			}
			if(ReadFlag)	// 每200ms读取一次温湿度 (按键未按下时)
			{
				ReadFlag=0;	// 清除读取标志

				DHTCheckFlag=DHT11_ReadData();	// 读取DHT11传感器数据 (5字节数据+校验)
				if(DHTCheckFlag)
				{
					TempValid = 1;
					temp = DHT11Data[2];	// 提取温度整数部分
					humi = DHT11Data[0];	// 提取湿度整数部分

					LCD_ShowNum(1, 14, humi, 2);	// 在第1行第14列显示湿度
					LCD_ShowNum(2, 13, temp, 2);	// 在第2行第13列显示温度

					sprintf((char *)buffer_printf, "TEMP:%d,HUMI:%d\r\n", temp, humi);	// 格式化串口输出
					PutString(buffer_printf);
				}
				else
				{
					TempValid = 0;   // ← 新增：读取失败，标记数据无效
				}

				/*  只有数据有效时才进行继电器控制  */
				if(TempValid)
					Relay_Control(DHT11Data[2], DHT11Data[0]);	// 根据温湿度控制继电器
			}
		}
		if (TimeMode==2)
		{
			 THSet_Show();	// 显示温湿度阈值设置界面
		}
	}
}

void Timer0_Routine() interrupt 1	// 定时器0中断服务函数 (每10ms中断一次)
{
	static unsigned int T0Count1,T0Count2,T0Count3,T0Count4;	// 静态计数器
	TL0=0x00;	// 重装定时器初值 (定时10ms，晶振11.0592MHz)
	TH0=0xDC;	// 重装定时器初值
	T0Count1++;
	T0Count2++;
	T0Count3++;
	T0Count4++;
	if(T0Count1>=50)	// 500ms计数 (50*10ms)
	{
		T0Count1=0;
		TimeFlashFlag=!TimeFlashFlag;	// 时间闪烁标志取反 (1Hz闪烁)
		THFlashFlag=!THFlashFlag;	// 温湿度闪烁标志取反
	}
	if(T0Count2>=3)	// 30ms计数 (按键扫描周期)
	{
		T0Count2=0;
		Key_Loop();	// 执行按键扫描 (支持短按和长按检测)
	}
	if(T0Count3>=10)	// 100ms计数
	{
		T0Count3=0;
		ReadTimeFlag=1;	// 设置读取时间标志
	}
	if(T0Count4>=20)	// 200ms计数
	{
		T0Count4=0;
		ReadFlag=1;	// 设置读取温湿度标志
	}
}


// 串口发送字符串函数
void PutString(unsigned char *TXStr)
{
    ES=0;	// 关闭串口中断 (防止发送中断干扰)
    while(*TXStr!=0)
    {
        SBUF=*TXStr;	// 发送当前字符
        while(TI==0);	// 等待发送完成
        TI=0;	// 清除发送标志
        TXStr++;
    }
    ES=1;	// 打开串口中断
}

// 串口中断服务函数

void UART() interrupt 4	// 串口中断 (发送标志TI和接收标志RI需要手动清除)
{
	unsigned char temp;
    if(RI==1)
	{
		temp=SBUF;	// 读取接收到的字符
		if(temp=='#')
		{	// 如果收到'#'，打开UV灯
			PutString("UV ON\r\n");
			Relay_UV_On();// 继电器低电平触发 (UV灯亮)
		}
		else if(temp=='$')
		{	// 如果收到'$'，关闭UV灯
			PutString("UV OFF\r\n");
			Relay_UV_Off();	// 继电器断开 (UV灯灭)
		}
    }
    RI=0;	// 清除接收中断标志 (防止中断嵌套)
}

