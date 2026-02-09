#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

class MotorControl {
public:
    MotorControl(int ena, int in1, int in2, int enb, int in3, int in4);
    
    void begin();
    void mover(int motor, int velocidad);
    void detenerTodo();

private:
    int _ena, _in1, _in2;
    int _enb, _in3, _in4;
};

#endif
