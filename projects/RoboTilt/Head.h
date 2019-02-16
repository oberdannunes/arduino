#ifndef Head_h
#define Head_h

#include <Servo.h>
#include <HCSR04.h>

class Head {
    public:
        Head();
        ~Head();
        void attach(int pinH, int pinV, int pinUT, int pinUE);
        void moveHead(int h, int v);
        void center();
        float readDistance();
    
    private:
        Servo _headH;
        Servo _headV;
        int _pinUT;
        int _pinUE;
        HCSR04 *_ultrassonic;
        int _hPosition = -1;
        int _vPosition = -1;
};

#endif
