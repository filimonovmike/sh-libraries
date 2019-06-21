Some libraries for my projects



and a contents of `src/main.c`:
```
#include <Foo.h>
#include <Bar.h>

int main (void)
{
  ...
}

```

## DHT22Sensor

Библиотека запрашивает данные с датчика DHT22 10 (`SENSOR_POLL_IN_ROW_COUNT`) раз за серию запросов с задержкой в 2,5 секунды и высчитывает среднее значение.
Задержка между сериями `SENSOR_DATA_SERIAL_POLLING_DELAY` по умолчанию 1 минута 

## MQTTClient

Использует `<PubSubClient.h>`. Подключается к MQTT и умеет возвращать состояние подключения, делал для отображения состояния подключения для дисплея. 

### Настройки

```
const char MQTT_SERVER[30] = "192.168.1.100"; // адрес сервера MQTT
const char MQTT_ID[20] = "WS-bedroom"; // id устройстсва
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
```


## WiFiConnector

Подключается к WiFi и следит за подключением.

### Использование

```
#include "WiFiConnector.h"

void setup()
{
  wifi.init();
  wifi.setup();
  ...
}

void loop()
{
  wifi.tick();
  ...
}
```
