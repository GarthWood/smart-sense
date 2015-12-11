#pragma once

#include <OneWire.h>
#include "Sensor.h"

#define MOISTER_PIN            A0
#define MIN_READING            380
#define MAX_READING            1020

class Moisture : public Sensor {

public:
    Moisture(bool active)
    : Sensor(active) {
    }

    virtual ~Moisture() {
    }

private:

    virtual void start() {
    }

    virtual bool readValue(float& value) {

        int reading = analogRead(MOISTER_PIN);

        // constrict the reading
        reading = (reading < MIN_READING) ? MIN_READING : reading;
        reading = (reading > MAX_READING) ? MAX_READING : reading;

        // convert the reading to a saturation %, 0 to 100
        value = (float)map(reading, MAX_READING, MIN_READING, 0, 100);

        return true;
    }
};
