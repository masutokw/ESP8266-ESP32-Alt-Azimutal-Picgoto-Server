#include <ESP8266WebServer.h>
#include "misc.h"
#ifndef WEBSERVER_H_INCLUDED
#define WEBSERVER_H_INCLUDED
#include <ESP8266WebServer.h>
#include "mount.h"
#include <FS.h>
//#define N_STYLE  " style='text-align: right;height:20px; width:70px' value ='"
//#define N_STYLE  " style='text-align: right;height:20px; width:70px; background-color:black;color: red;border:0 ' value ='"
//#define B_STYLE  " style='text-align: right; background-color:#100000;color:#900000;border-color:red; border-radius:5px'"
#define BUTT ".button_red {text-align: right; background-color:#100000;color:yellow;border-color:red; border-radius:5px}"
#define TEXTT  ".text_red {text-align: right;height:20px; width:90px; background-color:black;color: red;border:0}" 

extern ESP8266WebServer serverweb;
extern mount_t *telescope;
extern String ssi;
extern String pwd;
//void handleConfig();
//String getContentType(String filename);
//bool handleFileRead(String path);
void initwebserver(void);

#endif // WEBSERVER_H_INCLUDED
