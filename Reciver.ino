#include<VirtualWire.h>
#include "Keyboard.h"

//RF
int var;
enum MoveToReceive{LEFT,RIGHT,JUMP,JUMP_NOTHING,LR_NOTHING};

//keyboard
const int safetyPin=12;
const int buttonPin=4;
const int rightLed=5;
const int upLed=6;
const int leftLed=7;

void setup() {
  //RF
  pinMode(rightLed,OUTPUT);
  pinMode(upLed,OUTPUT);
  pinMode(leftLed,OUTPUT);
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(11);
  vw_setup(4000);
  vw_rx_start();
  Serial.begin(9600);

  //KEYBOARD
  pinMode(safetyPin,INPUT_PULLUP);
  pinMode(buttonPin,INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  Keyboard.begin();
}

void loop()
{
  uint8_t buff[VW_MAX_MESSAGE_LEN];
  uint8_t buffLen=VW_MAX_MESSAGE_LEN;
 if(digitalRead(safetyPin)==LOW)
 {
  if (vw_get_message(buff, &buffLen))
  {
      var=buff[0]-'0';
      switch(var)
      {
        case LEFT:
          Serial.println("LEFT");
          Keyboard.press(KEY_LEFT_ARROW);
          digitalWrite(leftLed,HIGH);
          Keyboard.release(KEY_RIGHT_ARROW);
          digitalWrite(rightLed,LOW);
          break;
         
        case RIGHT:
          Serial.println("RIGHT");
          Keyboard.press(KEY_RIGHT_ARROW);
          digitalWrite(rightLed,HIGH);
          Keyboard.release(KEY_LEFT_ARROW);
          digitalWrite(leftLed,LOW);
          break;
         
        case JUMP: 
          Serial.println("JUMP");
          Keyboard.press(KEY_UP_ARROW);
          digitalWrite(upLed,HIGH);
          break;

        case JUMP_NOTHING:
           Serial.println("JUMP NOTHING");
           Keyboard.release(KEY_UP_ARROW);
           digitalWrite(upLed,LOW);
           break;

        case LR_NOTHING:
           Serial.println("LEFT RIGHT NOTHING");
           Keyboard.release(KEY_RIGHT_ARROW);
           digitalWrite(rightLed,LOW);
           Keyboard.release(KEY_LEFT_ARROW);
           digitalWrite(leftLed,LOW);
           break; 
     }
    }
   }
  }
