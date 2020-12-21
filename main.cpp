/*
//  SPARROWATCH LIMITED
//  FILE: Controlling the motor and switches of the BleeperBike device through Latched Relays
//  AUTHOR: Paul Banahan
//  DATE: 13th February 2018
*/

#include "mbed.h"

//// Setting up the Debugging
Serial pc (USBTX,USBRX);

//// RECIEVER PIN
DigitalIn RecieverSignal = A0;

//// RELAY 1 Pins
DigitalOut R1_BikeControl_Right = D2;   // Bike 1 output connected to digital pin 2
DigitalOut R1_BikeControl_Left = D3;    // Bike 1 output connected to digital pin 3
DigitalOut R1_RelayControl_Upper = D1;   // Relay 1 output connected to digital pin 8
DigitalOut R1_RelayControl_Lower = D9;   // Relay 1 output connected to digital pin 9

//// RELAY 2 Pins
DigitalOut MotorPinA = A4;                  // Motor A output connected to digital pin 4
DigitalOut MotorPinB = A5;                  // Motor B output connected to digital pin 5
DigitalOut R2_RelayControl_Upper = D10;   // Relay 2 output connected to digital pin 10
DigitalOut R2_RelayControl_Lower = D11;   // Relay 2 output connected to digital pin 11

//// RELAY 3 Pins
DigitalOut R3_BikeControl_Right = D6;   // Bike 3 output connected to digital pin 6
DigitalOut R3_BikeControl_Left = D7;    // Bike 3 output connected to digital pin 7
DigitalOut R3_RelayControl_Upper = D12;   // Relay 3 output connected to digital pin 12
DigitalOut R3_RelayControl_Lower = D13;   // Relay 3 output connected to digital pin 13

//// Varible for the control of the states of the relays
int State_Controller = 0;
//// Varible for the control of the states of the motors //////////// ASK PAUL ///////////???????????????????????????????????????????????????????????????????
bool MotorEnabler = false;

int LOW = 0;       // LOW Logic level
int HIGH = 1;      // HIGH Logic level

int ON = 0;        // LOW Logic level -- Active LOW System
int OFF = 1;       // HIGH Logic level -- Active LOW System

int SR_Sleep_Time = 20;        // Delay time for Relays -- Milliseconds
int SR_HoldOff_Time = 30;         // Duration for the activation pulse of the Set Reset

//int Gen_Delay = 75;

//// Resetting Function for the Relays
void ResettingFunc()
{
    //// Delays to allow for the Relays to find Steady State
    pc.printf("Relays to be Reset \r\n");

    R1_RelayControl_Lower = OFF;
    R1_RelayControl_Upper = ON;
    R2_RelayControl_Lower = OFF;
    R2_RelayControl_Upper = ON;
    R3_RelayControl_Lower = OFF;
    R3_RelayControl_Upper = ON;
    wait_ms(SR_Sleep_Time);
    //// Delays to allow for the Relays to find Steady State
    wait(1.0);
    pc.printf("Relays are now Reset \r\n");
};

//// Motor Controlling Fucntion --
/* moves the motor once while out of the area and  once
 * if it is moved when it returns to the area
 */
void  MotorControllerFunction()
{
    while(MotorEnabler == true && RecieverSignal == 0 || MotorEnabler == false && RecieverSignal == 1) {
        MotorPinA = LOW;
        MotorPinB = LOW;
        pc.printf("Motor Stationary Code Start \r\n");
        pc.printf("MotorEnabler State: %d \r\n", MotorEnabler);
        pc.printf("Motor Stationary Code End \r\n");
        pc.printf("\r\n");
        wait(1.0);
    }

    while(MotorEnabler == false && RecieverSignal == 0 || MotorEnabler == true && RecieverSignal == 1) {
        MotorPinA = HIGH;
        MotorPinB = LOW;
        wait(1.75);
        MotorPinA = LOW;
        MotorPinB = LOW;
        pc.printf("Motor Movement Code Start \r\n");
        pc.printf("MotorEnabler State: %d \r\n", MotorEnabler);
        MotorEnabler = !MotorEnabler;
        pc.printf("MotorEnabler State: %d \r\n", MotorEnabler);
        pc.printf("Motor Movement Code End \r\n");
        pc.printf("\r\n");
        wait(1.0);
    }
};

