#ifndef SVII_H
#define SVII_H

namespace svii // Smokey VII
{
    constexpr int NUM_RINGS = 0;
    constexpr int NUM_SNOW_LINES = 2;

    constexpr int START_RING_PIN = 2;
    constexpr int START_SNOW_PIN = START_RING_PIN + NUM_RINGS;

    constexpr int SENSOR_APIN = 0;
    constexpr int SENSOR_SENSITIVITY = 30;

    constexpr int LOOP_DELAY = 20;
}

#endif // SVII_H