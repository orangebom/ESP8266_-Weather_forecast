#include "wifi_set.h"
#include "c_types.h"
#include "user_interface.h"
#include "osapi.h"
#include "oled.h"
#include "mem.h"

char data_temp[128];

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

/*
 * 获取附近wifi并打印
 */
void wifi_scan(void)
{
	wifi_set_opmode_current(0x01);//wifi璁剧疆涓簊tate妯″紡
	system_init_done_cb(to_scan);
}

/* 连接wifi */
void ICACHE_FLASH_ATTR Wifi_conned(uint8 *arg){
	static uint8 count=0;
	uint8 status;
	const char remote_ip[4] = {192,168,43,44};
	os_timer_disarm(&connect_timer);
	count++;
	status=wifi_station_get_connect_status();
	if(status==STATION_GOT_IP)
	{
		OLED_ShowString(0,4,"Wifi OK!",8);
		*arg = 1;
		os_printf("Wifi connect success!\n");

		HttpReadFile("http://www.sojson.com/open/api/weather/json.shtml?city=桂林","GET","NULL");
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
/*
 * wifi 连接
 */
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

//发送定时器回调函数
void ICACHE_FLASH_ATTR user_udp_send(void){
	char hwaddr[6];
	char DeviceBuffer[40]={0};
	wifi_get_macaddr(STATION_IF,hwaddr);
	os_sprintf(DeviceBuffer,"设备MAC地址："MACSTR"!!\r\n",MAC2STR(hwaddr));
	espconn_sent(&user_udp_espconn,DeviceBuffer,os_strlen(DeviceBuffer));
}

//接收回调函数
void ICACHE_FLASH_ATTR user_udp_recv_cb(void *arg,
char *pdata,
unsigned short len){
	unsigned short i;
	for(i = 0; i<len; i++)
		data_temp[i] = pdata[i] ;

	os_printf("udp receiver:%s\n",pdata);
}

//发送回调函数
void ICACHE_FLASH_ATTR user_udp_sent_cb(void *arg){
	os_printf("send ok\n");
	os_timer_disarm(&udp_timer);
	os_timer_setfn(&udp_timer,user_udp_send,NULL);
	os_timer_arm(&udp_timer,1000,NULL);
}

/* 发送UDP广播
 * 必须要wifi_Connect之后使用
 */
char * wifi_UDP_Radio(unsigned int local_port,unsigned int remote_port,char* sendbuf)
{
	user_udp_espconn.type=ESPCONN_UDP;
	user_udp_espconn.proto.udp=(esp_udp *)os_zalloc(sizeof(esp_udp));
	user_udp_espconn.proto.udp->local_port=local_port;  //本地端口
	user_udp_espconn.proto.udp->remote_port=remote_port; //远程端口
	const char udp_remote_ip[4]={255,255,255,255};//远程IP 发送到所有的主机上
	os_memcpy(user_udp_espconn.proto.udp->remote_ip,udp_remote_ip,4);
	espconn_regist_recvcb(&user_udp_espconn,user_udp_recv_cb);
	espconn_regist_sentcb(&user_udp_espconn,user_udp_sent_cb);
	espconn_create(&user_udp_espconn);
	user_udp_send();
	//espconn_sent(&user_udp_espconn,sendbuf,os_strlen(sendbuf));
	os_timer_disarm(&udp_timer);
	os_timer_setfn(&udp_timer,user_udp_send,NULL);
	os_timer_arm(&udp_timer,1000,NULL);
	return data_temp;
}

void wifi_set_UDP(char* name, char* pass)
{

	wifi_set_opmode(0x03);   //设置AP与status兼容模式

	wifi_softap_get_config(&config);  //获取wifi配置
	os_memcpy(config.ssid,name,strlen(name));  //写入flash
	os_memcpy(config.password,pass,strlen(pass));
	config.ssid_len=strlen(name);
	config.authmode=AUTH_WPA_WPA2_PSK;
	config.beacon_interval=100;
	config.channel=1;
	config.max_connection=4;
	config.ssid_hidden=0;
	wifi_softap_set_config(&config);
}



