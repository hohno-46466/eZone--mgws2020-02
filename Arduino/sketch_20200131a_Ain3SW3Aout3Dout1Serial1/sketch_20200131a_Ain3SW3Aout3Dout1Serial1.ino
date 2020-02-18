//
// Ain3SW3Aout3Dout1Serial1.ino
//

// public domain

// ---------------------------------------------------------

// First version: Wed Feb 12 04:45:00 JST 2020 by @hohno_at_kuimc
// Prev update: Mon Feb 17 06:14:14 JST 2020
// Prev update: Mon Feb 17 08:16:55 JST 2020
// Last update: Mon Feb 17 08:43:49 JST 2020

// ---------------------------------------------------------

const int analogInPin0 = A0; // Potentiometer(Left)
const int analogInPin1 = A1; // Potentiometer(Middle)
const int analogInPin2 = A2; // Potentiometer(Right)

const int pushSW0 = A3; // Push button Switch(Left)
const int pushSW1 = A4; // Push button Switch(Middle)
const int pushSW2 = A5; // Push button Switch(Right)

const int analogOutPin0 = 11; // RED LED
const int analogOutPin1 = 10; // GREEN LED
const int analogOutPin2 = 9;  // BLUE LED

const int digitalOutPin = 13;  // Bulit-in LED

int sensorValue[3] = { 0, 0, 0 }; // value read from the potentiometers
int outputValue[3] = { 0, 0, 0 }; // value output to the PWM (LEDs)

boolean modeFlag = false;

// ---------------------------------------------------------

void setup() {

  Serial.begin(57600);

  pinMode(pushSW0, INPUT_PULLUP);
  pinMode(pushSW1, INPUT_PULLUP);
  pinMode(pushSW2, INPUT_PULLUP);

  pinMode(analogOutPin0, OUTPUT);
  pinMode(analogOutPin1, OUTPUT);
  pinMode(analogOutPin2, OUTPUT);

  pinMode(digitalOutPin, OUTPUT);

  analogWrite(analogOutPin0, 255);
  analogWrite(analogOutPin1, 255);
  analogWrite(analogOutPin2, 255);
  delay(1000);
  
  modeFlag = (digitalRead(pushSW2) == HIGH);
}

// ---------------------------------------------------------

void loop() {

  static int redVal = 255, greenVal = 255, blueVal = 255;

  while (Serial.available() > 0) {
    Serial.print("IN: ");
    // Serial.print("# R=");
    int r = Serial.parseInt();  // timeout after 1sec.
    Serial.print(r); Serial.print(" ");// Serial.print(",G=");
    int g = Serial.parseInt();  // timeout after 1sec.
    Serial.print(g); Serial.print(" "); // Serial.print(",B=");
    int b = Serial.parseInt();  // timeout after 1sec.
    Serial.print(b);

    char c;
    while (((c  = Serial.read()) != -1) && (c != '\n') && (c != '\r')) {}   // skip at the end of line
    Serial.println(); // Serial.println("[EOL]");

    redVal   = 255 - constrain(r, 0, 255);
    greenVal = 255 - constrain(g, 0, 255);
    blueVal  = 255 - constrain(b, 0, 255);
  }

  sensorValue[0] = analogRead(analogInPin0);
  sensorValue[1] = analogRead(analogInPin1);
  sensorValue[2] = analogRead(analogInPin2);

  boolean dout;
  
  if (modeFlag) {
    analogWrite(analogOutPin0, redVal);
    analogWrite(analogOutPin1, greenVal);
    analogWrite(analogOutPin2, blueVal);
    dout = ((digitalRead(pushSW0)==0) || (redVal>127)) ? 1 : 0;

  } else {
    analogWrite(analogOutPin0, 255 - sensorValue[0]/4);
    analogWrite(analogOutPin1, 255 - sensorValue[1]/4);
    analogWrite(analogOutPin2, 255 - sensorValue[2]/4);
    dout = ((digitalRead(pushSW0)==0) || (sensorValue[0]>512)) ? 1 : 0;
  }

  
  digitalWrite(digitalOutPin, dout);
  Serial.print("OUT: "); Serial.print(dout); Serial.print(" ");

  Serial.print(sensorValue[0]); Serial.print(" ");
  Serial.print(sensorValue[1]); Serial.print(" ");
  Serial.print(sensorValue[2]); Serial.print(" ");

  Serial.print(digitalRead(pushSW0)); Serial.print(" ");
  Serial.print(digitalRead(pushSW1)); Serial.print(" ");
  Serial.print(digitalRead(pushSW2)); Serial.print(" ");

  Serial.println();

  delay(100);
}

// ---------------------------------------------------------
