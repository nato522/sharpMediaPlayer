/*
 * sharpHandler
 *
 * Example of using SharpIR library to calculate the distance between the sensor and an obstacle
 *
 * Created by Giuseppe Masino, 15 June 2016
 *
 * -----------------------------------------------------------------------------------
 *
 * Things that you need:
 * - Arduino
 * - A Sharp IR Sensor
 *
 *
 * The circuit:
 * - Arduino 5V -> Sensor's pin 1 (Vcc)
 * - Arduino GND -> Sensor's pin 2 (GND)
 * - Arduino pin A0 -> Sensor's pin 3 (Output)
 *
 *
 * See the Sharp sensor datasheet for the pin reference, the pin configuration is the same for all models.
 * There is the datasheet for the model GP2Y0A41SK0F:
 *
 * http://www.robotstore.it/open2b/var/product-files/78.pdf
 *
 */

//import the library in the sketch
#include <SharpIR.h>

//Create a new instance of the library
//Call the sensor "sensor"
//The model of the sensor is "GP2Y0A02YK0F"
//The sensor output pin is attached to the pin A0
SharpIR sensor( SharpIR::GP2Y0A02YK0F, A0 );

// constants. Set of possible actions
const int START = 0;
const int STOP = 1;
const int PREVIOUS = 2;
const int NEXT = 3;
const int VOLUMEUP = 4;
const int VOLUMEDOWN = 5;
const int RESET_VALUE = 100;

// lEDs
int RED_PIN = 13;
int YELLOW_PIN = 12;
int GREEN_PIN = 11;

// variables
int section, prevSection, change;
int PREV_ACTION = RESET_VALUE; // initial value out or action set
int AUX_ACTION = RESET_VALUE; // initial value out or action set

// control flags
bool paused; // 0 = false

// action functions
void execAction(int action, int prevAction, int auxAction){
  switch (action) {
    case STOP:
    case START:
      if (action == prevAction){
        Serial.println((paused=true) ? ("START") : ("STOP"));
        paused = !paused;
        digitalWrite(RED_PIN, LOW);
        digitalWrite(YELLOW_PIN, HIGH);
        digitalWrite(GREEN_PIN, LOW);
        delay(500);
      }
      break;
    case PREVIOUS:
      if (action == prevAction){
        Serial.println("PREVIOUS");
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(YELLOW_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        delay(500);
      }
      break;
    case NEXT:
      if (action == prevAction){
        Serial.println("NEXT");
        digitalWrite(RED_PIN, LOW);
        digitalWrite(YELLOW_PIN, LOW);
        digitalWrite(GREEN_PIN, HIGH);
        delay(500);
      }
      break;
    case VOLUMEUP:
      if ((action == prevAction)&&(action == auxAction)){
        Serial.println("Volume UP");
        digitalWrite(RED_PIN, HIGH);
        delay(100);
        digitalWrite(RED_PIN, LOW);
        delay(100);
        digitalWrite(YELLOW_PIN, HIGH);
        delay(100);
        digitalWrite(YELLOW_PIN, LOW);
        delay(100);
        digitalWrite(GREEN_PIN, HIGH);
        delay(100);
        digitalWrite(GREEN_PIN, LOW);
      }
      break;
    case VOLUMEDOWN:
      if ((action == prevAction)&&(action == auxAction)){
        Serial.println("Volume DOWN");
        digitalWrite(GREEN_PIN, HIGH);
        delay(100);
        digitalWrite(GREEN_PIN, LOW);
        delay(100);
        digitalWrite(YELLOW_PIN, HIGH);
        delay(100);
        digitalWrite(YELLOW_PIN, LOW);
        delay(100);
        digitalWrite(RED_PIN, HIGH);
        delay(100);
        digitalWrite(RED_PIN, LOW);
        delay(100);
      }
      break;
    default:
      Serial.println("no action");
  }
}

void detectArea(int distance){
  if (distance > 20 && distance < 40){
    execAction(PREVIOUS, PREV_ACTION, AUX_ACTION);
    PREV_ACTION = PREVIOUS;
  }
  else if (distance >= 40 && distance < 60){
    if (paused){
      execAction(START, PREV_ACTION, AUX_ACTION);
      PREV_ACTION = START;
    }
    else{
      execAction(STOP, PREV_ACTION, AUX_ACTION);
      PREV_ACTION = STOP;
    }
  }
  else if (distance >= 60 && distance < 80){
    execAction(NEXT, PREV_ACTION, AUX_ACTION);
    PREV_ACTION = NEXT;
  }
  else{
    //Serial.println("Out of range");
    PREV_ACTION = RESET_VALUE;
    AUX_ACTION = RESET_VALUE;
  }
}

void defineAction(int change, int distance){
  if (change > 0){
    execAction(VOLUMEUP, PREV_ACTION, AUX_ACTION);
    if (VOLUMEUP == PREV_ACTION)
      AUX_ACTION = VOLUMEUP;
    else
      PREV_ACTION = VOLUMEUP;
  }
  else if (change == 0){
    detectArea(distance);
  }
  else{
    execAction(VOLUMEDOWN, PREV_ACTION, AUX_ACTION);
    if (VOLUMEUP == PREV_ACTION)
      AUX_ACTION = VOLUMEDOWN;
    else
      PREV_ACTION = VOLUMEDOWN;
  }
}

void setup()
{
  Serial.begin(9600); //Enable the serial comunication
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop()
{
  delay(150);
  int distance = sensor.getDistance(); //Calculate the distance in centimeters and store the value in a variable
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  /*Serial.println("-----------------------------------------------------");
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("PREV_ACTION: ");
  Serial.println(PREV_ACTION);
  Serial.println("-----------------------------------------------------");*/

  if(distance < 20){
    section = 0;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance >= 20 && distance <= 30){
    section = 1;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance > 30 && distance <= 40){
    section = 2;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance > 40 && distance <= 50){
    section = 3;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance >= 50 && distance <= 60){
    section = 4;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance >= 60 && distance <= 70){
    section = 5;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance >= 70 && distance <= 80){
    section = 6;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if (distance > 80){
   section = 7;
   change = section - prevSection;
   defineAction(change, distance);
  }

  prevSection = section;
}
