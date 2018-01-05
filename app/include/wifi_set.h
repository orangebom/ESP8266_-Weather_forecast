#ifndef __WIFI_SET_H__
#define __WIFI_SET_H__
#include "ets_sys.h"
#include "espconn.h"

struct softap_config config; //ªÒ»°µƒWIFI≈‰÷√
struct espconn user_udp_espconn;

ETSTimer udp_timer;
ETSTimer connect_timer;
struct ip_info info;



char * wifi_UDP_Radio(unsigned int local_port,unsigned int remote_port,char* sendbuf);
void wifi_scan_get(void);
unsigned char wifi_Connect(char* name, char * pass);
void wifi_set_UDP(char* name, char* pass);

#endif
