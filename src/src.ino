/* Gabriel Tyler
 * EF 158 Project 1
 * Recitation 3-1?
 * Smokey VII Fan Club
 */

/*
TODO:
    add snow speed and ring speed 
    add fade support for functions that use set and clear
    test code with arduino
 */

namespace svii // Smokey VII
{
    constexpr int NUM_RINGS = 5;
    constexpr int NUM_SNOW_LINES = 5;

    constexpr int START_RING_PIN = 2;
    constexpr int START_SNOW_PIN = START_RING_PIN + NUM_RINGS;

    constexpr int SENSOR_APIN = 0;
    constexpr int SENSOR_SENSITIVITY = 50;

    constexpr float SNOW_SPEED = 1.0f;
    constexpr float RING_SPEED = 1.0f;

    constexpr int DELAY = 20;
}

class Component
{
private:
    unsigned char _digitalPin;

public:
    Component(unsigned char dpin=0);
    void SetPin(unsigned char dpin=0);
    bool FadeCheck(int currBrightness, int initialBrightness) const;
    void Fade(int initBrightness, int fadeIncrement, float fadespeed) const;
    void Set(bool fadein=false,    float fadespeed=1.0f) const;
    void Clear(bool fadeout=false, float fadespeed=1.0f) const;
};

class PhotoResistor
{
private:
    unsigned char _analogPin;
    long _min;
    long _max;
    int  _sensitivity;
    int  _initialLightVal;
    int  _currLightVal;

public:
    PhotoResistor(unsigned char apin, long min=0, long max=255);

    long GetRawData() const;
    int  GetData() const;
    void SetPin(unsigned char apin);
    void SetMin(long min=0);
    void SetMax(long max=255);
    void SetSensitivity(int s=0);

    void Init();
    void SetLightVal();
    bool HasLightChanged() const;
};

class Animation
{
private:
    enum State
    {
        INIT=0,
        ACTIVATED
    };
    State _currentState = INIT;
    Component _ringsArr[svii::NUM_RINGS];
    Component _snowLinesArr[svii::NUM_SNOW_LINES];

public:
    void InitPins();
    bool IsInit() const;

    void ToggleState();
    void LightRings();
    void LightSnow();
    void LightAllSnow();
    void ClearRings();
    void Activate();
};

namespace svii
{
    PhotoResistor sensor(SENSOR_APIN);
    Animation lights;
}

void setup()
{
    Serial.begin(9600);       // initialize Serial terminal
    svii::lights.InitPins();  // fill snow and rings arrays with digital output pins
    svii::sensor.Init();      // get the initial light value of the room
}

void loop()
{
    svii::sensor.SetLightVal(); // get the current light value of the room

    svii::lights.LightSnow();   // light the next line of snow and clear the previous

    // check if the light value of the room has increased or decreased
    // by a specified sensitivity
    if (svii::sensor.HasLightChanged()) 
    {
        // start the olympic ring animation or clear the olympic rings
        // if the light value returned to initial range
        svii::lights.Activate(); 
    }
    
    delay(svii::DELAY);  // delay loop for stability
}

Component::Component(unsigned char dpin)
    : _digitalPin(dpin)
{
}
void Component::SetPin(unsigned char dpin)
{
    _digitalPin = dpin;
}
bool Component::FadeCheck(int currBrightness, int initialBrightness) const
{
    if (initialBrightness == 255)
    {
        return currBrightness > 0;
    }
    else if (initialBrightness == 0)
    {
        return currBrightness < 255;
    }
    else
    {
        return false;
    }
}
void Component::Fade(int initialBrightness, int fadeIncrement, float fadespeed) const 
{
    int currBrightness = initialBrightness;
    int brightnessIncrement = fadeIncrement;
    while (FadeCheck(currBrightness, initialBrightness))
    {
        analogWrite(_digitalPin, currBrightness);
        currBrightness += brightnessIncrement;
        delay(static_cast<unsigned long>(1000ul/fadespeed));
    }
}
void Component::Set(bool fadein, float fadespeed) const
{
    if (fadein)
    {
        Fade(0, 50, fadespeed);
    }
    // set the pin on high
    analogWrite(_digitalPin, 255);
}
void Component::Clear(bool fadeout, float fadespeed) const
{
    if (fadeout)
    {
        Fade(255, -50, fadespeed);
    }
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
void PhotoResistor::SetMin(long min)
{
    _min = min;
}
void PhotoResistor::SetMax(long max)
{
    _max = max;
}
void PhotoResistor::SetSensitivity(int s)
{
    _sensitivity = s;
}
void PhotoResistor::Init()
{
    _initialLightVal = GetData(); // set the initial light val 
}
void PhotoResistor::SetLightVal()
{
    _currLightVal = GetData();    // get the current normalized data
}
bool PhotoResistor::HasLightChanged() const
{
    // return true if the current light value is above or below
    // the initial light value by a specified sensitivity
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
bool Animation::IsInit() const
{
    // return true if current state is the initial state
    return _currentState == INIT;
}
void Animation::ToggleState()
{
    // toggle the current state between initial state and activated state
    if (_currentState == INIT) _currentState = ACTIVATED;
    else _currentState = INIT;
}
void Animation::LightRings()
{
    // light all the rings one by one with a delay in between
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        _ringsArr[ring].Set();
        // animation delay, divide by ring speed 
    }
}
void Animation::LightSnow()
{
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
    }
}
void Animation::ClearRings()
{
    // clear the rings one by one with a delay in between each
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        _ringsArr[ring].Clear();
        // animation delay
    }
}
void Animation::Activate()
{
    // if the current state is the initial state, toggle the state to activated
    // and activate the snow and olympic ring animations with delays in between each
    // if the current state is the activated state, clear the olympic rings and toggle state
    if (svii::lights.IsInit())
    {
        ToggleState();

        LightAllSnow();
        delay(1000);
        LightRings();
        delay(1000);
    }
    else
    {
        ClearRings();
        ToggleState();
    }
}