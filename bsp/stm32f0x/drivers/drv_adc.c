/* Includes ------------------------------------------------------------------*/	
#include "stm32f0xx.h"
#include "drv_adc.h"
#include "usart.h"

unsigned int after_value[5];
unsigned int average_value[5];

#define ADC1_DR_ADDRESS     ((uint32_t)0x40012440)


void USART_Put_Char(unsigned char ch)
{
   USART_SendData(USART1,ch);
  while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

void ADC_DMA_Config(void)
{ 
      GPIO_InitTypeDef  GPIO_InitStructure;
      ADC_InitTypeDef   ADC_InitStructure;
      DMA_InitTypeDef   DMA_InitStructure;
 
      ADC_DeInit(ADC1);


      /* RCC Config */
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
      /* GPIO Config */
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
 
 
       /* DMA1 Config */
       DMA_DeInit(DMA1_Channel1);
       DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
       DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&after_value;
       DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
       DMA_InitStructure.DMA_BufferSize = 5;
       DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
       DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
       DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
       DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
       DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
       DMA_InitStructure.DMA_Priority = DMA_Priority_High;
       DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
       DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
       DMA_Cmd(DMA1_Channel1, ENABLE);
  
       ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  
       ADC_DMACmd(ADC1, ENABLE);//Enable ADC_DMA
  
       /* ADC1 Config */
       ADC_StructInit(&ADC_InitStructure);
       ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
       ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
       ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
       //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO;
       ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
       ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
       ADC_Init(ADC1, &ADC_InitStructure); 
  
       ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_239_5Cycles);
       ADC_ChannelConfig(ADC1, ADC_Channel_2, ADC_SampleTime_239_5Cycles);
       ADC_ChannelConfig(ADC1, ADC_Channel_3, ADC_SampleTime_239_5Cycles);
       ADC_ChannelConfig(ADC1, ADC_Channel_4, ADC_SampleTime_239_5Cycles);
       ADC_ChannelConfig(ADC1, ADC_Channel_5, ADC_SampleTime_239_5Cycles);
   
       ADC_GetCalibrationFactor(ADC1);
      
       ADC_Cmd(ADC1, ENABLE);
  
       while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));
  
       ADC_StartOfConversion(ADC1);
}

void Read_ADC(void)
{
    unsigned char a,b,c,d,i;
 
    for(i=1;i<6;i++)
    {      
       while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET);//Test DMA1 TC flag 
       DMA_ClearFlag(DMA1_FLAG_TC1);//Clear DMA TC flag      
       average_value[i] = (unsigned int)((after_value[i]*3300)/0x0FFF); 
    
       a = average_value[i]/1000;
       b = (average_value[i] - a*1000)/100;
       c = (average_value[i] - a*1000 - b*100)/10;
       d = average_value[i] - a*1000 - b*100 - c*10;
       USART_Put_Char(0x56);//"V"
       USART_Put_Char(i+48);
       USART_Put_Char(0x3D);//"="
       USART_Put_Char(a+48);
       USART_Put_Char(0x2e);//"."
       USART_Put_Char(b+48);
       USART_Put_Char(c+48);
       USART_Put_Char(d+48);
       USART_Put_Char(0x56);//"V"
       USART_Put_Char(0xD);
       USART_Put_Char(0xA);   
  }
}



//void USART_Config()
//{
//  GPIO_InitTypeDef   GPIO_InitStructure;
//  USART_InitTypeDef USART_InitStructure;
// 
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
// 
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource14,GPIO_AF_1);
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_1);
// 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA,&GPIO_InitStructure);
// 
//  USART_InitStructure.USART_BaudRate = 9600;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_No;
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
//  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
//  USART_Init(USART1,&USART_InitStructure);//��������
//  //USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//  USART_Cmd(USART1,ENABLE);
//}


//#define DMA_BUFFER_SIZE     6
//uint8_t sample_finish = 0;
//int16_t adc_dma_tab[6] = { 0 };
//uint8_t sample_index = 0;

////����������
//int16_t sample_1[128] = { 0 };
//int16_t sample_2[128] = { 0 };
//int16_t sample_3[128] = { 0 };
//int16_t sample_4[128] = { 0 };
//int16_t sample_5[128] = { 0 };
//int16_t sample_6[128] = { 0 };

