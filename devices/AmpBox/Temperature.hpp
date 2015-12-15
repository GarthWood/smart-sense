#pragma once

#include "Sensor.h"

#define TEMP_PIN            A0

class Temperature : public Sensor {

public:
    Temperature(bool active)
    : Sensor(active) {
    }

    virtual ~Temperature() {
    }

private:

    virtual void start() {
    }

    virtual bool readValue(float& value) {

        value = (3.3 * analogRead(TEMP_PIN) * 100.0) / 1024;

        return true;
    }
};
