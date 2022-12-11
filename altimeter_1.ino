#include <EEPROM.h>
#include <Servo.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

//servos are connected to respective pyro channels
int errorState = 0;
int flightMode = 1;
float groundAlt;
float trueAlt;
float currentAlt;
float currentAlt2;
float apogeeAlt;
float eepromAlt;
int mainDep = 50; //main chute deployment altitude
Adafruit_BMP280 bmp;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(2,OUTPUT);
  Serial.println("Altimeter from Brian Wong!");
  Serial.println("Last altitude reading:");
  Serial.println(EEPROM.get(0, eepromAlt));
  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
    Serial.println("no baro");
    errorState = 1;
  }
  led1b(1); //boot sequence
  delay(1000);
  groundAlt = bmp.readAltitude(1013.25);
  led1b(0);
  delay(500);
  led1g(1);
  delay(100);
  led1g(0);
}

void loop() {
  // put your main code here, to run repeatedly
  if (errorState == 1){
    error1();
  }
  if (flightMode == 1){
    coast();
  }
  if (flightMode == 2){
    descent();
  }
  trueAlt = (bmp.readAltitude(1013.25) - groundAlt);
  Serial.println(trueAlt);
}

void coast(){
  currentAlt = (bmp.readAltitude(1013.25));
  delay(500);
  currentAlt2 = (bmp.readAltitude(1013.25));
  if (currentAlt2 <= (currentAlt - 0.5)){
    flightMode = 2;
    apogeeAlt = trueAlt;
    //apogeeAlt = currentAlt;
    eepromAlt = apogeeAlt;y
    EEPROM.put(0, eepromAlt);
    descent();
  }
}

void descent(){
  Serial.println("Descending");
  Serial.println(apogeeAlt);
  pyro1(1);
  if (currentAlt <= mainDep){
    pyro2(1);
  }
  led1g(1);
  delay(500);
  led1g(0);
  delay(500);
}

//library thingy

void error1(){ //red led blinking
  Serial.println("Error 1");
  led1r(1);
  delay(500);
  led1r(0);
  delay(500);
}

void pyro1(int pyro1input){
  if (pyro1input == 0){
    digitalWrite(5,LOW);
  }
  if (pyro1input == 1){
    digitalWrite(5,HIGH);
  }
}

void pyro2(int pyro2input){
  if (pyro2input == 0){
    digitalWrite(6,LOW);
  }
  if (pyro2input == 1){
    digitalWrite(6,HIGH);
  }
}

void led1r(int led1rinput){
  if (led1rinput == 0){
    digitalWrite(2,LOW);
  }
  if (led1rinput == 1){
    digitalWrite(2,HIGH);
  }
}

void led1g(int led1ginput){
  if (led1ginput == 0){
    digitalWrite(4,LOW);
  }
  if (led1ginput == 1){
    digitalWrite(4,HIGH);
  }
}

void led1b(int led1binput){
  if (led1binput == 0){
    digitalWrite(3,LOW);
  }
  if (led1binput == 1){
    digitalWrite(3,HIGH);
  }
}
