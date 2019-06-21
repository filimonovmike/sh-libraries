#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <PubSubClient.h>
#include "WiFiConnector.h"

const char MQTT_SERVER[30] = "192.168.1.100"; //адрес сервера MQTT
const char MQTT_ID[20] = "WS-bedroom";
const char MQTT_USER[20] = "********";
const char MQTT_PSW[20] = "********";
const uint16_t MQTT_PORT = 1883;

const char MQTT_TEMPERATURE_TOPIC[] = "home/bedroom/sensor/temperature";
const char MQTT_HUMIDITY_TOPIC[] = "home/bedroom/sensor/humidity";
const char MQTT_DEVICE_STATUS_TOPIC[] = "home/bedroom/status/WS-bedroom";

const uint32_t MQTT_DATA_SEND_DELAY = 60 * 1000;
const uint32_t MQTT_RECONNECT_DELAY = 5 * 1000;
const uint32_t MQTT_CONNECT_DELAY = 60 * 1000;
const uint16_t M_CONNECTION_TIMEOUT = 30000;

enum MQTTConnectionStatus
{
    M_DISCONNECTED,
    M_CONNECTING,
    M_CONNECTED
};

enum MQTTDeviceStatus
{
    DEVICE_CHECK = '0',
    DEVICE_CONNECTED = '1',
    DEVICE_FAIL = '2'
};

class MQTTClient
{
  private:
    PubSubClient *client;
    WiFiConnector *wifi;

    MQTTConnectionStatus status = M_DISCONNECTED;
    uint32_t last_send_mqtt_data_ms = -MQTT_DATA_SEND_DELAY;
    uint32_t last_connect_attempt = -MQTT_CONNECT_DELAY;
    uint32_t last_reconnect_attempt = -MQTT_RECONNECT_DELAY;
    void checkConnectionStatus();
    uint32_t start_connection_ms = 0;
    void setStatus(MQTTConnectionStatus new_status);

  public:
    MQTTClient(WiFiConnector &wifi);
    ~MQTTClient();

    void loop();
    void reconnect();
    MQTTConnectionStatus getStatus();
    void publish(const char *topic, const char *payload);

    void publishTemperature(const char *payload);
    void publishHumidity(const char *payload);
};

#endif
