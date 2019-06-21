#ifndef WIFI_CONNECTOR
#define WIFI_CONNECTOR

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

const char ssid[32] = "********";
const char password[32] = "*********";

const uint16_t CONNECTION_TIMEOUT = 30000;

enum WiFiConnectionStatus
{
  WF_DISCONNECTED,
  WF_CONNECTING,
  WF_CONNECTED
};

class WiFiConnector
{
private:
  WiFiConnectionStatus status = WF_DISCONNECTED;
  uint32_t start_connection_ms = 0;

  void setStatus(WiFiConnectionStatus);
  void checkStatus();

public:
  WiFiConnector(/* args */);
  ~WiFiConnector();

  void init();
  WiFiConnectionStatus getStatus();
  void (*onStatusChange)(WiFiConnectionStatus);
  void setup();
  void tick();
};

#endif
