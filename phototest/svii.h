#ifndef SVII_H
#define SVII_H

namespace svii // Smokey VII
{
    constexpr int NUM_RINGS = 5;
    constexpr int NUM_SNOW_LINES = 4;

    constexpr int START_RING_PIN = 2;
    constexpr int START_SNOW_PIN = START_RING_PIN + NUM_RINGS;

    constexpr int SENSOR_APIN = 0;  // A0
    constexpr int SENSOR_SENSITIVITY = 20;

    constexpr int SNOW_DELAY = 300; // ms
    constexpr int LOOP_DELAY = 20;  // ms
}

#endif // SVII_H