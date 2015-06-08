/**
  ******************************************************************************
  * @file    drv_exti.c
  * @author  Alice
  * @version V1.0
  * @date    2015-5-25
  * @brief   I/O���ж�Ӧ��
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
  
#include "drv_exti.h"

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_PA0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	
	/* Enable the GPIO  Clock ����GPIOA������ʱ��*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
											
	/* config the NVIC */
	NVIC_Configuration();

	/* EXTI line gpio config*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;								//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;							//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* config the SYSCFG clock  */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	/* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); //PA0���ӵ��ж���0
	
	/* EXTI line mode config */ 
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}

void EXTI_PA0_IRQHandler(void)
{
	  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    /* Toggle LED1 */
    GPIOC->ODR ^= GPIO_Pin_6;

    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
	}
}


