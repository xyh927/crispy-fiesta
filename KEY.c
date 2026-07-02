#include <REGX52.H>

sbit Key1=P1^0;
sbit Key2=P1^1;
sbit Key3=P1^2;
sbit Key4=P1^3;

unsigned char idata KeyNumber;

/**
  * @brief  获取独立按键键码
  * @param  无
  * @retval 按下按键的键码，范围：0,1~13,0表示无按键按下
  */
unsigned char Key(void)
{
	unsigned char KeyTemp=0;
	KeyTemp=KeyNumber;
	KeyNumber=0;	//主程序中获取键码值之后键码值清零，在下一次定时器扫描按键之前再次获取键码值，一定会返回0
	return KeyTemp;
}

/**
  * @brief  获取当前按键的状态，无消抖及松手检测
  * @param  无
  * @retval 按下的按键，范围：0~4，无按键按下时返回值为0
  */
unsigned char Key_GetState()
{
	unsigned char KeyValue=0;
	
	if(Key1==0){KeyValue=1;}
	if(Key2==0){KeyValue=2;}
	if(Key3==0){KeyValue=3;}
	if(Key4==0){KeyValue=4;}

	return KeyValue;
}


/**
  * @brief  按键驱动函数，在中断中调用
  * @param  无
  * @retval 无
  */
void Key_Loop(void)
{
	static unsigned char NowState,LastState;
	LastState=NowState;	//按键状态更新
	NowState=Key_GetState();	//获取当前按键状态
	
	//如果上个时间点按键未按下，这个时间点按键按下，则是按下瞬间
	if(LastState==0)
	{
		switch(NowState)
		{
			case 1:KeyNumber=1;break;
			case 2:KeyNumber=2;break;
			case 3:KeyNumber=3;break;
			case 4:KeyNumber=4;break;
			default:break;
		}
	}
	
	//如果上个时间点按键按下，这个时间点按键按下，则是一直按住按键
	if(LastState==3 && NowState==3){KeyNumber=5;}
	if(LastState==4 && NowState==4){KeyNumber=6;}
	
	//如果上个时间点按键按下，这个时间点按键未按下，则是松手瞬间
	if(NowState==0)
	{
		switch(LastState)
		{
			case 1:KeyNumber=9;break;
			case 2:KeyNumber=10;break;
			case 3:KeyNumber=11;break;
			case 4:KeyNumber=12;break;
			default:break;
		}
	}
}
