#include "drv_encoder.h"
#include "drv_timer.h"

//void TIM3_Mode_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	//TIM_OCInitTypeDef  TIM_OCInitStructure;
//	
///*----------------------------------------------------------------*/
//	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//  
//  GPIO_StructInit(&GPIO_InitStructure);
//  /* Configure PA.06,07 as encoder input */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO�ڵ��ٶ�Ϊ50MHz
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	
//		
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3??
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //?????0?
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //????3?
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
//	NVIC_Init(&NVIC_InitStructure);  //??NVIC_InitStruct???????????NVIC???
///*----------------------------------------------------------------*/	

//	
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ�ܣԣɣͣ�
//	TIM_DeInit(TIM3);
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//	
//	TIM_TimeBaseStructure.TIM_Period =0xffff;       //
//  TIM_TimeBaseStructure.TIM_Prescaler =0;	    //����Ԥ��Ƶ��
//  TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
//  //TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1; 
//	/*��ʼ��TIM3��ʱ�� */
//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//	
//	/*-----------------------------------------------------------------*/
//	//��������                        ����ģʽ
//	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, 
//                             TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  //TIM_ICPolarity_Rising�����ز���
//  TIM_ICStructInit(&TIM_ICInitStructure);
//  TIM_ICInitStructure.TIM_ICFilter = 6;         //�Ƚ��˲���
//  TIM_ICInit(TIM3, &TIM_ICInitStructure);
//  
//	//TIM_ARRPreloadConfig(TIM3, ENABLE);
// // Clear all pending interrupts
//  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
//	//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   //ʹ���ж�
//  //Reset counter
//  TIM3->CNT =0;
//	TIM_Cmd(TIM3, ENABLE);   //ʹ�ܶ�ʱ��3
//}

//void TIM_Init(void)
//{
//  TIM3_Mode_Config();
//}

void Get_Angle(int16_t*degree)   //��ȡһ�μ���ֵ
{
	int32_t temp;
	int16_t num;
	num=TIM_GetCounter(TIM3);
	if(num>=400)   //����180��
	{
		degree[0]='-';   //����
		degree[1]=(800-num)*360/(899+1);   //��������
		temp=(800-num)*(1000000/((899+1)/2))*360-degree[1]*1000000;
		degree[2]=(unsigned char)temp;  //С������
  }
	else
	{
		degree[0]='+';
		degree[1]=num*360/(899+1);
		temp=num*(1000000/((899+1)/2))*180-degree[1]*1000000;
    degree[2]=(unsigned char)temp;
	}
}