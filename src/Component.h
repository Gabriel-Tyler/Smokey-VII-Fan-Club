#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{
private:
    unsigned char _digitalPin;

public:
    Component(unsigned char dpin=0);
    void SetPin(unsigned char dpin=0);
    void Set() const;
    void Clear() const;
};

#endif // COMPONENT_H