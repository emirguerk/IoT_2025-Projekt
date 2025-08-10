#include "heater.h"
#include <Arduino.h>
#include <Grove_Temperature_And_Humidity_Sensor.h>

Heater::Heater(int dht_pin, int dht_type, int rel_pin) {
    dht = new DHT(dht_pin, dht_type);
    this->rel_pin = rel_pin;
}

void Heater::begin() {
    pinMode(rel_pin, OUTPUT);
    Wire.begin();
    dht->begin();
}

void Heater::loop() {
    float temp_hum_val[2] = {0};
    float max_temp = temp_target + temp_variance;
    float min_temp = temp_target - temp_variance;

    if (active && !dht->readTempAndHumidity(temp_hum_val)) {
        if (heating && temp_hum_val[1] > max_temp) {
            heating = false;
        } else if (!heating && temp_hum_val[1] < min_temp) {
            heating = true;
        }
    } else if (active) {
        heating = false;
        Serial.println("Failed to get temprature and humidity value.");
    }
    digitalWrite(rel_pin, heating);
}

void Heater::activate(float target_temp) {
    this->temp_target = target_temp;
    this->active = true;
}

void Heater::deactivate() {
    this->heating = false;
    this->active = false;
}

void Heater::set_threshold(float temp_variance) {
    this->temp_variance = temp_variance;
}