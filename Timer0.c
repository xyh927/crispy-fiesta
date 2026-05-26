#include <REGX52.H>

/**
  * @brief	定时器0初始化，1毫秒@11.0592MHz
  * @param  无
  * @retval 无
  */
void Timer0_Init(void)
{
	TMOD&=0xF0;	//设置定时器模式（高四位不变，低四位清零）
	TMOD|=0x01;	//设置定时器模式（通过低四位设为“定时器0工作方式1”的模式）
	TL0=0x00;	//设置定时初值，定时10ms
	TH0=0xDC;	//设置定时初值，定时10ms
	TF0=0;	//清除TF0标志
	TR0=1;	//定时器0开始计时
	ET0=1;	//打开定时器0中断允许
	EA=1;	//打开总中断
	PT0=0;	//当PT0=0时，定时器0为低优先级，当PT0=1时，定时器0为高优先级
}

/*定时器中断函数模板
void Timer0_Routine() interrupt 1	//定时器0中断函数
{
	static unsigned int T0Count;	//定义静态变量
	TL0=0x00;	//设置定时初值，定时10ms
	TH0=0xDC;	//设置定时初值，定时10ms
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;
		
	}
}
*/
