#ifndef __Time_H__
#define __Time_H__

extern unsigned char TimeFlashFlag;	// 时间闪烁标志位 (1=显示，0=不显示，用于设置时闪烁)
extern unsigned char THFlashFlag;	// 温湿度闪烁标志位 (用于阈值设置时闪烁)
extern unsigned char TimeMode;	// 工作模式 (0=正常显示模式，1=时间设置模式，2=温湿度阈值设置模式)
extern unsigned char TimeSelect;	// 时间选择位 (设置时间时选择年月日时分秒)
extern unsigned char THSelect;	// 温湿度选择位 (0=温度上限，1=温度下限，2=湿度上限，3=湿度下限)
extern unsigned char StaticShowFlag;	// 静态显示标志 (切换模式时刷新LCD静态字符)

void TimeShow(void);
void TimeCheck(void);
void TimeUpdate(void);
void THSet_Show(void);

#endif
