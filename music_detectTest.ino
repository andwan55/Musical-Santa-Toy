#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

int relayTest = 0;

uint8_t servonum = 0;

Servo servo1;
Servo servo2;

int pos1 = 0;//initial servo positions
int pos2 = 0;

int led0 = 2; //
int led1 = 3; //
int led2 = 4; //
int led3 = 5; //
int led4 = 6;
int led5 = 7;
int led6 = 8;
int led7 = 9;

int lVal0=0; // Store brightness values
int lVal1=0;
int lVal2=0;
int lVal3=0;
int lVal4=0;
int lVal5=0;

int lVal6 = 0;
int lVal7 = 0;

int RELAY_PIN = 11;

char buff[] = "000000000000"; // Incoming serial array

void setup() {
  Serial.begin(9600);
  
  pinMode(led0, OUTPUT); // Set up pins
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  
  pinMode(RELAY_PIN, OUTPUT);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
}

void loop() {
  //servo driver test code
  while(Serial.available() > 0) {
    for(int i = 0; i < 12; i++) { // For each element of buff
      buff[i] = buff[i+1]; // Take the value of the next element
    }
    
    buff[12] = Serial.read();
    if(buff[12] == 'A') {
      lVal0 = int(buff[11]);
      digitalWrite(led0, lVal0);
      lVal6 = int(buff[11]);
      digitalWrite(led6, lVal6);
    }
    
    else if(buff[12] == 'B') {
      lVal1 = int(buff[11]);
      digitalWrite(led1, lVal1);

      /*if(relayTest == 200){
        digitalWrite(RELAY_PIN, HIGH);
        delay(200);
        digitalWrite(RELAY_PIN, LOW);
      }*/
      relayTest++;
    }
    
    else if(buff[12] == 'C') {
      for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
        pwm.setPWM(0, 0, pulselen);
      }
      
      for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        pwm.setPWM(0, 0, pulselen);
      }

  //delay(5);
      
      lVal2 = int(buff[11]);
      digitalWrite(led2, lVal2);
    }
    
    else if(buff[12] == 'D') {
      lVal3 = int(buff[11]);
      digitalWrite(led3, lVal3);

      for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        pwm.setPWM(1, 0, pulselen);
      }

  //delay(5);
      for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
        pwm.setPWM(1, 0, pulselen);
      }
    }
    
    else if(buff[12] == 'E') {
      lVal4 = int(buff[11]);
      digitalWrite(led4, lVal4);
    }
    
    else if(buff[12] == 'F') {
      lVal5 = int(buff[11]);
      digitalWrite(led5, lVal5);
      lVal7 = int(buff[11]);
      digitalWrite(led7, lVal7);
    }
  }
  //delay(50);
}
