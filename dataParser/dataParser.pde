import processing.serial.*;

Serial myPort;

void setup(){
  myPort = new Serial(this, Serial.list()[1], 9600);
  println(myPort);
}
