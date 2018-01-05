
#include "user_main.h"



void user_init(){
	uint8 temp = 0;
	struct ip_info info;



	uart_init(115200,115200);
	os_printf("\r\nESP8266 start\r\n");
	OLED_Init();
	OLED_Clear();
	OLED_ShowString(0,0,"OLED OK",8);

	wifi_Connect("orang","ccc123456");
	//wifi_set_UDP("esp8266","123456789");


}

void user_rf_pre_init(){}



