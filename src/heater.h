#ifndef HEATER_H
#define HEATER_H
#include <Grove_Temperature_And_Humidity_Sensor.h>

class Heater {
    private:
        int rel_pin = 0;
        float temp_variance = 0;
        float temp_target = 0;
        int heating = 0;
        int active = 0;
        DHT* dht = nullptr;
    public:
        Heater(int dht_pin, int dht_type, int rel_pin);
        void begin();
        void loop();
        void set_threshold(float temp_variance);
        void activate(float target_temp);
        void deactivate();
};

#endif