/* Gabriel Tyler
 * EF 158 Project 1
 * Recitation 3-1?
 * Smokey VII Fan Club
 */

/*
TODO:
    Add a fade to the leds
    Add a random function to the snow
    Make magic numbers into constants
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
    svii::sensor.SetLightVal(); // get the current light value of the room
    Serial.println("loop");
    
    svii::lights.IncrementSnow();   // light the next line of snow and clear the previous
    delay(300);
    
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

Component::Component(unsigned char dpin)
    : _digitalPin(dpin)
{
}
void Component::SetPin(unsigned char dpin)
{
    _digitalPin = dpin;
}
void Component::Set() const
{
    Serial.println(_digitalPin);
    // set the pin on high
    analogWrite(_digitalPin, 255);
}
void Component::Clear() const
{
    Serial.println(_digitalPin);
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
void Animation::LightAllRings()
{
    // light all the rings one by one with a delay in between
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        _ringsArr[ring].Set();
        // animation delay, divide by ring speed 
        delay(1000);
    }
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
void Animation::LightAllSnow()
{
    // light all the lines of snow one by one with a delay in between
    for (int snow = 0; snow < svii::NUM_SNOW_LINES; ++snow)
    {
        _snowLinesArr[snow].Set();
        // animation delay
        delay(100);
    }
}
void Animation::ClearRings()
{
    // clear the rings one by one with a delay in between each
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        _ringsArr[ring].Clear();
        // animation delay
        delay(1000);
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

        LightAllSnow();
        delay(500);
        LightAllRings();
        delay(500);
    }
    else
    {
        ClearRings();
        ToggleState();
    }
}
