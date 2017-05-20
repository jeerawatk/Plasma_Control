// Plasma Control V.2
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3E, 16, 2);

//pin
int PUL = ; //define Pulse pin
int DIR = ; //define Direction pin
int ENA = ; //define Enable Pin
const int modepin = ;  //pin mode
const int leftpin = ; //pin left
const int rightpin = ; //pin right
const int enterpin = ; // pint start or pin OK
const int relayTop = ; //pin relay Top
const int relayDown = ; //pin relay Down 

int moden = 0;
int templcd = 0;

int mode = 0;
int left = 0;
int right = 0;
int enter = 0;

float h = 0.0;

void setup() {
  lcd.begin(); // initialize the LCD
  lcd.backlight(); // Turn on the blacklight and print a message.
  pinMode(modepin, INPUT); 
  pinMode(leftpin, INPUT);
  pinMode(rightpin, INPUT);
  pinMode(enterpin, INPUT);
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode(relayTop, INPUT);
  pinMode(relayDown, INPUT);
}

void loop() {
  mode = digitalRead(startpin);
  left = digitalRead(leftpin);
  right = digitalRead(rightpin);
  enter = digitalRead(enterpin);
  statusRelayTop = digitalRead(relayTop);
  statusRelayDown = digitalRead(relayDown);

  //sensor
  int sensorValue = analogRead(A1);
  float vin = sensorValue 

  if(mode == HIGH){
    moden++;
    delay(200);
  }
  
  //redy mode 
  if(moden == 0){
    
  }

  //setting mode
  if(moden == 1){
    sethigh();
  }

  //manual control motor mode
  if(moden == 2){
    templcd = 0;
    if(templcd == 0){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Manual Control");
      lcd.setCursor(0, 1);
      lcd.print("L:Up  R:Down");
      templcd = 1;
    }
    else{
      manualMotor();
    }
  }
  
  if(moden == 3){
    moden = 0; // set default 
  }
}



void sethigh(){
  if(left == HIGH && right == LOW){
      h=h-0.5;
    }
    if(right == HIGH && left == LOW){
      h=h+0.5;
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Setting Mode");
      lcd.setCursor(0, 1);
      lcd.print("High :      cm.");
      lcd.setCursor(7, 1);
      lcd.print(h);
    }
    delay(100);
}



//left up , right down
void manualMotor(){
  if(left == HIGH && right == LOW){
    upmotor();
    if(statusRelayTop == HIGH){
      for(int i;i<=2000;i++){
        downmotor();
        }
    }
   }
   
   if(right == HIGH && left == LOW){
    downmotor();
    if(statusRelayDown == HIGH){     
      for(int i;i<=2000;i++){
        upmotor();
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
