#pragma once

#include <OneWire.h>
#include "Sensor.h"

#define SETUP_DELAY         1000
#define TEMP_PIN            13

class Temperature : public Sensor {

public:
    Temperature(bool active)
    : Sensor(active)
    , _timer(0) {

      _tempSensor = new OneWire(TEMP_PIN);
    }

    virtual ~Temperature() {
        delete _tempSensor;
    }

private:

    OneWire* _tempSensor;

    byte _sensorAddress[8];

    long _timer;

    virtual void start() {

        while (!_tempSensor->search(_sensorAddress)) {
            _tempSensor->reset_search();
            memset(_sensorAddress, 0, 8);
        }
    }

    virtual bool readValue(float& value) {

        bool result = false;
        long now = millis();

        if (_timer == 0) {
            _tempSensor->reset();
            _tempSensor->select(_sensorAddress);
            _tempSensor->write(0x44, 1);
            _timer = now;
        } else if ((now - _timer) >= SETUP_DELAY) {

            byte data[12];

            _timer = 0;

            _tempSensor->reset();
            _tempSensor->select(_sensorAddress);
            _tempSensor->write(0xBE);

            for (int i = 0; i < 9; ++i) {           // we need 9 bytes
                data[i] = _tempSensor->read();
            }

            int16_t raw = (data[1] << 8) | data[0];

            byte cfg = (data[4] & 0x60);

            if (cfg == 0x00) {
                raw = raw & ~7;  // 9 bit resolution, 93.75 ms
            } else if (cfg == 0x20) {
                raw = raw & ~3; // 10 bit res, 187.5 ms
            } else if (cfg == 0x40) {
                raw = raw & ~1; // 11 bit res, 375 ms
            }

            value = (float)raw / 16.0;
            result = true;
        }

        return result;
    }
};
