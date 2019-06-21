#include "WiFiConnector.h"

#define DEBUG

WiFiConnector::WiFiConnector(/* args */)
{
}

WiFiConnector::~WiFiConnector()
{
}

void WiFiConnector::init()
{
    WiFi.mode(WIFI_STA);
}

void WiFiConnector::setStatus(WiFiConnectionStatus new_status)
{
    if (new_status == status)
        return;
    status = new_status;
    if (onStatusChange != nullptr)
    {
        onStatusChange(status);
    }
}

WiFiConnectionStatus WiFiConnector::getStatus()
{
    return status;
}

void WiFiConnector::setup()
{
    if (getStatus() == WF_CONNECTED)
        return;

    delay(10);
    uint32_t current_ms = millis();
    if (getStatus() != WF_CONNECTING)
    {
#ifdef DEBUG
        Serial.println();
        Serial.print(F("Connecting to \""));
        Serial.print(ssid);
        Serial.println(F("\""));
#endif
        WiFi.begin(ssid, password);
        setStatus(WF_CONNECTING);
        start_connection_ms = current_ms;
    }
}

void WiFiConnector::checkStatus()
{
    uint32_t current_ms = millis();
    WiFiConnectionStatus current_status = getStatus();

    if (WiFi.status() == WL_CONNECTED)
    {
        if (current_status == WF_CONNECTED)
            return;
#ifdef DEBUG
        Serial.println(F(""));
        Serial.println(F("WiFi connected"));
        Serial.println(F("IP address: "));
        Serial.println(WiFi.localIP());
#endif
        setStatus(WF_CONNECTED);
    }
    else
    {
        if (current_status == WF_CONNECTED)
        {
#ifdef DEBUG
            Serial.println(F("WiFi connection lost"));
#endif
            setStatus(WF_DISCONNECTED);
        }
        if (current_status == WF_CONNECTING && (uint32_t)(current_ms - start_connection_ms) > CONNECTION_TIMEOUT)
        {
#ifdef DEBUG
            Serial.println(F("Wifi not connected"));
#endif
            setStatus(WF_DISCONNECTED);
        }
    }
}

void WiFiConnector::tick()
{
    checkStatus();
}