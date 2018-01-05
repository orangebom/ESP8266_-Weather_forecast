#ifndef __WIFI_SET_H__
#define __WIFI_SET_H__
#include "ets_sys.h"
ETSTimer connect_timer;


void wifi_scan_get(void);
unsigned char wifi_Connect(char* name, char * pass);

#endif
