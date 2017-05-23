// Plasma Control V.2
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3E, 16, 2);

//pin
int PUL = 9; //define Pulse pin
int DIR = 8; //define Direction pin
int ENA = 7; //define Enable Pin
const int modepin = 4;  //pin mode
const int leftpin = 5; //pin left
const int rightpin = 6; //pin right
const int enterpin = 3; // pint start or pin OK
const int relayTop = 11; //pin relay Top
const int relayDown = 12; //pin relay Down 
const int cutpin = 2;

int moden = 0;
int templcd = 0;
int tempenter = 0;
int lcdcut = 0;

int mode = 0;
int left = 0;
int right = 0;
int enter = 0;
int statusRelayTop;
int statusRelayDown;
int cuting;
int cuttemp = 0;

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
  pinMode(cutpin, INPUT);
  pinMode(10, OUTPUT); // on-off Plasma
}

void loop() {
  
  mode = digitalRead(modepin);
  left = digitalRead(leftpin);
  right = digitalRead(rightpin);
  enter = digitalRead(enterpin);
  statusRelayTop = digitalRead(relayTop);
  statusRelayDown = digitalRead(relayDown);
  cuting = digitalRead(cutpin);

  if(mode == HIGH){
    moden++;
    delay(200);
  }
  
  //Ready mode 
  if(moden == 0){
    if(templcd == 0){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Are you Ready ? ");
      //lcd.setCursor(8, 0);
      //lcd.print(vin);
      lcd.setCursor(0, 1);
      lcd.print("Status : OFF");
      templcd = 1;
    }
    else if(enter == HIGH){
      tempenter = 1;
      lcd.setCursor(0, 1);
      lcd.print("Status : Wait");
    }
    else if(tempenter == 1){
      downcut();
    }
    else if(cuting == HIGH){
      if(cuttemp == 0){
        digitalWrite(10,HIGH);
        downcuting();
    }
    /*
    else if(cuttemp == 1){
      if(lcdcut == 0){
        lcd.setCursor(0, 1);
        lcd.print("Status : ON   ");
        lcdcut = 1;
        //digitalWrite(10,HIGH);
      }
      downcut();
      */
    }
    else if(cuting == LOW){
      digitalWrite(10,LOW);
      lcd.setCursor(0, 1);
      lcd.print("Status : OFF  ");
      cuttemp = 0;
    }
  }

  //setting mode
  if(moden == 1){
    templcd = 0;
    sethigh();
  }

  //manual control motor mode
  if(moden == 2){
    if(templcd == 0){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Manual Control");
      lcd.setCursor(0, 1);
      lcd.print("L:Up  R:Down");
      templcd = 1;
    }
    if(templcd == 1){
      manualMotor();
    }
  }
  
  if(moden == 3){
    templcd = 0;
    moden = 0; // set default 
  }
}

void downcut(){
  int sensorValue = analogRead(A0);
  float vin = sensorValue;
  if(vin <= 4.0){
        upset();
        lcd.setCursor(0, 1);
        lcd.print("Status : Ready");
        tempenter = 0;
        cuttemp = 1;
      } 
  else{
        downmotor();
        
        if(statusRelayDown == HIGH){     
          for(int i;i<=2000;i++){
            upmotor();
            }
            tempenter = 0;
            cuttemp = 1;
        }
        
      }
}
void downcuting(){
  int sensorValue = analogRead(A0);
  float vin = sensorValue;
  if(vin <= 4.0){
        delay(3000);
        upset();
        lcd.setCursor(0, 1);
        lcd.print("Status : Ready");
        tempenter = 0;
        cuttemp = 1;
      } 
  else{
        downmotor();
        
        if(statusRelayDown == HIGH){     
          for(int i;i<=2000;i++){
            upmotor();
            }
            tempenter = 0;
            cuttemp = 1;
        }
        
      }
}

void upset(){
  for(int i=0;i<=h*1000;i++){
    upmotor();
  }
}

void sethigh(){
  if(left == HIGH && right == LOW){
      h=h-1;
    }
  else if(right == HIGH && left == LOW){
      h=h+1;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Setting Mode");
    lcd.setCursor(0, 1);
    lcd.print("High :      mm.");
    lcd.setCursor(7, 1);
    lcd.print(h);
    
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
   
   else if(right == HIGH && left == LOW){
    downmotor();
    /*
    if(statusRelayDown == HIGH){     
      for(int i;i<=2000;i++){
        upmotor();
        }
    }
    */
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
