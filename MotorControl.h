int MotorEnabler = 0;
int recieverSignal = A1;

void  MotorControllerFucntion()
{
    while(MotorEnabler== 0 && RecieverSignal == 0) {
        MotorPinA = HIGH;
        MotorPinB = HIGH;
        wait(1.75);
        MotorPinA = HIGH;
        MotorPinB = HIGH;
        MotorEnabler = 1;
    }
    while(MotorEnabler== 1 && RecieverSignal == 0) {
        MotorPinA = HIGH;
        MotorPinB = LOW;
        wait(0.5);
    }
    while(MotorEnabler== 0 && RecieverSignal == 1) {
        MotorPinA = HIGH;
        MotorPinB = LOW;
        wait(0.5);
    }
    while(MotorEnabler== 1 && RecieverSignal == 1) {
        MotorPinA = HIGH;
        MotorPinB = HIGH;
        wait(1.75);
        MotorPinA = HIGH;
        MotorPinB = HIGH;
        MotorEnabler = 0;
    }
};
