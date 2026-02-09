#include "MotorControl.h"

MotorControl::MotorControl(int ena, int in1, int in2, int enb, int in3, int in4) 
    : _ena(ena), _in1(in1), _in2(in2), _enb(enb), _in3(in3), _in4(in4) {}

void MotorControl::begin() {
    pinMode(_ena, OUTPUT);
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    pinMode(_enb, OUTPUT);
    pinMode(_in3, OUTPUT);
    pinMode(_in4, OUTPUT);
}

void MotorControl::mover(int motor, int velocidad) {
    int pwPin = (motor == 1) ? _ena : _enb;
    int inA = (motor == 1) ? _in1 : _in3;
    int inB = (motor == 1) ? _in2 : _in4;

    if (velocidad == 0) {
        digitalWrite(inA, LOW);
        digitalWrite(inB, LOW);
        analogWrite(pwPin, 0);
    } else if (velocidad > 0) {
        digitalWrite(inA, HIGH);
        digitalWrite(inB, LOW);
        analogWrite(pwPin, min(255, velocidad));
    } else {
        digitalWrite(inA, LOW);
        digitalWrite(inB, HIGH);
        analogWrite(pwPin, min(255, abs(velocidad)));
    }
}

void MotorControl::detenerTodo() {
    mover(1, 0);
    mover(2, 0);
}
