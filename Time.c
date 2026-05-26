#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "Relay.h"

unsigned char TimeFlashFlag;	//设置时间时闪烁的标志，1：不显示，0：显示
unsigned char THFlashFlag;   // 1=显示  0=不显示（闪烁用）
unsigned char TimeMode;	//时间模式，0：正常走时模式，1：设置时间模式，2：设置温湿度上下限模式
unsigned char TimeSelect;	//时间选择变量
unsigned char THSelect;	//温湿度选择变量
unsigned char StaticShowFlag;	//切换模式时LCD1602显示静态字符的标志，1：清屏和显示静态字符，0：不清屏和不重复显示静态字符

void TimeShow(void)	//时间显示功能
{
	if(TimeMode==1&&TimeFlashFlag==1&&TimeSelect==0){LCD_ShowString(1,1,"  ");}	//设置时间时闪烁
	else{LCD_ShowNum(1,1,DS1302_Time[0],2);}	//显示年
	if(TimeMode==1&& TimeFlashFlag==1&&TimeSelect==1){LCD_ShowString(1,4,"  ");}
	else{LCD_ShowNum(1,4,DS1302_Time[1],2);}	//显示月
	if(TimeMode==1&& TimeFlashFlag==1&&TimeSelect==2){LCD_ShowString(1,7,"  ");}
	else{LCD_ShowNum(1,7,DS1302_Time[2],2);}	//显示日
	if(TimeMode==1&& TimeFlashFlag==1&&TimeSelect==3){LCD_ShowString(1,10,"   ");}
	else
	{		
		switch(DS1302_Time[6])
		{
			case 1 : LCD_ShowString(1,10,"Mon");break;
			case 2 : LCD_ShowString(1,10,"Tue");break;
			case 3 : LCD_ShowString(1,10,"Wed");break;
			case 4 : LCD_ShowString(1,10,"Thu");break;
			case 5 : LCD_ShowString(1,10,"Fri");break;
			case 6 : LCD_ShowString(1,10,"Sat");break;
			case 7 : LCD_ShowString(1,10,"Sun");break;
		}
	}	//显示周
	if(TimeMode==1&&TimeFlashFlag==1&&TimeSelect==4){LCD_ShowString(2,1,"  ");}
	else{LCD_ShowNum(2,1,DS1302_Time[3],2);}	//显示时
	if(TimeMode==1&&TimeFlashFlag==1&&TimeSelect==5){LCD_ShowString(2,4,"  ");}
	else{LCD_ShowNum(2,4,DS1302_Time[4],2);}	//显示分
	if(TimeMode==1&&TimeFlashFlag==1&&TimeSelect==6){LCD_ShowString(2,7,"  ");}
	else{LCD_ShowNum(2,7,DS1302_Time[5],2);}	//显示秒
}

void TimeCheck(void)	//时间越界判断，防止向DS1302写入错误数据
{
		if(DS1302_Time[0]<0){DS1302_Time[0]=99;}	//年越界判断，范围：0~99
		if(DS1302_Time[0]>99){DS1302_Time[0]=0;}
		if(DS1302_Time[1]<1){DS1302_Time[1]=12;}	//月越界判断，，范围：1~12
		if(DS1302_Time[1]>12){DS1302_Time[1]=1;}
		if(DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 || DS1302_Time[1]==7 || DS1302_Time[1]==8 || DS1302_Time[1]==10 || DS1302_Time[1]==12)	//日越界判断，区分大小月、闰年2月、平年2月
		{
			if(DS1302_Time[2]<1){DS1302_Time[2]=31;}	//大月
			if(DS1302_Time[2]>31){DS1302_Time[2]=1;}
		}
		else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 || DS1302_Time[1]==9 || DS1302_Time[1]==11)
		{
			if(DS1302_Time[2]<1){DS1302_Time[2]=30;}	//小月
			if(DS1302_Time[2]>30){DS1302_Time[2]=1;}
		}
		else if(DS1302_Time[1]==2)
		{
			if(DS1302_Time[0]%4==0)
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=29;}	//闰年2月
				if(DS1302_Time[2]>29){DS1302_Time[2]=1;}
			}
			else
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=28;}	//平年2月
				if(DS1302_Time[2]>28){DS1302_Time[2]=1;}
			}
		}
		if(DS1302_Time[3]<0){DS1302_Time[3]=23;}	//时越界判断，范围：0~23
		if(DS1302_Time[3]>23){DS1302_Time[3]=0;}
		if(DS1302_Time[4]<0){DS1302_Time[4]=59;}	//分越界判断，范围：0~59
		if(DS1302_Time[4]>59){DS1302_Time[4]=0;}	
		if(DS1302_Time[5]>59){DS1302_Time[5]=0;}	//秒越界判断，范围：0~59	
		if(DS1302_Time[6]<1){DS1302_Time[6]=7;}		//周越界判断，范围：1~7
		if(DS1302_Time[6]>7){DS1302_Time[6]=1;}		
}

void TimeUpdate(void)	//设置时按下（短按或长按）K3和K4更新显示时间
{
	LCD_ShowNum(1,1,DS1302_Time[0],2);	//显示年
	LCD_ShowNum(1,4,DS1302_Time[1],2);	//显示月
	LCD_ShowNum(1,7,DS1302_Time[2],2);	//显示日
	switch(DS1302_Time[6])	//显示周
	{
		case 1 : LCD_ShowString(1,10,"Mon");break;
		case 2 : LCD_ShowString(1,10,"Tue");break;
		case 3 : LCD_ShowString(1,10,"Wen");break;
		case 4 : LCD_ShowString(1,10,"Thu");break;
		case 5 : LCD_ShowString(1,10,"Fri");break;
		case 6 : LCD_ShowString(1,10,"Sat");break;
		case 7 : LCD_ShowString(1,10,"Sun");break;
	}
	LCD_ShowNum(2,1,DS1302_Time[3],2);	//显示时
	LCD_ShowNum(2,4,DS1302_Time[4],2);	//显示分
	LCD_ShowNum(2,7,DS1302_Time[5],2);	//显示秒
}

void THSet_Show(void)
{
    static unsigned char last_select=99;   // 记录上一次选中的项，用于“重刷”
    if (StaticShowFlag)                      // 第一次进入模式 2
    {
        StaticShowFlag=0;
        LCD_Clear();
        LCD_ShowString(1,1,"T_H:   T_L:  ");
        LCD_ShowString(2,1,"H_H:   H_L:  ");
        last_select=99;                    // 强制刷新
    }
    /* 如果选中项变了，也重新刷静态字（保险） */
    if (THSelect!=last_select)
    {
        LCD_Clear();
        LCD_ShowString(1,1,"T_H:   T_L:  ");
        LCD_ShowString(2,1,"H_H:   H_L:  ");
        last_select=THSelect;
    }

    /* T_H */
    if(THSelect==0&&!THFlashFlag){LCD_ShowString(1,5,"  ");}
    else {LCD_ShowNum(1,5,Relay_GetTempHigh(),2);}
    /* T_L */
    if(THSelect==1&&!THFlashFlag){LCD_ShowString(1,12,"  ");}
    else {LCD_ShowNum(1,12,Relay_GetTempLow(),2);}
    /* H_H */
    if(THSelect==2&&!THFlashFlag){LCD_ShowString(2,5,"  ");}
    else {LCD_ShowNum(2,5,Relay_GetHumiHigh(),2);}
    /* H_L */
    if(THSelect==3&&!THFlashFlag){LCD_ShowString(2,12,"  ");}
    else {LCD_ShowNum(2,12,Relay_GetHumiLow(),2);}
}