//void adc_config()
//{
//    ADC_InitTypeDef adc_init_structure;

//		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);				// ʹ��GPIOʱ��
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);            //ʹ��ADC1

//    ADC_DeInit(ADC1);                                               //��λADC
//    ADC_StructInit(&adc_init_structure);                            //��ʼ��ADC�ṹ��

//    adc_init_structure.ADC_ContinuousConvMode = ENABLE;            //����ת��ģʽ
//    adc_init_structure.ADC_DataAlign = ADC_DataAlign_Right;         //���������Ҷ���
//    adc_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; //�ⲿ��������ΪTIM2
//    adc_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;//�����ش���
//    adc_init_structure.ADC_Resolution = ADC_Resolution_12b;         //12λ�ֱ���
//    adc_init_structure.ADC_ScanDirection = ADC_ScanDirection_Upward;//����ɨ��0-18ͨ��
//    ADC_Init(ADC1, &adc_init_structure);

//    ADC_OverrunModeCmd(ADC1, ENABLE);                               //ʹ�����ݸ���ģʽ
//    ADC_ChannelConfig(ADC1, ADC_Channel_0 | ADC_Channel_1 | ADC_Channel_2
//                          | ADC_Channel_8 | ADC_Channel_14 | ADC_Channel_15,
//                          ADC_SampleTime_13_5Cycles);               //���ò���ͨ��������ʱ��125nS
//    ADC_GetCalibrationFactor(ADC1);                                 //ʹ��ǰУ׼ADC
//    ADC_Cmd(ADC1, ENABLE);                                          //ʹ��ADC1
//    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET);         //�ȴ�ADC1ʹ�����

//    ADC_DMACmd(ADC1, ENABLE);                                       //ʹ��ADC_DMA
//    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);           //����DMA����ģʽΪѭ��ģʽ
//    ADC_StartOfConversion(ADC1);                                    //����һ��ת�������룩
//}

//void adc_gpio_init()
//{
//    GPIO_InitTypeDef gpio_init_structure;
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE);

//    GPIO_StructInit(&gpio_init_structure);
//    //GPIOA                                                         //PA-1~5����ADC
//    gpio_init_structure.GPIO_Pin = (GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
//    gpio_init_structure.GPIO_Mode = GPIO_Mode_AN;                   //ʹ�ܸ��ӣ�ģ�⣩����
//    gpio_init_structure.GPIO_OType = GPIO_OType_PP;                 //�������
//    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;              //Fast speed
//    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_UP;                    //����
//    GPIO_Init(GPIOA, &gpio_init_structure);
//}

//void adc_dma_init()
//{
//    DMA_InitTypeDef dma_init_structure;
//    NVIC_InitTypeDef nvic_init_structure;

//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);              //ʹ��DMAʱ��

//    nvic_init_structure.NVIC_IRQChannel = DMA1_Channel1_IRQn;       //ѡ��DMA1ͨ���ж�
//    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;                //�ж�ʹ��
//    nvic_init_structure.NVIC_IRQChannelPriority = 0;                //���ȼ�����Ϊ0
//    NVIC_Init(&nvic_init_structure);

//    DMA_DeInit(DMA1_Channel1);                                      //��λDMA1_channel1
//    DMA_StructInit(&dma_init_structure);                            //��ʼ��DMA�ṹ��

//    dma_init_structure.DMA_BufferSize = DMA_BUFFER_SIZE;            //DMA���������С����
//    dma_init_structure.DMA_DIR = DMA_DIR_PeripheralSRC;             //DMA����������Ϊ����Դ
//    dma_init_structure.DMA_M2M = DISABLE;                           //�ڴ浽�ڴ����
//    dma_init_structure.DMA_MemoryBaseAddr = (uint32_t)&adc_dma_tab[0];//��������������ʼ��ַ
//    dma_init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݴ�С����ΪHalfword
//    dma_init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable;        //�ڴ��ַ����
//    dma_init_structure.DMA_Mode = DMA_Mode_Circular;                //DMAѭ��ģʽ������ɺ����¿�ʼ����
//    dma_init_structure.DMA_PeripheralBaseAddr = (uint32_t) &(ADC1->DR);//ȡֵ�������ַ
//    dma_init_structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//����ȡֵ��С����ΪHalfword
//    dma_init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ��������
//    dma_init_structure.DMA_Priority = DMA_Priority_High;             //DMA���ȼ�����Ϊ��
//    DMA_Init(DMA1_Channel1, &dma_init_structure);

