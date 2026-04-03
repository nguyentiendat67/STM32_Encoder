#include "uart.h" 


/******************/
void init_uart2(void){
	GPIO_InitTypeDef in4p;
	USART_InitTypeDef in4u;
	//TX:PA2
	in4p.GPIO_Mode = GPIO_Mode_AF_PP;
	in4p.GPIO_Pin = GPIO_Pin_2;
	in4p.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&in4p);
	//RX:PA3
	in4p.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	in4p.GPIO_Pin = GPIO_Pin_3;
		GPIO_Init(GPIOA,&in4p);
	//usart2
	in4u.USART_BaudRate = 57600;//tra bang 192 RM008 tan so 36Mhz thi chia tan 57k6 vua tron
	in4u.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	in4u.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	in4u.USART_Parity = USART_Parity_No;
	in4u.USART_StopBits = USART_StopBits_1;
	in4u.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART2 , &in4u);
	//cho phep hoat dong
	USART_Cmd(USART2,ENABLE);
	//cho phep ngat noi bo
	USART_ITConfig(USART2 , USART_IT_RXNE ,ENABLE);
}
void u2_send_char(char c){
	USART_SendData(USART2 , c);
	while(USART_GetFlagStatus(USART2 , USART_FLAG_TXE) == RESET);
	//while den khi chua gui xong data
}
char u2_get_char(void){
	//while den khi nhan duoc data
	while(USART_GetFlagStatus(USART2 , USART_FLAG_RXNE)==RESET);
	return USART_ReceiveData(USART2);
}
void USART2_IRQHandler(void){
	char temp;
	if(USART_GetITStatus(USART2, USART_IT_RXNE)==SET){
		temp=u2_get_char();
		if(temp=='!'){
			checkStr=1;
			strReceive[lenStr]='\0';
		}
		else{
			strReceive[lenStr]=temp;
			lenStr++;
			
		}
	}
}

struct __FILE{
	int dummy;
};
FILE __stdout;
int fputc(int c , FILE* f){
	u2_send_char(c);
	return c;
}


