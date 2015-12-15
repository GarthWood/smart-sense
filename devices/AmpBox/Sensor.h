#pragma once

#define NUM_READINGS        5
#define READ_INTERVAL       100
#define UNDEFINED           -999

class Sensor {

public:
    Sensor(bool active = true)
    : _readingIndex(0)
    , _currentReading(UNDEFINED)
    , _lastReading(UNDEFINED)
    , _active(active)
    , _timer(0) {
    }

    virtual ~Sensor() {
    }

    void init() {
        if (_active) {
            start();
            _timer = millis();
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

    long _timer;

    virtual void start() = 0;

    virtual bool readValue(float& value) = 0;

    void readSensor() {

        if ((millis() - _timer) >= READ_INTERVAL) {
            if (_readingIndex == NUM_READINGS) {

                float total = 0;

                for (int i = 0; i < NUM_READINGS; ++i) {
                    total += _readings[i];
                }

                _currentReading = total / (float)NUM_READINGS;
                _readingIndex = 0;
            } else {
                float value;

                if (readValue(value)) {
                     _readings[_readingIndex++] = value;
                }
            }

            _timer = millis();
        }
    }
};
