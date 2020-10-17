#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <time.h>
#include "sntp.h"
#include <Ticker.h>
#include <Wire.h>
#include "misc.h"
#include "mount.h"
#include "webserver.h"
#include "taki.h"
#include "piclevel.h"
//Comment out undesired Feature
//---------------------------
#define NUNCHUCK_CONTROL
//#define FIXED_IP 21
//#define OLED_DISPLAY
//#define PAD
//--------------------------------
#ifdef  NUNCHUCK_CONTROL
#include "nunchuck.h"
#endif
#define OTA
#ifdef OTA
#include <ArduinoOTA.h>
#include "OTA_helper.hpp"
#endif
#define EPOCH_1_1_2019 1546300800
#define BAUDRATE 19200
#define MAX_SRV_CLIENTS 3
#define SPEED_CONTROL_TICKER 10
#define COUNTERS_POLL_TICKER 100
#include <FS.h>
extern long sdt_millis;
//#include "wifipass.h" //comment wifipass.h and uncomment for your  wifi parameters
const char* ssid = "MyWIFI";
const char* password = "Mypassword";
extern volatile int state;
WiFiServer server(10001);
WiFiClient serverClients[MAX_SRV_CLIENTS];
//IPAddress serv(192, 168, 4, 1);
ESP8266WebServer serverweb(80);
ESP8266HTTPUpdateServer httpUpdater;
char buff[50] = "Waiting for connection..";
extern char  response[200];
mount_t *telescope;
c_star volatile st_now, st_target, st_current, st_1, st_2;
String ssi;
String pwd;
Ticker speed_control_tckr, counters_poll_tkr;
extern long command( char *str );
time_t now;
time_t init_time;
#ifdef PAD
#include "pad.h"
#endif

#ifdef OLED_DISPLAY
#include "SSD1306.h"
#include "oled.h"
//#include "SH1106.h"
//SSD1306 display(0x3c, D5, D6);
#endif


int net_task(void)
{
  int lag = millis();
  size_t n;
  uint8_t i;
  //Sky Safari does not make a persistent connection, so each commnad query is managed as a single independent client.
  if (server.hasClient())
  {
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
    {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected())
      {
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        continue;
      }
    }
    //Only one client at time, so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  //check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++)
  {
    if (serverClients[i] && serverClients[i].connected())
    {
      if (serverClients[i].available())
      {
        //get data from the  client and push it to LX200 FSM

        while (serverClients[i].available())
        {
          delay(1);
          size_t n = serverClients[i].available();
          serverClients[i].readBytes(buff, n);
          command( buff);
          buff[n] = 0;
          serverClients[i].write((char*)response, strlen(response));

          //checkfsm();
        }

      }
    }
  }
  return millis() - lag;
}


void setup()
{

#ifdef OLED_DISPLAY
  oled_initscr();
#endif

 
 WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("ESP-PGT", "boquerones");
  SPIFFS.begin();
  File f = SPIFFS.open("/wifi.config", "r");
  if (f)
  {
    ssi = f.readStringUntil('\n');
    pwd = f.readStringUntil('\n');
    f.close();
    char  ss [ssi.length() + 1];
    char  pw [pwd.length() + 1];
    ssi.toCharArray(ss, ssi.length() + 1);
    pwd.toCharArray(pw, pwd.length() + 1);
    pw[pwd.length() + 1] = 0;
    ss[ssi.length() + 1] = 0;

    WiFi.begin((const char*)ss, (const char*)pw);
  }
  else {SPIFFS.format(); 
  WiFi.begin(ssid, password);
  }
  f = SPIFFS.open("/network.config", "r");
  if (f)
  { IPAddress ip;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress dns;
    if (ip.fromString(f.readStringUntil('\n')) && subnet.fromString(f.readStringUntil('\n')) && gateway.fromString(f.readStringUntil('\n')) + dns.fromString(f.readStringUntil('\n'))) {
      WiFi.config(ip, gateway, subnet, dns);
    }

    f.close();
  }
#ifdef FIXED_IP
  IPAddress ip(192, 168, 1, FIXED_IP);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 0, 0);
  //  IPAddress DNS(192, 168, 1, 1);
  WiFi.config(ip, gateway, subnet, gateway);
#endif

  delay(500);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if (i == 21)
  {
    //hile (1) delay(500);
  }
  if  (WiFi.status() != WL_CONNECTED) WiFi.disconnect(true);
#ifdef OLED_DISPLAY
  oled_waitscr();
#endif

  //start UART and the server
  Serial.begin(BAUDRATE);
#ifdef OLED_DISPLAY
  // Serial.swap();
#endif
  //
  server.begin();
  server.setNoDelay(true);
  telescope = create_mount();
readconfig(telescope);
  httpUpdater.setup(&serverweb);

  config_NTP(telescope->time_zone, 0);

  if  (WiFi.status() == WL_CONNECTED)
  { int cn = 0;  now = time(nullptr);
    while ((now < EPOCH_1_1_2019) && (cn++) < 5) {
      delay(500);
      now = time(nullptr);
    }
    init_time = time(nullptr);
   
  }
  initwebserver();
  tak_init(telescope);
  speed_control_tckr.attach_ms(SPEED_CONTROL_TICKER, thread_motor2, telescope);
  counters_poll_tkr.attach_ms(COUNTERS_POLL_TICKER, track, telescope);

#ifdef PAD
  pad_Init();
#endif //PAD
#ifdef NUNCHUCK_CONTROL
  // nunchuck_init(D6, D5);
  nunchuck_init(2, 0);
#endif
#ifdef OTA
  InitOTA();
#endif

}

void loop()
{
  delay(10);
  net_task();
  now = time(nullptr);
  serverweb.handleClient();

#ifdef  NUNCHUCK_CONTROL
  nunchuck_read();
#endif

#ifdef OLED_DISPLAY
  oledDisplay();
#endif
#ifdef PAD
  doEvent();
#endif

#ifdef OTA
  ArduinoOTA.handle();
#endif

}
