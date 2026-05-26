#include <REGX52.H>

void UART_Init()
{
    SCON=0x50;	// 串口模式1，允许接收
    TMOD|=0x20;	// 定时器1，模式2 (8位自动重装)
    TMOD&=~0x10;
    TH1=0xFA;	// 9600bps (晶振11.0592MHz)
    TL1=0xfa;
    PCON|=0x80;	// SMOD=1 (波特率加倍)
    TR1=1;	// 启动定时器1
    TI=0;
    RI=0;
    ES=1;	// 允许串口中断
}