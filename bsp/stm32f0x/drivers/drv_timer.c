/* Includes ------------------------------------------------------------------*/	
#include "stm32f0xx.h"
#include "drv_timer.h"



//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Init(void)
{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
//	
//	
//	/* -----------------------------------------------------------------------
//  In this example TIM7 counter clock (TIM7CLK) is set to APB1 clock (PCLK1), since
//  APB1 prescaler is set to 1 and TIM7 prescaler is set to 0.
//  
//  In this example TIM7 input clock (TIM7CLK) is set to APB1 clock (PCLK1), 
//  since APB1 prescaler is set to 1.   
//  TIM7CLK = PCLK1 = HCLK = SystemCoreClock
//  
//  With Prescaler set to 479 and Period to 24999, the TIM7 counter is updated each 250 ms
//  (i.e. and interrupt is generated each 250 ms)
//     TIM7 counter clock = TIM7CLK /((Prescaler + 1)*(Period + 1))
//                        = 48 MHz / ((25000)*(480))
//                        = 4 Hz 
//     ==> TIM7 counter period = 250 ms

//  Note:
//  SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
//  Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
//  function to update SystemCoreClock variable value. Otherwise, any configuration
//  based on this variable will be incorrect.    
//  ----------------------------------------------------------------------- */ 
//  /* Time base configuration */
//	
//  TIM_TimeBaseInitStructure.TIM_Period = 24999; 	//�Զ���װ��ֵ
//	TIM_TimeBaseInitStructure.TIM_Prescaler=479;  //��ʱ����Ƶ
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
//	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPriority=0x01; //��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;    
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //??TIM3??
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //?????????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO?????50MHz
            GPIO_Init(GPIOA, &GPIO_InitStructure);					
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;  //
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //??NVIC_InitStruct???????????NVIC???

	TIM_TimeBaseStructure.TIM_Period = 899; //?????????? 
	TIM_TimeBaseStructure.TIM_Prescaler =0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //??????:???
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????
 
  TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//??TIM????3

	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; //?????
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; //TIM??2?3?4??????IC1?2?3?4??
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1; //??????,?????
	TIM_ICInitStructure.TIM_ICFilter=0x03;  //IC4F=0011 8??????????
	TIM_ICInit(TIM3,&TIM_ICInitStructure);  //????????????*/
	TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter=0x03;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //?????TIM3??

	TIM_Cmd(TIM3, ENABLE);  //?????3

	
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
//	{
//		//�������
//		GPIOC->ODR ^= GPIO_Pin_7;
//	}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
	}	
}
