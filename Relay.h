#ifndef __RELAY_H__
#define __RELAY_H__

/* 供外部使用的变量声明 */
extern unsigned int temp_high;
extern unsigned int temp_low;
extern unsigned int humi_high;
extern unsigned int humi_low;

// 初始化函数
void Relay_Init(void);
// 温湿度控制函数
void Relay_Control(unsigned int temp, unsigned int humi);
void Relay_UV_On(void);
void Relay_UV_Off(void);

/* 阈值设置 / 读取 封装 */
void Relay_SetTempHigh(unsigned int val);
void Relay_SetTempLow (unsigned int val);
void Relay_SetHumiHigh(unsigned int val);
void Relay_SetHumiLow (unsigned int val);

unsigned int Relay_GetTempHigh(void);
unsigned int Relay_GetTempLow (void);
unsigned int Relay_GetHumiHigh(void);
unsigned int Relay_GetHumiLow (void);

#endif