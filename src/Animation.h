#ifndef ANIMATION_H
#define ANIMATION_H

#include "svii.h"
#include "Component.h"
#include "PhotoResistor.h"

class Animation
{
private:
    enum State
    {
        INITIAL=0,
        ACTIVATED
    };
    State _currentState = INITIAL;
    Component _ringsArr[svii::NUM_RINGS];
    Component _snowLinesArr[svii::NUM_SNOW_LINES];

public:
    void InitPins();
    bool IsInitState() const;

    void ToggleState();
    void LightAllRings();
    void IncrementSnow();
    void LightAllSnow();
    void ClearRings();
    bool HasLightChanged(const PhotoResistor& sensor) const;
    void Activate();
};

#endif // ANIMATION_H