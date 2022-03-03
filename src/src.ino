/* Gabriel Tyler
 * EF 158 Project 1
 * Recitation 3-1
 * Smokey VII Fan Club
 */

#include "Component.h"
#include "PhotoResistor.h"
#include "Animation.h"

namespace svii // Smokey VII Fan Club
{
    PhotoResistor sensor(SENSOR_APIN);
    Animation lights;
}

// -------- MAIN SETUP AND LOOP --------

void setup()
{
    Serial.begin(9600);       // initialize Serial terminal
    Serial.println("\nStarting:");
    svii::lights.InitPins();  // fill snow and rings arrays with digital output pins
    svii::sensor.Init();      // get the initial light value of the room
}

void loop()
{
    svii::sensor.SetLightVal(); // get the current light value of the room
    Serial.println("loop");
    
    svii::lights.IncrementSnow();   // light the next line of snow and clear the previous
    delay(svii::SNOW_DELAY);
    
    // check if the light value of the room has increased or decreased
    // by a specified sensitivity
    if (svii::lights.HasLightChanged(svii::sensor)) 
    {
        Serial.println("light changed");
        // start the olympic ring animation or clear the olympic rings
        // if the light value returned to initial range
        svii::lights.Activate(); 
    }
    
    delay(svii::LOOP_DELAY);  // delay loop for stability
}

// -------- COMPONENT DEFINITIONS --------

Component::Component(unsigned char dpin)
    : _digitalPin(dpin)
{
}

void Component::SetPin(unsigned char dpin)
{
    _digitalPin = dpin;
}
unsigned char Component::GetPin() const
{
    return _digitalPin;
}

void Component::Set() const
{
    // set the pin on high
    analogWrite(_digitalPin, 255);
}
void Component::Clear() const
{
    // set the pin on low
    analogWrite(_digitalPin, 0);
}

// -------- PHOTORESISTOR DEFINITIONS --------

PhotoResistor::PhotoResistor(unsigned char apin, long min, long max) 
    : _analogPin(apin), _min(min), _max(max), 
      _sensitivity(svii::SENSOR_SENSITIVITY), 
      _initialLightVal(0), _currLightVal(0)
{
}

void PhotoResistor::SetPin(unsigned char apin)
{
    _analogPin = apin;
}

long PhotoResistor::GetRawData() const
{
    // returns a number 0 to 1023
    // higher light, higher value
    return static_cast<long>(analogRead(_analogPin));
}
int  PhotoResistor::GetData() const
{
    // normalizes the number from [0, 1023] to [_min, _max]/[0, 255]
    return static_cast<int>(constrain(map(GetRawData(), 0l, 1023l, _min, _max), 0l, 1023l));
}

void PhotoResistor::Init()
{
    _initialLightVal = GetData(); // set the initial light val 
}
void PhotoResistor::SetLightVal()
{
    _currLightVal = GetData();    // get the current normalized data
}
bool PhotoResistor::IsBeyondSensitivity() const
{
    // Check if the current light value is above or below 
    // the initial light value by a sensitivity
    return (_currLightVal - _sensitivity > _initialLightVal) ||
           (_currLightVal + _sensitivity < _initialLightVal);
}

// -------- ANIMATION DEFINITIONS --------

void Animation::InitPins()
{
    // fill the ring and snow components with corresponding pins
    // and set the pins as output

    unsigned char pin;
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        pin = svii::START_RING_PIN + ring;
        _ringsArr[ring].SetPin(pin);
        pinMode(pin, OUTPUT);
    }

    for (int snow = 0; snow < svii::NUM_SNOW; ++snow)
    {
        pin = svii::START_SNOW_PIN + snow;
        _snowArr[snow].SetPin(pin);
        pinMode(pin, OUTPUT);
    }
}

bool Animation::IsInitState() const
{
    // return true if current state is the initial state
    return _currentState == INITIAL;
}
void Animation::ToggleState()
{
    // toggle the current state between initial state and activated state
    if (_currentState == INITIAL) _currentState = ACTIVATED;
    else _currentState = INITIAL;
}

bool Animation::HasLightChanged(const PhotoResistor& sensor) const
{
    // return true if the current light value is above or below
    // the initial light value by a specified sensitivity
    // and if the state needs to change
    return ( sensor.IsBeyondSensitivity() &&  IsInitState()) || 
           (!sensor.IsBeyondSensitivity() && !IsInitState());
}

void Animation::IncrementSnow()
{
    static int currSnow = svii::NUM_SNOW-1; // current snow LED

    // if there are no snow LEDS, do nothing
    if (svii::NUM_SNOW < 1) return;

    // clear the currently lit line of snow and light the next line of snow
    _snowArr[currSnow].Clear();

    ++currSnow;
    if (currSnow >= svii::NUM_SNOW) 
        currSnow = 0;

    _snowArr[currSnow].Set();
}

void Animation::LightAllRings()
{
    Serial.println("Setting all rings");
    // light all the rings one by one with a delay in between
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        Serial.print("Setting Pin: ");
        Serial.println(_ringsArr[ring].GetPin());

        _ringsArr[ring].Set();

        // animation delay, divide by ring speed 
        delay(svii::ANIMATION_DELAY);
    }
}
void Animation::LightAllSnow()
{
    Serial.println("Setting all snow");
    // light all the rings one by one with a delay in between
    for (int snow = 0; snow < svii::NUM_SNOW; ++snow)
    {
        Serial.print("Setting Pin: ");
        Serial.println(_snowArr[snow].GetPin());

        _snowArr[snow].Set();

        // animation delay, divide by ring speed 
        delay(svii::ANIMATION_DELAY);
    }
}

void Animation::ClearRings()
{
    Serial.println("Clearing all rings");
    // clear the rings one by one with a delay in between each
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        Serial.print("Clearing pin: ");
        Serial.println(_ringsArr[ring].GetPin());

        _ringsArr[ring].Clear();

        // animation delay
        delay(svii::ANIMATION_DELAY);
    }
}
void Animation::ClearSnow()
{
    Serial.println("Clearing all snow");
    // clear the rings one by one with a delay in between each
    for (int snow = 0; snow < svii::NUM_SNOW; ++snow)
    {
        Serial.print("Clearing pin: ");
        Serial.println(_snowArr[snow].GetPin());

        _snowArr[snow].Clear();

        // animation delay
        delay(svii::ANIMATION_DELAY);
    }
}

void Animation::Activate()
{
    // if the current state is the initial state, toggle the state to activated
    // and activate the snow and olympic ring animations with delays in between each
    // if the current state is the activated state, clear the olympic rings and toggle state
    if (svii::lights.IsInitState())
    {
        ToggleState();
        LightAllSnow();
        LightAllRings();
        delay(svii::SNOW_DELAY);
    }
    else
    {
        ClearSnow();
        ClearRings();
        ToggleState();
    }
}
