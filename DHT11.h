#ifndef	__DHT11_H__
#define	__DHT11_H__

extern unsigned char DHT11Data[5];

unsigned char DHT11_Init(void);
unsigned char DHT11_ReceiveByte(void);
unsigned char DHT11_ReadData(void);

#endif
