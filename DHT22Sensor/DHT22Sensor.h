#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H

#include <SimpleDHT.h>

// DHT22 sampling rate is 0.5HZ.
const uint16_t SENSOR_POLLING_TIMEOUT = 2500;                // таймаут между опросами сенсора
const uint8_t SENSOR_POLL_IN_ROW_COUNT = 10;                 // кол-во запросов подряд для определения среднего значения
const uint32_t SENSOR_DATA_SERIAL_POLLING_DELAY = 60 * 1000; // перерыв между сериями запроса
class DHT22Sensor
{
private:
  SimpleDHT22 dht22;
  float temperature;
  float humidity;
  uint32_t last_sensor_polling_ms = -SENSOR_POLLING_TIMEOUT;
  uint32_t last_data_set_ms = -SENSOR_DATA_SERIAL_POLLING_DELAY;

  uint8_t data_polls_in_row_count = 0;
  uint8_t error_data_polls_in_row_count = 0;
  float temperature_summ_in_row;
  float humidity_summ_in_row;

  void resetTemporaryData();

public:
  DHT22Sensor(uint8_t const pin);
  ~DHT22Sensor();

  float getTemperature();
  float getHumidity();
  void getDataFromSensor();
};

#endif