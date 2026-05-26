/**
  * @brief	延时函数，晶振@11.0592MHz时，输入参数延时的单位是1ms
  * @param	xms 延时的时间，范围：0~65535
  * @retval	无
  */
  
void Delay(unsigned int xms)
{
	unsigned char idata i, j;
	while(xms)
	{
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
		xms--;
	}
}