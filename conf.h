#ifndef CONF_H_INCLUDED
#define CONF_H_INCLUDED
#if defined(ESP8266)
#define esp8266
#include <ESP8266WiFi.h>
#include <lwip/napt.h>
#include <lwip/dns.h>
#include <dhcpserver.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "sntp.h"
#include <user_interface.h>
#define NAPT 1000
#define NAPT_PORT 10

#else 
#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#include <SPIFFS.h>
#include "BluetoothSerial.h"

#endif
//optional

//#define NUNCHUCK_CONTROL
#define IR_CONTROL
//#define PAD
//#define OLED_DISPLAY
//#define OTA
//#define FIXED_IP 21


//mandatory
#define EPOCH_1_1_2019 1546300800
#define SERVER_PORT 10001
#define WEB_PORT 80
#define BAUDRATE 19200
#define MAX_SRV_CLIENTS 3
#define SPEED_CONTROL_TICKER 20
#define COUNTERS_POLL_TICKER 100
#endif
