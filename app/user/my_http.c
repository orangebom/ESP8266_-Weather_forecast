/*
 * my_http.c
 *
 *  Created on: 2015Äê7ÔÂ24ÈÕ
 *      Author: Administrator
 */
#include "my_http.h"
#include "c_types.h"
#include "client.h"
void ICACHE_FLASH_ATTR http_parse_request_url(char *URL,char *host,char *filename,unsigned short *port){
	char *PA;
	char *PB;
	memset(host,0,sizeof(host));
	memset(filename,0,sizeof(filename));
	*port=0;
	if(!(*URL)) return;
	PA=URL;
	if(!strncmp(PA,"http://",strlen("http://")))
		PA=URL+strlen("http://");
	if(!strncmp(PA,"https://",strlen("https://")))
		PA=URL+strlen("https://");
	PB=strchr(PA,'/');
	if(PB){
		memcpy(host,PA,strlen(PA)-strlen(PB));
		if(PB+1){
			memcpy(filename,PB+1,strlen(PB-1));
			filename[strlen(PB)-1]=0;
		}
		host[strlen(PA)-strlen(PB)]=0;
	}else{
		memcpy(host,PA,strlen(PA));
		host[strlen(PA)]=0;
	}
	PA=strchr(host,':');
	if(PA)
		*port=atoi(PA+1);
	else
		*port=80;



}
void ICACHE_FLASH_ATTR user_esp_dns_found(const char *name, ip_addr_t *ipaddr, void *arg){
	struct ip_info info;
	wifi_get_ip_info(STATION_IF,&info);
	my_station_init(ipaddr,&info.ip,port);
}
void ICACHE_FLASH_ATTR HttpReadFile(char *URL,char *method,char *postdata){
	struct ip_addr addr;
	memset(buffer,0,2048);
	http_parse_request_url(URL,host,filename,&port);
	if(strcmp(method,"GET")==0){
		os_sprintf(buffer,GET,filename,host);
	}else{
		os_sprintf(buffer,POST,filename,strlen(postdata),host,postdata);
	}
	espconn_gethostbyname(&user_tcp_conn,host, &addr,
	user_esp_dns_found);

}
