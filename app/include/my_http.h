/*
 * my_http.h
 *
 *  Created on: 2015��7��24��
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_MY_HTTP_H_
#define APP_INCLUDE_MY_HTTP_H_

void HttpReadFile(char *URL,char *method,char *postdata);
char host[32];
char filename[128];
unsigned short port;



#endif /* APP_INCLUDE_MY_HTTP_H_ */
