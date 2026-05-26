#include <REGX52.H>
#include "Relay.h"
#include "DS1302.h"
#include "LCD1602.h"

sbit RELAY_COOL = P1^4;    // 降温降湿继电器
sbit RELAY_HEAT = P1^5;    // 加热继电器
sbit RELAY_HUMI = P1^6;    // 加湿继电器
sbit RELAY_UV = P1^7;      // 紫外灯继电器

unsigned int temp_high=30;       // 温度上限(默认30℃)
unsigned int temp_low=20;        // 温度下限(默认20℃)
unsigned int humi_high=60;       // 湿度上限(默认60%)
unsigned int humi_low=40;        // 湿度下限(默认40%)

// 继电器初始化
void Relay_Init(void)
{
    RELAY_HEAT=1;
    RELAY_COOL=1;
    RELAY_HUMI=1;
    RELAY_UV=1;
}


void Relay_Control(unsigned int temp, unsigned int humi)
{
    /* ---------- 制冷（RELAY_COOL=0 吸合）---------- */
    if(temp > temp_high + 1 || humi > humi_high + 2) {
        RELAY_COOL = 0;           // 温度过高或湿度过高 → 强制开启
    }
    else if(temp < temp_high - 1) {
        RELAY_COOL = 1;           // 温度已降到安全线以下 → 允许关闭
    }
    /* 滞回带：temp 在 [high-1, high+1] 之间时，不操作，保持原状态 */

    /* ---------- 加热（RELAY_HEAT=0 吸合）---------- */
    if(temp < temp_low - 1) {
        RELAY_HEAT = 0;           // 温度过低 → 强制开启
    }
    else if(temp > temp_low + 1) {
        RELAY_HEAT = 1;           // 温度已回升 → 允许关闭
    }
    /* 中间态保持原状态（原代码已如此，无需改动） */

    /* ---------- 加湿（RELAY_HUMI=0 吸合）---------- */
    if(humi < humi_low - 2) {
        RELAY_HUMI = 0;           // 湿度过低 → 强制开启
    }
    else if(humi > humi_low + 2) {
        RELAY_HUMI = 1;           // 湿度已回升 → 允许关闭
    }
    /* 滞回带：humi 在 [low-2, low+2] 之间时，不操作，保持原状态 */
}

/* UV灯控制（低电平触发） */
void Relay_UV_On(void)  { RELAY_UV = 0; }  // 继电器吸合，UV灯亮
void Relay_UV_Off(void) { RELAY_UV = 1; }  // 继电器断开，UV灯灭

/* ---------- 设置阈值 ---------- */
void Relay_SetTempHigh(unsigned int val)
{
    if(val>99) val=99;
    temp_high=val;
}
void Relay_SetTempLow (unsigned int val)
{
    if(val>99) val=99;
    temp_low=val;
}
void Relay_SetHumiHigh(unsigned int val)
{
    if(val>99) val=99;
    humi_high=val;
}
void Relay_SetHumiLow (unsigned int val)
{
    if(val>99) val=99;
    humi_low=val;
}
/* ---------- 读取阈值 ---------- */
unsigned int Relay_GetTempHigh(void) {return temp_high;}
unsigned int Relay_GetTempLow (void) {return temp_low;}
unsigned int Relay_GetHumiHigh(void) {return humi_high;}
unsigned int Relay_GetHumiLow (void) {return humi_low;}