int main()
{

    // Resetting Function called
    ResettingFunc();

    // Printing Debug Statements
    pc.printf("Start up \r\n");
    pc.printf("%d MotorEnabler State: \r\n", MotorEnabler);
    pc.printf("\r\n");

    while(1) {

/////////////////////////////////////
////   BIKE IS INSIDE THE AREA  ////
///////////////////////////////////

// NOTE RECIVERSIGNAL IS A0 DIGITAL INPUT
// WHEN HIGH ITS EQUAL TO 1
// WHEN LOW ITS EQUAL TO 0

        if(RecieverSignal == 1) {

            State_Controller = 0;
            if(State_Controller == 0  && RecieverSignal == 1);
            {
                //// Current_State_Checker = "RX=1 A";
                //// State RX=1 A

                //// Relay Controls setting

                R1_RelayControl_Lower = ON;
                R1_RelayControl_Upper = OFF;           ////  UPPER Relay ON  -- BleeperBikes in Control
                R2_RelayControl_Lower = ON;
                R2_RelayControl_Upper = OFF;
                R3_RelayControl_Lower = ON;
                R3_RelayControl_Upper = OFF;


                wait_ms(SR_Sleep_Time);

                wait_ms(SR_HoldOff_Time);


                /*
                 *  Controlling the Spoofing Signals of the Bike
                 */
                R1_BikeControl_Right =  LOW;
                R1_BikeControl_Left  =  LOW;

                R3_BikeControl_Right =  LOW;
                R3_BikeControl_Left  =  LOW;

                MotorControllerFunction();

                State_Controller = 1;


            }

            if(State_Controller == 1  && RecieverSignal == 1);
            {
                //// State RX=1 B
                //// Relay Controls setting
                //// Current_State_Checker = "RX=1 B";

                //// Delays to allow for the Relays to find Steady State
                //// Switching the Relays OFF for Heat and active LOW Pulsing

                ///////////////////////////////////
                // Bike is inside the Lock Area //
                /////////////////////////////////
                R1_RelayControl_Lower = OFF;           ////  LOWER Relay ON  -- SparroWatch in Control
                R1_RelayControl_Upper = ON;
                R2_RelayControl_Lower = OFF;
                R2_RelayControl_Upper = ON;
                R3_RelayControl_Lower = OFF;
                R3_RelayControl_Upper = ON;

                wait_ms(SR_Sleep_Time);

                wait_ms(SR_HoldOff_Time);

                R1_BikeControl_Right =  LOW;
                R1_BikeControl_Left  =  LOW;

                R3_BikeControl_Right =  LOW;
                R3_BikeControl_Left  =  LOW;
                MotorControllerFunction();

            }
        }

/////////////////////////////////////
////   BIKE IS OUTSIDE THE AREA ////
///////////////////////////////////

        if(RecieverSignal == 0) {

            State_Controller = 0;
            if(State_Controller == 0 && RecieverSignal == 0);
            {
                //// State RX=0 A
                //Current_State_Checker = "RX=0 A";
                /* Resetting the Relays to BleeperBikes having control on Power up
                 */
                ////  Relay Controls setting

                ///////////////////////////////////////
                /// Transition               /////////
                // Latching from Relays take place //
                // /////////////////////////////////
                R1_RelayControl_Lower = ON;
                R1_RelayControl_Upper = ON;           ////  UPPER Relay ON  -- BleeperBikes in Control
                R2_RelayControl_Lower = ON;
                R2_RelayControl_Upper = ON;
                R3_RelayControl_Lower = ON;
                R3_RelayControl_Upper = ON;


                wait_ms(SR_Sleep_Time);

                wait_ms(SR_HoldOff_Time);
                R1_BikeControl_Right =  HIGH;
                R1_BikeControl_Left =   LOW;
                R3_BikeControl_Right =  HIGH;
                R3_BikeControl_Left =   LOW;

                MotorControllerFunction();

                State_Controller = 1;

            }

            if(State_Controller == 1 && RecieverSignal == 0);
            {


                R1_RelayControl_Lower = ON;
                R1_RelayControl_Upper = ON;           ////  UPPER Relay ON  -- BleeperBikes in Control
                R2_RelayControl_Lower = ON;
                R2_RelayControl_Upper = ON;
                R3_RelayControl_Lower = ON;
                R3_RelayControl_Upper = ON;

                R1_BikeControl_Right =  HIGH;
                R1_BikeControl_Left  =  LOW;

                R3_BikeControl_Right =  HIGH;
                R3_BikeControl_Left  =  LOW;
                MotorControllerFunction();

            }
        }
    }
}
