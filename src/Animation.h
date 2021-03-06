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
    Component _snowArr[svii::NUM_SNOW];

public:
    void InitPins();
    
    bool IsInitState() const;
    void ToggleState();

    bool HasLightChanged(const PhotoResistor& sensor) const;

    void IncrementSnow();

    void LightAllRings();
    void LightAllSnow();

    void ClearRings();
    void ClearSnow();

    void Activate();
};

#endif // ANIMATION_H
