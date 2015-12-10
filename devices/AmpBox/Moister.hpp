#pragma once

#include <OneWire.h>
#include "Sensor.h"

#define MOISTER_PIN            A0
#define MIN_READING            380
#define MAX_READING            1020

class Moister : public Sensor {

public:
    Moister(bool active)
    : Sensor(active) {
    }

    virtual ~Moister() {
    }

private:

    virtual void start() {
    }

    virtual bool readValue(float& value) {

        int reading = analogRead(MOISTER_PIN);

        // constrict the reading
        reading = max(reading, MIN_READING);
        reading = min(reading, MAX_READING);

        // convert the reading to a saturation %, 0 to 100
        value = (float)map(reading, MAX_READING, MIN_READING, 0, 100);

        return true;
    }
};
