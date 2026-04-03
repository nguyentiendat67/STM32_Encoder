/*Dung 
UART2 TX:PA2 RX:PA3

TIM3  CH1:PA6

LED TM1637 DIO PA1 
			CLK PA0
*/

#include <stm32f10x.h>
#include <string.h>
#include "uart.h"
#include "tim.h"
#include "led.h"
#include "flash.h"

/*************************/
//cac bien cho uart2
char strReceive[MAX_ID];//chuoi
volatile uint8_t lenStr=0;//luu do dai cua chuoi
volatile uint8_t checkStr=0;//da nhan chuoi = 1

//cac bien chotim3
volatile uint8_t captureNumber=0;
volatile uint16_t IC3ReadValue1;
volatile uint16_t IC3ReadValue2;

//cac bien tinh toc do
volatile uint8_t ReadyToDisplay=0;
volatile uint32_t chuKy1Khe;
volatile uint32_t RPS;
volatile uint32_t RPM;

//cac bien cho he so hieu chinh toc do
volatile uint32_t PhanNguyen;
volatile uint32_t PhanThapPhan;
volatile uint32_t HeSo;

/*************************/
void init_clock(void);
void init_nvic(void);
void print_thong_tin_nhom(void);
void lay_he_so_hieu_chinh(void);

/*************************/

int main(){
//cac ham khoi tao
	init_clock();
	init_uart2();
	init_nvic();
	init_basetimer3();
	init_ictimer3();
	init_led_pin();
//clear cac bien
	lenStr=0;
	checkStr=0;
//lay he so hieu chinh trong flash
	doc_flash();
	if(HeSo==0) HeSo=1;

	printf("bat dau!!!\n");
	
	while(1){
		if(checkStr==1){
			TIM_ITConfig(TIM3 , TIM_IT_CC1 , DISABLE);//tat ngat cua timer
			
			if(strstr(strReceive,"INFO_REQUEST")!=NULL)
			{
				print_thong_tin_nhom();
			}
			else if(strstr(strReceive,"SET_CALIBRATION_FACTOR")!=NULL)
			{		
				lay_he_so_hieu_chinh();//tach tu string
				ghi_flash();//luu vao flash
			}
			else
			{
				printf("Khong Hop Le!!!\n");
			}
			checkStr=0;
			lenStr=0;
			
			TIM_ITConfig(TIM3 , TIM_IT_CC1 , ENABLE);//bat lai ngat cua timer
		}
		if (ReadyToDisplay >=20)
		{
			TIM_ITConfig(TIM3 , TIM_IT_CC1 , DISABLE);
			//nhan he so
			RPM = RPM*PhanNguyen + RPM*PhanThapPhan/HeSo;
			hien_thi_led7(RPM);
			if(RPM<=2000)
			{
				printf("MOTOR_SPEED: %d RPM\n", RPM);
				//hien 
			}
			else
			{
				printf("WARNING: Speed Exceeded Limit!\n");
			}
			ReadyToDisplay = 0;
			TIM_ITConfig(TIM3 , TIM_IT_CC1 , ENABLE);
		}
		

	}
	
}

/**************************/
void print_thong_tin_nhom(void){
	printf("GROUP_NAME: NHOM 7\n");
	printf("MENBERS: Nhat, Dat, Hung, Khanh\n");
	printf("PROJECT_NAME: Do toc do dong co\n");
	printf("LINH_KIEN: stm32f103c8, tm1637, cp2102, motor co encoder\n");
	printf("SOFTWARE_USED: KeilC, ChatGPT, Gemini, STM32F10x_StdPeriph_Lib_V3.6.0\n");
	printf("NGAY: 17/11/2025\n");
}
void lay_he_so_hieu_chinh(void){
	uint16_t viTri=23;//chuoi kia co 23 ki tu
	uint16_t leng = strlen(strReceive);
	uint8_t kyTu;
	//xoa bien cu
	PhanNguyen=0;
	PhanThapPhan=0;
	HeSo=1;
	//lay phan truoc dau phay
	while(viTri<leng){
		kyTu = strReceive[viTri];
		
		if(kyTu=='.') break;
		if(kyTu>='0' && kyTu<='9' ){
			kyTu = kyTu - 48;
			PhanNguyen = PhanNguyen*10 +kyTu;
		}
		viTri++;
	}
	//lay phan sau dau phay
	while(viTri<leng){
		kyTu = strReceive[viTri];
		
		if(kyTu>='0' && kyTu<='9' ){
			kyTu = kyTu - 48;
			PhanThapPhan = PhanThapPhan*10 +kyTu;
			HeSo = HeSo * 10;
		}
		viTri++;
	}
	
	if(HeSo==0) HeSo=1;
	
}

void init_clock(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
}
void init_nvic(void){
	
	NVIC_InitTypeDef in4Nvic;
	//chua biet uu tien cai nao
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//NVIC_EnableIRQ(USART2_IRQn);
	in4Nvic.NVIC_IRQChannel = USART2_IRQn;
	in4Nvic.NVIC_IRQChannelCmd = ENABLE;
	in4Nvic.NVIC_IRQChannelPreemptionPriority=0;
	in4Nvic.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&in4Nvic);
//NVIC_EnableIRQ(TIM3_IRQn);	
	in4Nvic.NVIC_IRQChannel = TIM3_IRQn;
	in4Nvic.NVIC_IRQChannelCmd = ENABLE;
	in4Nvic.NVIC_IRQChannelPreemptionPriority = 1;
	in4Nvic.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&in4Nvic);	
}


