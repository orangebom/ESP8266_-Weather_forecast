#include "wifi_set.h"
#include "c_types.h"
#include "user_interface.h"
#include "osapi.h"
#include "oled.h"
/* 获取附近wifi列表  */
void ICACHE_FLASH_ATTR scan_done(void *arg,STATUS status){
	uint8 ssid[33];
	struct station_config stationConf;

	  if (status == OK)
	  {
	    struct bss_info *bss_link = (struct bss_info *)arg;
	    bss_link = bss_link->next.stqe_next;//ignore first

	    while (bss_link != NULL)
	    {
	      os_memset(ssid, 0, 33);
	      if (os_strlen(bss_link->ssid) <= 32)
	      {
	        os_memcpy(ssid, bss_link->ssid, os_strlen(bss_link->ssid));
	      }
	      else
	      {
	        os_memcpy(ssid, bss_link->ssid, 32);
	      }
	      os_printf("+CWLAP:(%d,\"%s\",%d,\""MACSTR"\",%d)\r\n",
	     	                 bss_link->authmode, ssid, bss_link->rssi,
	     	                 MAC2STR(bss_link->bssid),bss_link->channel);

	      bss_link = bss_link->next.stqe_next;
	    }


	  }
	  else
	  {
	//  	os_sprintf(temp,"err, scan status %d\r\n", status);
	//  	uart0_sendStr(temp);

	  }

}
void to_scan(void){ wifi_station_scan(NULL,scan_done); }

void wifi_scan(void)
{
	wifi_set_opmode_current(0x01);//wifi璁剧疆涓簊tate妯″紡
	system_init_done_cb(to_scan);
}

/* 连接wifi */
void ICACHE_FLASH_ATTR Wifi_conned(uint8 *arg){
	static uint8 count=0;
	uint8 status;

	os_timer_disarm(&connect_timer);
	count++;
	status=wifi_station_get_connect_status();
	if(status==STATION_GOT_IP)
	{
		OLED_ShowString(0,0,"Wifi OK!",8);
		*arg = 1;
		os_printf("Wifi connect success!");
		return ;
	}else{
		if(count>=7){
			*arg = 0;
			OLED_ShowString(0,0,"Wifi Fail!",8);
			return ;
		}
	}



	os_timer_arm(&connect_timer,2000,NULL);
}

unsigned char wifi_Connect(char* name, char * pass)
{

	uint8 status = 0;
	struct station_config stationConf;

	os_memcpy(&stationConf.ssid, name, 32);
	os_memcpy(&stationConf.password, pass, 64);
	wifi_station_set_config_current(&stationConf);
	wifi_station_connect();
	os_timer_disarm(&connect_timer);

	os_timer_setfn(&connect_timer,Wifi_conned,&status);
	os_timer_arm(&connect_timer,2000,NULL);

	return status;

}


