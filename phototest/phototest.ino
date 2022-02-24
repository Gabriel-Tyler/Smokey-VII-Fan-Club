const int photoPin = 0; // analog input connected to photoresistor
const int ledPin = 12;  // output pin

int initLightVal; // initial light value read on setup
int lightVal;     // light value read once every loop

void setup()
{
    Serial.begin(9600);  // setup the serial monitor for light value output
    pinMode(ledPin, OUTPUT);  // set the ledPin as output so it can be set on HIGH or LOW
    initLightVal = analogRead(photoPin);  // read initial light value
}

void loop()
{
    lightVal = analogRead(sensorPin);  // read current light value
    Serial.print("lightVal = ");  // current print light value
    Serial.println(lightVal);

    // if light value increase over a certain threshold, set ledPin on high
    // else set on low
    if (lightVal-50 > initialLightVal)
    {
        digitalWrite(ledPin, HIGH); 
    }
    else
    {
        digitalWrite(ledPin, LOW);
    }
    delay(20);
}
