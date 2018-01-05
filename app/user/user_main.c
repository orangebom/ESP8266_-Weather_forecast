
#include "user_main.h"
void user_init(){
	uint8 temp = 0;
	uart_init(115200,115200);
	uart0_sendStr("\r\nHello World\r\n");
	os_printf("xxxx");
	OLED_Init();
	OLED_Clear();
	//OLED_ShowString(3,2,"hello",8);
	//OLED_ShowString(0,0,"HELLO",16);
	wifi_scan();
	wifi_Connect("orange","ccc123456");

}

void user_rf_pre_init(){}



