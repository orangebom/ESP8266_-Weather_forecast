/*
 * client.h
 *
 *  Created on: 2015��7��22��
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_CLIENT_H_
#define APP_INCLUDE_CLIENT_H_
#include "user_main.h"
#include "espconn.h"
#include "mem.h"
#include "my_http.h"
char buffer[2048];
#define GET "GET /%s HTTP/1.1\r\nAccept: */*\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n"
#define POST "POST /%s HTTP/1.1\r\nAccept: */*\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n%s"

struct espconn user_tcp_conn;
void my_station_init(struct ip_addr *remote_ip,struct ip_addr *local_ip,int remote_port);


#endif /* APP_INCLUDE_CLIENT_H_ */
