/* Gabriel Tyler
 * EF 158 Project 1
 * Recitation 3-1?
 * Smokey VII Fan Club
 */

// git pull origin main

/*
TODO:
    Add a fade to the leds
    Add a random function to the snow
    Make magic numbers into constants
    Add Serial prints
*/

#include "Component.h"
#include "PhotoResistor.h"
#include "Animation.h"

namespace svii
{
    PhotoResistor sensor(SENSOR_APIN);
    Animation lights;
}

void setup()
{
    Serial.begin(9600);       // initialize Serial terminal
    Serial.println("\nStarting:");
    svii::lights.InitPins();  // fill snow and rings arrays with digital output pins
    svii::sensor.Init();      // get the initial light value of the room
}

void loop()
{

    for (int i = 0; i < 9; ++i)
    {
        analogWrite(2+i, 255);
    }
    
    delay(1000);  // delay loop for stability
}

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

PhotoResistor::PhotoResistor(unsigned char apin, long min, long max) 
    : _analogPin(apin), _min(min), _max(max), 
      _sensitivity(svii::SENSOR_SENSITIVITY), 
      _initialLightVal(0), _currLightVal(0)
{
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
void PhotoResistor::SetPin(unsigned char apin)
{
    _analogPin = apin;
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
    return (_currLightVal - _sensitivity > _initialLightVal) ||
           (_currLightVal + _sensitivity < _initialLightVal);
}

void Animation::InitPins()
{
    // maybe output to Serial the pins for the rings and snow on startup

    // fill the ring and snow components with corresponding pins
    // and set the pins as output
    unsigned char pin;
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        pin = svii::START_RING_PIN + ring;
        _ringsArr[ring].SetPin(pin);
        pinMode(pin, OUTPUT);
    }

    for (int snowline = 0; snowline < svii::NUM_SNOW_LINES; ++snowline)
    {
        pin = svii::START_SNOW_PIN + snowline;
        _snowLinesArr[snowline].SetPin(pin);
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
void Animation::IncrementSnow()
{
    if (svii::NUM_SNOW_LINES < 1) return;
    // todo: add a check for snow speed 
    // snow lines start from bottom to top
    // clear the currently lit line of snow and light the next line of snow
    static int currSnowLine = 0;
    _snowLinesArr[currSnowLine].Clear();
    --currSnowLine;
    if (currSnowLine < 0) currSnowLine = svii::NUM_SNOW_LINES-1;
    _snowLinesArr[currSnowLine].Set();
}
void Animation::LightAllRings()
{
    Serial.println("Setting all rings");
    // light all the rings one by one with a delay in between
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        _ringsArr[ring].Set();
        Serial.print("Setting Pin: ");
        Serial.println(_ringsArr[ring].GetPin());
        // animation delay, divide by ring speed 
        delay(1000);
    }
}
void Animation::ClearRings()
{
    Serial.println("Clearing all rings");
    // clear the rings one by one with a delay in between each
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        _ringsArr[ring].Clear();
        Serial.print("Clearing pin: ");
        Serial.println(_ringsArr[ring].GetPin());
        // animation delay
        delay(200);
    }
}
bool Animation::HasLightChanged(const PhotoResistor& sensor) const
{
    // return true if the current light value is above or below
    // the initial light value by a specified sensitivity
    return ( sensor.IsBeyondSensitivity() &&  IsInitState()) || 
           (!sensor.IsBeyondSensitivity() && !IsInitState());
}
void Animation::Activate()
{
    // if the current state is the initial state, toggle the state to activated
    // and activate the snow and olympic ring animations with delays in between each
    // if the current state is the activated state, clear the olympic rings and toggle state
    if (svii::lights.IsInitState())
    {
        ToggleState();
        // light all snow
        LightAllRings();
        delay(300);
    }
    else
    {
        ClearRings();
        ToggleState();
    }
}
