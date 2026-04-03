#include "tim.h"
#define FreqTIM3 1000000
#define soKhe 15 //do da chia 2

void init_ictimer3(void){
	TIM_ICInitTypeDef in4t3;
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//chong nhieu
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	in4t3.TIM_Channel = TIM_Channel_1; //PA6
	in4t3.TIM_ICFilter = 0x4;// lay mau)
	in4t3.TIM_ICPolarity = TIM_ICPolarity_Rising; 
	in4t3.TIM_ICPrescaler = TIM_ICPSC_DIV2; //2 xung lay 1 xung (so khe giam con 15)
	in4t3.TIM_ICSelection = TIM_ICSelection_DirectTI;//lay truc tiep tu kenh A hoac B (khong hieu lam)
	
	TIM_ICInit(TIM3 , &in4t3);
//bat ngat cho ccr1
	TIM_ITConfig(TIM3 , TIM_IT_CC1 , ENABLE);
}
void init_basetimer3(void){
	TIM_TimeBaseInitTypeDef in4t3;
	
	in4t3.TIM_ClockDivision = TIM_CKD_DIV1;//khong chia
	in4t3.TIM_CounterMode = TIM_CounterMode_Up;
	in4t3.TIM_Period = 0xFFFF;
	in4t3.TIM_Prescaler = 72-1; //72Mhz => 1Mhz
	
	TIM_TimeBaseInit(TIM3 , &in4t3);
	
	TIM_Cmd(TIM3 , ENABLE);
}
void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3,TIM_IT_CC1)==SET){
		TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);
		if(captureNumber==0){
			captureNumber=1;
		//luu gia tri bat dau
			IC3ReadValue1 = TIM_GetCapture1(TIM3);
		}
		else if(captureNumber==1){
		//reset 
			captureNumber=0;
		// luu gia tri ket thuc
			IC3ReadValue2 = TIM_GetCapture1(TIM3);
		//tinh toan
			if(IC3ReadValue1 < IC3ReadValue2){
				chuKy1Khe = IC3ReadValue2 - IC3ReadValue1;
			}
			else{
				chuKy1Khe = (0xFFFF - IC3ReadValue1) + IC3ReadValue2 + 1;
			}
			RPS = FreqTIM3/(chuKy1Khe*soKhe);
			RPM = RPS*60;
			ReadyToDisplay++;
		}
		
	}
}
