#include "MQTTClient.h"

WiFiClient espClient;
PubSubClient client_e(MQTT_SERVER, MQTT_PORT, espClient);

MQTTClient::MQTTClient(WiFiConnector &wifiConnector)
{
    wifi = &wifiConnector;
    WiFiClient espClient = WiFiClient();

    client = &client_e;
}

MQTTClient::~MQTTClient()
{
}

MQTTConnectionStatus MQTTClient::getStatus()
{
    return status;
}

void MQTTClient::reconnect()
{
    if (status == M_CONNECTED)
    {
        return;
    }
    if (status != M_CONNECTING)
    {
        Serial.println();
        Serial.print(F("Connecting to MQTT Broker "));
        Serial.print(MQTT_SERVER);
        Serial.println();
        uint32_t current_ms = millis();

        setStatus(M_CONNECTING);
        start_connection_ms = current_ms;
    }

    client->connect(MQTT_ID, MQTT_USER, MQTT_PSW);
}

void MQTTClient::setStatus(MQTTConnectionStatus new_status)
{
    if (status == new_status)
    {
        return;
    }
    status = new_status;
    if (new_status == M_CONNECTED)
    {
        Serial.println(F("MQTT connected"));
        client->publish(MQTT_DEVICE_STATUS_TOPIC, "1");
        client->subscribe(MQTT_DEVICE_STATUS_TOPIC, 1);
    }
}

void MQTTClient::checkConnectionStatus()
{
    if (client->connected())
    {
        if (status != M_CONNECTED)
        {
            setStatus(M_CONNECTED);
        }
    }
    else
    {
        uint32_t current_ms = millis();
        if (status == M_CONNECTED)
        {
            Serial.println(F("MQTT connection lost"));
            setStatus(M_DISCONNECTED);
        }
        if (status == M_CONNECTING && (uint32_t)(current_ms - start_connection_ms) > M_CONNECTION_TIMEOUT)
        {
            Serial.println(F("MQTT not connected"));
            setStatus(M_DISCONNECTED);
        }
    }
}

void MQTTClient::loop()
{
    checkConnectionStatus();
    if (status != M_CONNECTED)
    {
        if (wifi->getStatus() != WF_CONNECTED)
            return;

        uint32_t current_ms = millis();
        if (status == M_CONNECTING && current_ms - last_reconnect_attempt > MQTT_RECONNECT_DELAY)
        {
            last_reconnect_attempt = current_ms;
            reconnect();
        }
        else if (status == M_DISCONNECTED && current_ms - last_connect_attempt > MQTT_CONNECT_DELAY)
        {
            last_reconnect_attempt = current_ms;
            last_connect_attempt = current_ms;
            reconnect();
        }
    }
    else
    {
        client->loop();
    }
}

void MQTTClient::publish(const char *topic, const char *payload)
{
    client->publish(topic, payload);
    delay(50);
};

void MQTTClient::publishTemperature(const char *payload)
{
    client->publish(MQTT_TEMPERATURE_TOPIC, payload);
};

void MQTTClient::publishHumidity(const char *payload)
{
    client->publish(MQTT_HUMIDITY_TOPIC, payload);
};
