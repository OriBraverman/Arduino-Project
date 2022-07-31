#include <Wire.h>                 // Must include Wire library for I2C
#include "SparkFun_MMA8452Q.h"    // Click here to get the library: http://librarymanager/All#SparkFun_MMA8452Q
#include<VirtualWire.h>
char *controller;
MMA8452Q accel;                   // create instance of the MMA8452 class
enum MoveToSend{LEFT, RIGHT, JUMP, NOTHING_JUMP, NOTHING_SIDES, NOTHING};
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long prevTime, currTime;

void sendData(MoveToSend movement);
MoveToSend detectLeftOrRight(double Ymovement);
MoveToSend detectJump(double Zmovement);

void setup() {
  Serial.begin(9600);
  Serial.println("MMA8452Q Basic Reading Code!");
  //this is the Tx part
  pinMode(13,OUTPUT);
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(12);
  vw_setup(4000);
  controller="01";//init controller
  //
  Wire.begin();

  if (accel.begin() == false) {
    Serial.println("Not Connected. Please check connections and read the hookup guide.");
    while (1);
  }
}

void loop() {
  MoveToSend var;
  if (accel.available()) {      // Wait for new data from accelerometer
    var = detectLeftOrRight(accel.getCalculatedY());
    sendData(var);
    //Serial.print(var);
    var = detectJump(accel.getCalculatedZ());
    sendData(var);
    Serial.print(var);
  }
}

void sendData(MoveToSend movement)
{
  if(movement==MoveToSend::NOTHING)
    return;
  digitalWrite(13,HIGH);//set pin13 to HIGH
  controller[0] = movement+'0';
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx();
  digitalWrite(13,LOW);//set pin13 to LOW
}

MoveToSend detectLeftOrRight(double Ymovement)
{
  const double RightDetected = 0.3;
  const double LeftDetected = -0.3;
  MoveToSend LastMove = NOTHING_SIDES;
  if(Ymovement>=RightDetected)
  {
    LastMove = RIGHT;
    return RIGHT;
  }
  else if(Ymovement<=LeftDetected)
  {
    LastMove = MoveToSend::LEFT;
    return MoveToSend::LEFT;
  }
  else
  {
    LastMove = MoveToSend::NOTHING_SIDES;
    return MoveToSend::NOTHING_SIDES;
  }
}

MoveToSend detectJump(double Zmovement)
{
  currTime=millis();
  const double JumpDetected = -0.2;
  MoveToSend LastJump = MoveToSend::NOTHING_JUMP;
  if(Zmovement<=JumpDetected)
  {
    prevTime=currTime;
    LastJump = MoveToSend::JUMP;
    return MoveToSend::JUMP;
  }
  else if(currTime-prevTime<=250)//500 is half of a second
  {
    LastJump = MoveToSend::NOTHING;
    return MoveToSend::NOTHING;
  }
  else 
  {
    LastJump = MoveToSend::NOTHING_JUMP;
    return MoveToSend::NOTHING_JUMP;
  }
}
