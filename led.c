#include "led.h"
#define DIO_pin GPIO_Pin_1
#define CLK_pin GPIO_Pin_0
#define PORT GPIOA
const uint8_t display_digit[]={
	//dgfedcba
    0x3f, // 0	/0011 1111
    0x06, // 1	/0000 0110
    0x5b, // 2	/0101 1011
    0x4f, // 3	/0100 1111
    0x66, // 4	/0110 0110
    0x6d, // 5	/0110 1101
    0x7d, // 6	/0111 1101
    0x07, // 7	/0000 0111
    0x7f, // 8	/0111 1111
    0x6f, // 9	/0110 1111
	0x00  // 10 /tat het
};


void start_signal(void){
	/*When CLK is a high level and DIO changes from high to low level, 
	data input starts. */
    GPIO_SetBits(PORT, DIO_pin);
    GPIO_SetBits(PORT, CLK_pin);
    GPIO_ResetBits(PORT, DIO_pin);
	
}
void stop_signal(void){
	/*When CLK is a high level and DIO changes from low level to high level,
	data input ends.*/
	//keo clk xuong roi len cho tranh nhieu
	GPIO_ResetBits(PORT, CLK_pin);
    GPIO_ResetBits(PORT, DIO_pin);
    GPIO_SetBits(PORT, CLK_pin);
    GPIO_SetBits(PORT, DIO_pin);
}
void write_byte(uint8_t data){
	for(uint8_t i=0 ; i<8 ; i++){
		GPIO_ResetBits(PORT, CLK_pin);
		if(data&1) //10101010 
				//c t c t c t c t
			GPIO_SetBits(PORT, DIO_pin);
		else
			GPIO_ResetBits(PORT, DIO_pin);
		GPIO_SetBits(PORT, CLK_pin);
		data>>=1;
	}
//ACK gi do
	GPIO_ResetBits(PORT, CLK_pin);
    GPIO_SetBits(PORT, CLK_pin);	
}
void data_command(void){
	//data command
    start_signal();
	//0100 0100  normal mode - fix address - write data to display
    write_byte(0x44);
    stop_signal();	
}
void display_control(void){
	//Display control : Display on - muc sang 3
    start_signal();
    write_byte(0x88 + 3); 
    stop_signal();
}
void display_number(uint8_t pos, uint8_t number){

	start_signal();
//Address command
	//1100 0pos   pos=0-3
    write_byte(0xC0 + pos); 
//Data
    write_byte(display_digit[number]);
    stop_signal();

}

void init_led_pin(void){
	GPIO_InitTypeDef in4;
	in4.GPIO_Mode = GPIO_Mode_Out_PP;
	in4.GPIO_Pin = DIO_pin | CLK_pin;
	in4.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT , &in4);
}

void hien_thi_led7(uint32_t temp){
		data_command();
		for(uint8_t j=0 ; j<=3 ; j++){
		if(temp>0){
			display_number(3-j,temp%10);
			temp/=10;
		}else{
			display_number(3-j,10);
		}
	}
		display_control();
}

