#pragma once

#define NUM_READINGS        5

class Sensor {

public:
    Sensor(bool active = true)
    : _readingIndex(0)
    , _currentReading(0)
    , _lastReading(0)
    , _active(active) {
    }

    virtual ~Sensor() {
    }

    void init() {
        if (_active) {
            start();
        }
    };

    void run() {
        if (_active) {
            readSensor();
        }
    }

    float read() {

        if (_currentReading != _lastReading) {
            _lastReading = _currentReading;
        }

        return _lastReading;
    }

    bool hasChanged() {
        return _active && (_currentReading != _lastReading);
    }

protected:

    float _readings[NUM_READINGS];

    int _readingIndex;

    float _currentReading,
          _lastReading;

    bool _active;

    virtual void start() = 0;

    virtual bool readValue(float& value) = 0;

    void readSensor() {

        if (_readingIndex == NUM_READINGS) {

            int total = 0;

            for (int i = 0; i < NUM_READINGS; ++i) {
                total += _readings[i];
            }

            _currentReading = (int)(total / NUM_READINGS);
            _readingIndex = 0;
        } else {
            float value;

            if (readValue(value)) {
                 _readings[_readingIndex++] = value;
            }
        }
    }
};