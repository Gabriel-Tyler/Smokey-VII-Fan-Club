/* Gabriel Tyler
 * EF 158 Project 1
 * Recitation 3-1?
 * Smokey VII Fan Club
 */

class Component
{
private:
    unsigned char _digitalPin;
public:
    Component(unsigned char dpin);
    void SetPin(unsigned char dpin);
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
public:
};

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

    Component ringsArr[NUM_RINGS];
    Component snowLinesArr[NUM_SNOW_LINES];

    PhotoResistor sensor(SENSOR_APIN);

    enum State
    {
        INIT=0,   // only snow
        ACTIVATED // snow and rings
    };
    State currentState = INIT;
    void SwitchState();

    void InitPins();

    void LightRings();
    void LightSnow();
    void LightAllSnow();
    void Light();
}

void setup()
{
    Serial.begin(9600);

    svii::InitPins();

    svii::sensor.Init();
}

void loop()
{
    svii::sensor.SetLightVal();

    svii::LightSnow();

    if (svii::sensor.HasLightChanged())
    {
        if (svii::currentState == svii::INIT)
        {
            svii::LightAllSnow();
            delay(1000);
            svii::LightRings();
            delay(1000);
            svii::currentState = svii::ACTIVATED;
        }
        else
        {
            svii::currentState = svii::INIT;
        }
    }

    delay(svii::DELAY);
}

void svii::InitPins()
{
    unsigned char pin = 0;
    // output to Serial the pins for the rings and snow
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        pin = svii::START_RING_PIN + ring;
        svii::ringsArr[ring].SetPin(pin);
        pinMode(pin, OUTPUT);
    }

    for (int snowline = 0; snowline < svii::NUM_SNOW_LINES; ++snowline)
    {
        pin = svii::START_SNOW_PIN + snowline;
        svii::snowLinesArr[snowline].SetPin(pin);
        pinMode(pin, OUTPUT);
    }
}
void svii::LightSnow()
{
    // todo: add a check for snow speed 
    // snow lines start from bottom to top
    static int currSnowLine = 0;
    snowLinesArr[currSnowLine].Clear();
    --currSnowLine;
    if (currSnowLine < 0) currSnowLine = NUM_SNOW_LINES-1;
    snowLinesArr[currSnowLine].Set();
}
void svii::LightAllSnow()
{
    for (int snow = 0; snow < NUM_SNOW_LINES; ++snow)
    {
        snowLinesArr[snow].Set();
        // animation delay
    }
}
void svii::LightRings()
{
    for (int ring = 0; ring < NUM_RINGS; ++ring)
    {
        ringsArr[ring].Set();
        // animation delay, divide by ring speed 
    }
}

Component::Component(unsigned char dpin)
    : _digitalPin(dpin)
{
}
void Component::SetPin(unsigned char dpin)
{
    _digitalPin = dpin;
}
void Component::Set(bool fadein, float fadespeed) const
{
    if (fadein)
    {
        // take initial time
        // use analogWrite and a sin function of delta time
        return;
    }
    // set on
    analogWrite(_digitalPin, 255);
}
void Component::Clear(bool fadeout, float fadespeed) const
{
    if (fadeout)
    {
        // take initial time
        // use analogWrite and a sin function of delta time
        return;
    }
    // set off 
    analogWrite(_digitalPin, 0);
}

PhotoResistor::PhotoResistor(unsigned char apin, long min, long max) 
    : _analogPin(apin), _min(min), _max(max), _sensitivity(svii::SENSOR_SENSITIVITY), _initialLightVal(0), _currLightVal(0)
{
}
long PhotoResistor::GetRawData() const
{
    // returns a number 0 to 1023
    return static_cast<long>(analogRead(_analogPin));
}
int  PhotoResistor::GetData() const
{
    // maps the number from [0, 1023] to [_min, _max]/[0, 255]
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
    _initialLightVal = GetData();
}
void PhotoResistor::SetLightVal()
{
    _currLightVal = GetData();
}
bool PhotoResistor::HasLightChanged() const
{
    return (_currLightVal - _sensitivity > _initialLightVal) ||
           (_currLightVal + _sensitivity < _initialLightVal);
}