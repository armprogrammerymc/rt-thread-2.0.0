	
#ifndef DRV_BUTTON_H_
#define DRV_BUTTON_H_


#include "stm32f0xx.h"

/*Ĭ�ϵ�0���Ͽ�  ��1 ����*/
#define BUTTON_ON  1
#define BUTTON_OFF 0

void     BUTTON_Init(void);//IO��ʼ��
uint8_t  BUTTON_Scan(void);  //����ɨ�躯��	

#endif // DRV_BUTTON_H_
