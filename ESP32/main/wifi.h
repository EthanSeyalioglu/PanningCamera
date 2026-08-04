#ifndef __WIFI_H
#define __WIFI_H

#include "esp_wifi.h"
#include "esp_log.h"

extern const char *TAG;

void wifi_init_sta(void);

#endif