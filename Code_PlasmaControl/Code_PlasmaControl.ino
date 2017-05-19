#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3E, 16, 2);

int PUL=7; //define Pulse pin
int DIR=6; //define Direction pin
int ENA=5; //define Enable Pin
const int statusPin = 2; //pin status on-off
const int motorUp = 3; //pin  switch motor up
const int motorDown = 4; //pin  sitch motor down 
const int startpin = 12;

const int relayTop = 8; //pin relay Top
const int relayDown = 9; //pin relay Down 

int start=0;
int statusState = 0; //status
int statusUp = 0;
int statusDown =0;

int statusRelayTop = 0;
int statusRelayDown = 0;

int statusU = 0;
int statusD = 0;

float voltset = 0.0;
int statevolt =0;
int hh=1; //set head high
int fin = 0;
int reset=0;
int startn1 = 0;



void setup() {
  pinMode(startpin, INPUT);
  pinMode(10, OUTPUT);
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode(statusPin, INPUT);
  pinMode(relayTop, INPUT);
  pinMode(relayDown, INPUT);
 // initialize the LCD
 lcd.begin();
 // Turn on the blacklight and print a message.
 lcd.backlight();
}

void loop() {
  
  start = digitalRead(startpin);
  statusState = digitalRead(statusPin);
  statusRelayTop = digitalRead(relayTop);
  statusRelayDown = digitalRead(relayDown);
  statusUp = digitalRead(motorUp);
  statusDown = digitalRead(motorDown);
  
  int sensorValue = analogRead(A1);
  float vin = sensorValue * (5.0 / 1023.0);
  //manualMotor();

  if(start == HIGH){
    startn1 = 1;
    delay(500);
  }
  else if(statusUp == HIGH && statusDown == LOW){
    manualMotor();
  }
  else if(statusDown == HIGH && statusUp == LOW){
    manualMotor();
  }

  else if(startn1 == 1){
    if(vin<=2){
        readycut();
        startn1 = 0;
    }
    else{
      downfin();
    }
  }
  
  else if(statusState == HIGH){
    digitalWrite(10,HIGH);
    if(vin<=2){
        readycut();
        startn1 = 0;
    }
    else{
      downfin();
    }
  }
 
 else{
  digitalWrite(10,LOW);
  statusU = 0;
  statusD = 0;
  sethigh();
 }
}


void readycut(){
  for(int i;i<=voltset*1000.0;i++){
    upmotor();
  }
}

void sethigh(){
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  voltset = voltage/3;

  lcd.setCursor(0, 0);
  lcd.print("Controller");
  lcd.setCursor(0, 1);
  lcd.print("High :      cm.");
  lcd.setCursor(7, 1);
  lcd.print(voltset);
  delay(100);
}

void motorControl(){
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  if(voltage>voltset+0.2){
    upmotor();
  }
  if(voltage<voltset-0.2){
    downmotor();
  }
}

void manualMotor(){
  if(statusUp == HIGH && statusDown == LOW){
    upmotor();
    if(statusRelayTop == HIGH){
      for(int i;i<=2000;i++){
        downmotor();
        }
    }
   }
   
   if(statusDown == HIGH && statusUp == LOW){
    downmotor();
    if(statusRelayDown == HIGH){     
      for(int i;i<=2000;i++){
        upmotor();
        }
    }
   }
   
}

void downfin(){
  if(statusD == 0){
    downmotor();
    if(statusRelayDown == HIGH){
      statusD = 1;
      if(statusD == 1){
        upmotor();
        if(statusRelayTop == HIGH){
          for(int i;i<=2000;i++){
            downmotor();
          }
        }
      }
    }
  }
}


void upfin(){
  if(statusU == 0){
    upmotor();
    if(statusRelayTop == HIGH){
      statusU = 1;
      for(int i;i<=2000;i++){
        downmotor();
      }
    }
  }
}


void upmotor(){
  digitalWrite(DIR,HIGH);
  digitalWrite(ENA,HIGH);
  digitalWrite(PUL,HIGH);
  delayMicroseconds(50);
  digitalWrite(PUL,LOW);
  delayMicroseconds(50);
}
void downmotor(){
  digitalWrite(DIR,LOW);
  digitalWrite(ENA,HIGH);
  digitalWrite(PUL,HIGH);
  delayMicroseconds(50);
  digitalWrite(PUL,LOW);
  delayMicroseconds(50);
}

