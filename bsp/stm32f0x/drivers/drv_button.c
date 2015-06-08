/* Includes ------------------------------------------------------------------*/	
#include "stm32f0xx.h"
#include "drv_button.h"

void Delay(__IO uint32_t nCount)
{
	for(;nCount != 0; nCount--);
}

void BUTTON_Init(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
  GPIO_InitTypeDef GPIO_InitStructure;
   
  /* Enable the GPIO  Clock ����GPIOA������ʱ��*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
  /* Configure USART Tx Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;										//GPIOA0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;								//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;							//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);											//��ʼ��PA0
	
}


uint8_t BUTTON_Scan(void)
{	 
//	static uint8_t key = 0;
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == BUTTON_ON)
	{
		/* ��ʱ����*/
		Delay(10000);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == BUTTON_ON)
		{
			return BUTTON_ON;
		}
	}
	
	return BUTTON_OFF;// �ް�������
}
