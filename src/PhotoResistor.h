#ifndef PHOTORESISTOR_H
#define PHOTORESISTOR_H

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

    void SetPin(unsigned char apin);

    long GetRawData() const;
    int  GetData() const;

    void Init();
    void SetLightVal();
    bool IsBeyondSensitivity() const;
};

#endif // PHOTORESISTOR_H