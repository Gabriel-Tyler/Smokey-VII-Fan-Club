class Component
{
private:
    int _digitalPin;
public:
    void SetPort(int newpin);
    void Set(bool fadein=false,    float fadespeed=1.0f) const;
    void Clear(bool fadeout=false, float fadespeed=1.0f) const;
};

class PhotoResistor
{
private:
    int _analogPin;
    int _min;
    int _max;
public:
    PhotoResistor(int apin, int min=0, int max=1023);
    int GetRawData() const;
    int GetData() const;
    int SetMin(int min=0);
    int SetMax(int max=1023);
};
PhotoResistor::PhotoResistor(int apin, int min, int max) 
    : _analogPin(apin), _min(min), _max(max)
{
}
int PhotoResistor::GetRawData() const
{
    // returns a number 0 to 1023
    return analogRead(_analogPin);
}
int PhotoResistor::GetData() const
{
    // maps the number from [0, 1023] to [_min, _max]/[0, 255]
    return constrain(map(GetRawData(), _min, _max, 0, 1023), 0, 1023);
}

namespace svii
{
    constexpr int ANALOG_LOW = 0;
    constexpr int ANALOG_HIGH = 255;

    constexpr int NUM_RINGS = 5;
    constexpr int NUM_SNOW_LINES = 5;

    constexpr int START_RING_PIN = 2;
    constexpr int START_SNOW_PIN = START_RING_PIN + NUM_RINGS;

    Component ringsArr[NUM_RINGS];
    Component snowLinesArr[NUM_SNOW_LINES];

    int initialLightVal;
    int currLightVal;
}

void setup()
{
    Serial.begin(9600);

    // output to Serial the pins for the rings and snow
    for (int ring = 0; ring < svii::NUM_RINGS; ++ring)
    {
        svii::ringsArr[ring].SetPort(svii::START_RING_PIN + ring);
    }

    for (int snowline = 0; snowline < svii::NUM_SNOW_LINES; ++snowline)
    {
        svii::snowLinesArr[snowline].SetPort(svii::START_SNOW_PIN+snowline);
    }
}

void loop()
{
}

void Component::SetPort(int newpin)
{
    _digitalPin = newpin;
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
    analogWrite(_digitalPin, svii::ANALOG_HIGH);
}
void Component::Clear(bool fadeout, float fadespeed) const
{
    if (fadeout)
    {
        // take initial time
        // use analogWrite and a sin function of delta time
        return;
    }
    analogWrite(_digitalPin, svii::ANALOG_LOW);
}