//    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);                  //ʹ��DMA�ж�
//    DMA_ClearITPendingBit(DMA_IT_TC);                                //���һ��DMA�жϱ�־
//    DMA_Cmd(DMA1_Channel1, ENABLE);                                  //ʹ��DMA1
//}

//void adc_timer_init()
//{
//    TIM_TimeBaseInitTypeDef timer_init_structure;
//    NVIC_InitTypeDef nvic_init_structure;

//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);            //ʹ��TIM2ʱ��

//    nvic_init_structure.NVIC_IRQChannel = TIM2_IRQn;                //ѡ��TIM2�ж�ͨ��
//    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;                //ʹ��TIM2�ж�
//    nvic_init_structure.NVIC_IRQChannelPriority = 0;                //���ȼ�Ϊ0
//    NVIC_Init(&nvic_init_structure);

//    TIM_DeInit(TIM2);                                               //��λTIM2
//    TIM_TimeBaseStructInit(&timer_init_structure);                  //��ʼ��TIMBASE�ṹ��

//    timer_init_structure.TIM_ClockDivision = TIM_CKD_DIV1;          //ϵͳʱ�ӣ�����Ƶ,48M
//    timer_init_structure.TIM_CounterMode = TIM_CounterMode_Up;      //���ϼ���ģʽ
//    timer_init_structure.TIM_Period = 312;                          //ÿ312 uS����һ���ж�,����ADC
//    timer_init_structure.TIM_Prescaler = 48-1;                      //����ʱ��Ԥ��Ƶ,f=1M,systick=1 uS
//    timer_init_structure.TIM_RepetitionCounter = 0x00;              //����0+1��update�¼������ж�
//    TIM_TimeBaseInit(TIM2, &timer_init_structure);

//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                      //ʹ��TIM2�ж�
//    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);           //ѡ��TIM2��update�¼�����Ϊ����Դ

//    TIM_Cmd(TIM2, ENABLE);                                          //ʹ��TIM2
//}

////void user_adc_init()
//void rt_hw_adc_init(void)
//{
//    adc_gpio_init();
//    adc_config();               // ע���ʼ��˳�򣬷������������������׳������ݴ�λ�Ľ��
//    adc_dma_init();             //
//    adc_timer_init();           //

//}


///****************************�жϷ������****************************/
//void TIM2_IRQHandler()
//{
//    if(TIM_GetITStatus(TIM2, TIM_IT_Update))            //�жϷ���update�¼��ж�
//    {
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //���update�¼��жϱ�־
//    }
//}

//void DMA1_Channel1_IRQHandler()
//{
//    if(DMA_GetITStatus(DMA_IT_TC))                      //�ж�DMA��������ж�
//    {
//        if(sample_finish == 0)
//        {
//            sample_1[sample_index] = adc_dma_tab[0];
//            sample_2[sample_index] = adc_dma_tab[1];
//            sample_3[sample_index] = adc_dma_tab[2];
//            sample_4[sample_index] = adc_dma_tab[3];
//            sample_5[sample_index] = adc_dma_tab[5];
//            sample_6[sample_index] = adc_dma_tab[4];
//            sample_index++;
//        }
//        if(sample_index >= 128)                         //ע���ֹ����Խ�絼��δ֪����
//        {
//            sample_index = 0;
//            TIM_Cmd(TIM2, DISABLE);                     //����ܲ�������ֹͣ��ʱ��
//            DMA_Cmd(DMA1_Channel1, DISABLE);            //����ܲ�������ֹͣDMA
//            sample_finish = 1;                          //�ò�����ɱ�־λ
//        }
//    }
//    DMA_ClearITPendingBit(DMA_IT_TC);                   //���DMA�жϱ�־λ
//}

//uint32_t get_volt(uint32_t adc)
//{
//	return(adc*3300/4096);
//}



/* Initial components for device */
//INIT_DEVICE_EXPORT(rt_hw_adc_init);