class Component
{
private:
    const int digitalPort;
public:
    Component(int dp);
    void Set(bool fadein=false,    float fadespeed=1.0f) const;
    void Clear(bool fadeout=false, float fadespeed=1.0f) const;
};

Component::Component(int dp) : digitalPort(dp)
{
}
void Component::Set(bool fadein, float fadespeed) const
{
    static constexpr int ANALOG_HIGH = 255;
    if (fadein)
    {
        // take initial time
        // use analogWrite and a sin function of delta time
        return;
    }
    // set on
    analogWrite(digitalPort, ANALOG_HIGH);
}
void Component::Clear(bool fadeout, float fadespeed) const
{
    static constexpr int ANALOG_LOW = 0;
    if (fadeout)
    {
        // take initial time
        // use analogWrite and a sin function of delta time
        return;
    }
    analogWrite(digitalPort, ANALOG_LOW);
}

namespace svii
{
    constexpr int NUM_RINGS = 5;
    constexpr int NUM_SNOW_LINES = 5;

    constexpr int START_RING_PORT = 2;
    constexpr int START_SNOW_PORT = START_RING_PORT + NUM_RINGS;

    Component ringsArr[NUM_RINGS];
    Component snowLinesArr[NUM_SNOW_LINES];

    int initialLightVal;
    int currLightVal;
}

void setup()
{
    Serial.begin(9600);

    for (int ring = 0; i < NUM_RINGS; ++ring)
    {
        svii::ringsArr[ring]
    }
}

void loop()
{
}
