//Import the library in the sketch
#include <SharpIR.h>
//#include <Keyboard.h>

#define MODEL0 SharpIR::GP2Y0A21YK0F // The model of the sensor is "GP2Y0A21YK0F" for 5cm to 80cm
#define MODEL1 SharpIR::GP2Y0A02YK0F // The model of the sensor is "GP2Y0A02YK0F" for 20cm to 150cm

#define MODEL MODEL0

//Create a new instance of the library
//Call the sensor "sensor"
//The sensor output pin is attached to the pin A0
SharpIR sensor( MODEL, A0 );

// SharpIR sensor codes
const int SHORTDISTANCE = 1;
const int LONGDISTANCE = 3;

// Set of possible actions
const int START = 0;
const int STOP = 1;
const int PREVIOUS = 2;
const int NEXT = 3;
const int VOLUMEUP = 4;
const int VOLUMEDOWN = 5;
const int RESET_VALUE = 100;

// LEDs
const int RED_PIN = 13;
const int YELLOW_PIN = 12;
const int GREEN_PIN = 11;

// variables
int section, prevSection, change;
int PREV_ACTION = RESET_VALUE; // initial value out of action set
int AUX_ACTION = RESET_VALUE; // initial value out of action set

// initialize ranges limit points
int RANGE0, RANGE1, RANGE2, RANGE3, RANGE4, RANGE5, RANGE6 = 0;

// control flags
bool paused; // 0 = false

// action functions
void execAction(int action, int prevAction, int auxAction){
  switch (action) {
    case STOP:
    case START:
      if (action == prevAction){
        Serial.println((paused==true) ? ("START") : ("STOP"));
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
      }
      break;
    default:
      Serial.println("no action");
  }
}

void detectArea(int distance){
  if (distance > RANGE0 && distance < RANGE2){
    execAction(PREVIOUS, PREV_ACTION, AUX_ACTION);
    PREV_ACTION = PREVIOUS;
  }
  else if (distance >= RANGE2 && distance < RANGE4){
    if (paused){
      execAction(START, PREV_ACTION, AUX_ACTION);
      PREV_ACTION = START;
    }
    else{
      execAction(STOP, PREV_ACTION, AUX_ACTION);
      PREV_ACTION = STOP;
    }
  }
  else if (distance >= RANGE4 && distance < RANGE6){
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
    if (VOLUMEDOWN == PREV_ACTION){
      AUX_ACTION = VOLUMEDOWN;
    }
    else{
      PREV_ACTION = VOLUMEDOWN;
    }
  }
}

void setup(){
  Serial.begin(9600); //Enable the serial comunication
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  switch(MODEL){
    case SHORTDISTANCE:
      RANGE0 = 5;
      RANGE1 = 15;
      RANGE2 = 25;
      RANGE3 = 35;
      RANGE4 = 45;
      RANGE5 = 55;
      RANGE6 = 65;
      break;
    case LONGDISTANCE:
      RANGE0 = 20;
      RANGE1 = 30;
      RANGE2 = 40;
      RANGE3 = 50;
      RANGE4 = 60;
      RANGE5 = 70;
      RANGE6 = 80;
      break;
    default:
      Serial.println("Model not detected. Check hardware setup.");
  }
}

void loop(){
  delay(150);
  int distance = sensor.getDistance(); //Calculate the distance in centimeters and store the value in a variable
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  
  if(distance < RANGE0){
    section = 0;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance >= RANGE0 && distance <= RANGE1){
    section = 1;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance > RANGE1 && distance <= RANGE2){
    section = 2;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance > RANGE2 && distance <= RANGE3){
    section = 3;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance >= RANGE3 && distance <= RANGE4){
    section = 4;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance >= RANGE4 && distance <= RANGE5){
    section = 5;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if(distance >= RANGE5 && distance <= RANGE6){
    section = 6;
    change = section - prevSection;
    defineAction(change, distance);
  }
  else if (distance > RANGE6){
   section = 7;
   change = section - prevSection;
   defineAction(change, distance);
  }

  prevSection = section;
}
