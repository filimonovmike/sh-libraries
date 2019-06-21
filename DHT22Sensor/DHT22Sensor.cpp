#include "DHT22Sensor.h"

DHT22Sensor::DHT22Sensor(uint8_t const pin)
{
    dht22 = SimpleDHT22(pin);
}

DHT22Sensor::~DHT22Sensor()
{
}

float DHT22Sensor::getTemperature()
{
    return temperature;
};

float DHT22Sensor::getHumidity()
{
    return humidity;
};

void DHT22Sensor::resetTemporaryData()
{
    temperature_summ_in_row = 0;
    humidity_summ_in_row = 0;
    data_polls_in_row_count = 0;
    error_data_polls_in_row_count = 0;
}

void DHT22Sensor::getDataFromSensor()
{
    uint32_t current_ms = millis();
    if ((uint32_t)(current_ms - last_sensor_polling_ms) < SENSOR_POLLING_TIMEOUT)
    {
        return;
    }
    if ((uint32_t)(current_ms - last_data_set_ms) < SENSOR_DATA_SERIAL_POLLING_DELAY)
    {
        return;
    }
    last_sensor_polling_ms = current_ms;

    float current_temperature;
    float current_humidity;
    int err = SimpleDHTErrSuccess;
    data_polls_in_row_count++;
    if ((err = dht22.read2(&current_temperature, &current_humidity, NULL)) != SimpleDHTErrSuccess)
    {
        Serial.print("Read DHT22 failed, err=");
        Serial.println(err);
        error_data_polls_in_row_count++;
        if (error_data_polls_in_row_count == SENSOR_POLL_IN_ROW_COUNT) // если все попытки сфэйлились, обнуляемся и пробуем еще раз
        {
            resetTemporaryData();
            return;
        }
    }
    else
    {
        temperature_summ_in_row += current_temperature;
        Serial.print("Sample OK: ");
        Serial.print(floor(current_temperature * 10) / 10);
        Serial.print("\u00b0C, ");

        humidity_summ_in_row += current_humidity;
        Serial.print(floor(current_humidity));
        Serial.println(" RH%");
    }
    if (data_polls_in_row_count == SENSOR_POLL_IN_ROW_COUNT)
    {
        last_data_set_ms = current_ms;

        temperature = temperature_summ_in_row / (data_polls_in_row_count - error_data_polls_in_row_count);
        temperature = floor(temperature * 10) / 10;

        humidity = humidity_summ_in_row / (data_polls_in_row_count - error_data_polls_in_row_count);
        humidity = floor(humidity * 10) / 10;

        resetTemporaryData();
    }
}