/*
 AP_SYNC 
*/
#include <AP_Sync.h>
AP_Sync streamer(Serial);

/* 
PRESSURE SENSORS 
*/

//Y, G, B, P

// right heal
int purpleFSR = 0; // PURPLE FSR is connected to analog 0
int purpleLED = 10;      // connect PURPLE LED to pin 10 (PWM pin)
int purpleReading = 0;      // the analog reading from the FSR resistor divider
int purpleBrightness = 0;

// left index
int blueFSR = 1; // BLUE FSR is connected to analog 1
int blueLED = 9;      // connect BLUE LED to pin 11 (PWM pin)
int blueReading = 0;      // the analog reading from the BLUE FSR resistor divider
int blueBrightness = 0;

// right index
int greenFSR = 2; // GREEN FSR is connected to analog 2
int greenLED = 6;      // connect GREEN LED to pin 12 (PWM pin)
int greenReading = 0;      // the analog reading from the GREEN FSR resistor divider
int greenBrightness = 0;

// left palm
int yellowFSR = 3; // YELLOW FSR is connected to analog 3
int yellowLED = 5;      // connect YELLOW LED to pin 13 (PWM pin)
int yellowReading = 0;      // the analog reading from the YELLOW FSR resistor divider
int yellowBrightness = 0;

/*
  TACHOMETER
*/

// this constant won't change:
int hallPin = 2;    // the pin that the pushbutton is attached to
int hallPinState = 0;         // current state of the button
int lastHallPinState = 0;     // previous state of the button

int hallLED = 13;
int hallLEDState = LOW;

int counter = 0;   // counter for the number of button presses
unsigned long then = 0;
unsigned long rpm = 0;
int blinkRate = 0;




void setup(void) {

//  PRESSURE SENSORS

  pinMode(purpleFSR, INPUT);
  pinMode(blueFSR, INPUT);
  pinMode(greenFSR, INPUT);
  pinMode(yellowFSR, INPUT);
  
  pinMode(purpleLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);

// TACHOMETER
  pinMode(hallPin, INPUT);
  pinMode(hallLED, OUTPUT);
  
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
//  Serial.println("ALL SET. LET REVOLUTION(S) BEGIN!");
}
 
void loop(void) {

  // PRESSURE SENSORS
  
  purpleReading = analogRead(purpleFSR);
  blueReading = analogRead(blueFSR);
  greenReading = analogRead(greenFSR);
  yellowReading = analogRead(yellowFSR);

  purpleBrightness = map(purpleReading, 0, 1023, 0, 255);
  blueBrightness = map(blueReading, 0, 1023, 0, 255);
  greenBrightness = map(greenReading, 0, 1023, 0, 255);
  yellowBrightness = map(yellowReading, 0, 1023, 0, 255);
  
  // LED gets brighter the harder you press
  analogWrite(purpleLED, purpleBrightness);
  analogWrite(blueLED, blueBrightness);
  analogWrite(greenLED, greenBrightness);
  analogWrite(yellowLED, yellowBrightness);

  // TACHOMETER

  unsigned long now = millis();
  unsigned long timepassed = now - then;

  if(timepassed >= 10000){
  
    rpm = (counter*6);

    Serial.print("RPM's: ");
    Serial.println(rpm);
    
    counter = 0;
    then = millis();
  }

  blinkRate = rpm/60;

  if(rpm > 0){ 
    
    if (hallLEDState == LOW) {
      hallLEDState = HIGH;
    } else {
      hallLEDState = LOW;
    }
      
  } else {
    hallLEDState = LOW;
  }

  digitalWrite(hallLED, hallLEDState);
  delay(blinkRate);

  
  hallPinState = digitalRead(hallPin);
  
  // compare the hallPinState to its previous state
  if (hallPinState != lastHallPinState) {
    
//    reportChange();

    // if the state has changed, increment the counter
    if (hallPinState == HIGH) {
//      reportApplied();
      // if the current state is HIGH then the button went from off to on:
      counter++;
//      Serial.print("TOTAL HALL PASSES: ");
//      Serial.println(counter);
    } else {
      // if the current state is LOW then the button went from on to off:
//      reportRemoved();
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastHallPinState = hallPinState;

  streamer.sync("Y",yellowReading);
  streamer.sync("G",greenReading);
  streamer.sync("B",blueReading);
  streamer.sync("P",purpleReading);
  streamer.sync("H",rpm);

 
//  delay(50);
}



//  String row = String(yellowReading) + "," + String(blueReading) + "," +
//             String(greenReading) + "," + String(purpleReading) + "," +
//             String(rpm);
//
//  Serial.println(row);
