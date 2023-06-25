/*
PIN CONFIGURATIONS:
MOTOR DRIVER OUT1 = 5
MOTOR DRIVER OUT2 = 3

IR SENSOR 1 (SENSOR FOR INSERTED TRASH) = 11 

IR SENSOR 2 = 22
IR SENSOR 3 = 23
IR SENSOR 4 = 24

left ultrasonic sensor
trig pin = 4
echo pin = 6

right ultrasonic sensor
trig pin = 7
echo pin = 8

panel ultrasonic sensor
trig pin = 25
echo pin = 26

PROXIMITY SENSOR = 10

magneticdoor = 2

servomotor sorter = 9

pushbutton switch = 12

buzzer = 13

LED RIGHT (Pet bottle trashcan) = A0
LED LEFT (Random trash trashcan) = A1

LCD VIN = 5V
LCD GND = GND
LCD SDA = 20
LCD SCL = 21



*/

#include <LiquidCrystal_I2C.h> // Library for LCD
#include <Servo.h>

// Motor Separator
const int motorAPin1  = 5; 
const int motorAPin2  = 3;

// Magnetic door
const int MagneticDoor = 2;

// Proximity Sensor
const int ProximitySensor = 10;

// IR Sensors
const int IR1 = 11;
const int IR2 = 22;
const int IR3 = 23;
const int IR4 = 24;

// left ultrasonic sensor for trashcan
const int LeftTrigPin = 6;
const int LeftEchoPin = 4;

// right ultrasonic sensor for trashcan
const int RightTrigPin = 8;
const int RightEchoPin = 7;

const int PanelTrigPin = 25;
const int PanelEchoPin = 26;


long panelUltrasonicDuration;

float panel = 0.0;

const int PushButtonSwitch = 12;

const int buzzer = 13;


long Leftduration;
long Rightduration;

float left = 0.0;
float right = 0.0;


Servo servoSorter;

int PetBottlePosition = 180; // eto babaguhin mo pre para 180 na movement ni servo eto dapat gagalaw sya papunta sa petbottle bin
int RandomTrashPosition = 0; // eto babaguhin mo pre para 180 na movement ni servo eto dapat gagalaw sya papunta sa random bin

bool IsTrashBeingInserted = false;

// I2C address 0x27, 16 column and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  // initialize serial comms
  Serial.begin(9600);
  lcd.init(); // initialize the lcd
  lcd.backlight(); // open lcd backlight

 // Setup of PinModes for Motors, IR's, and Proximity Sensor
  pinMode(motorAPin1, OUTPUT); // motor pin 1 - 5
  pinMode(motorAPin2, OUTPUT); // motor pin 2 -  3
  pinMode(ProximitySensor, INPUT); // proximity sensor - 10
  pinMode(IR1, INPUT); // ir sensor = 11
  pinMode(IR2, INPUT); // ir sensor = 22
  pinMode(IR3, INPUT); // ir sensor = 23
  pinMode(IR4, INPUT); // ir sensor = 24
  pinMode(MagneticDoor, INPUT); // magneticdoor - 2
  pinMode(LeftTrigPin, OUTPUT);  // left trig pin = 4
  pinMode(LeftEchoPin, INPUT);  // left echo pin = 6
  pinMode(RightTrigPin, OUTPUT);  // right trig pin = 7
  pinMode(RightEchoPin, INPUT);  // right echo pin = 8
  pinMode(PanelTrigPin, OUTPUT);  // left trig pin = 25
  pinMode(PanelEchoPin, INPUT);  // left echo pin = 26
  pinMode(A0,OUTPUT); // right led A0
  pinMode(A1,OUTPUT); // left led A1
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  pinMode(PushButtonSwitch, INPUT_PULLUP); // push button 

  servoSorter.attach(9); // servo motor = 9
  servoSorter.write(PetBottlePosition); 
}

void loop() {
  int counter = 0;
  int MagneticDoorValue = digitalRead(MagneticDoor);
  LeftUltrasonicSensor();
  RightUltrasonicSensor();
  
    int IR1State = digitalRead(IR1); 
    Serial.println(IR1State);

  if (right <= 3.0 && left <= 3.0){
    while(true){
      ShowBothTrashcanIsFull();
      int PushBtnValue = digitalRead(PushButtonSwitch);
      if(PushBtnValue == 0){
        digitalWrite(A0,LOW);
        digitalWrite(A1,LOW);
        noTone(buzzer); 
        break;
      }
    }
  }else if (right <= 3.0){
    while(true){
      ShowTrashcanForBottlesAreFull();
      int PushBtnValue = digitalRead(PushButtonSwitch);
      if(PushBtnValue == 0){
        digitalWrite(A0,LOW);
        noTone(buzzer); 
        break;
      }
    }
  }else if (left <= 3.0){
    while(true){
      ShowTrashcanForRandomTrashAreFull();
      int PushBtnValue = digitalRead(PushButtonSwitch);
      if(PushBtnValue == 0){
        digitalWrite(A1,LOW);
        noTone(buzzer); 
        break;
      }
    }
  }else{
    ShowDefaultLCDMessage();
  }

  Serial.println(MagneticDoorValue);

  if (MagneticDoorValue == 1){
    IsTrashBeingInserted = true;
    // Show Message that Trash has been Inserted
    ShowTrashInsertedMessage();
  }


  MagneticDoorValue = digitalRead(MagneticDoor);

  while (IsTrashBeingInserted && MagneticDoorValue == 0){
    PanelUltrasonicSensor();
    while (panel >= 4){
      PanelUltrasonicSensor();
      ForwardMotor();
      tone(buzzer,1000);
      delay(200); // palitan mo tong delay na to kasi eto ung time na iikot ung converyor belt tapos mag iistop siya.
      StopMotor();
      noTone(buzzer); 
      delay(1000);
      counter++;
      if (counter == 50){ // change this to stop motor.
        break;
      }
    }

    // this means that there is no more trash to be separated
    if (counter == 50){
      ShowSegregationCompletedMessage();
      IsTrashBeingInserted = false;
      break;
    }else{
      counter = 0;
    }

    
    int IR1State = digitalRead(IR1); 
    int IR2State = digitalRead(IR2); 
    int IR3State = digitalRead(IR3); 
    int IR4State = digitalRead(IR4); 

    // // initialize and read Proximity Sensor state variable
    // int ProximitySensorState = digitalRead(ProximitySensor);
    // // Print Proximity Sensor state for testing
    // Serial.println(ProximitySensorState);
    if (IR1State == 1 && IR2State == 1 && IR3State == 1 && IR4State == 1){
      // Show message that pet bottle has been inserted
      ShowPetBottleHasBeenDetected();
      // Show Segragating message in the LCD
      ShowSegregatingMessage();
      // Segregate the pet bottles by moving the Motors in Counter-Clockwise Direction (LEFT SIDE TRASHBIN)
      SegregatePetBottles();
    }else{
      // Show message that pet bottle has been inserted
      ShowRandomTrashHasbeenDetected();
      // Show Segragating message in the LCD
      ShowSegregatingMessage();
      // Segregate the random by moving the Motors in Clockwise Direction (RIGHT SIDE TRASHBIN)
      SegregateRandomTrash();
    }

  }

}

void LeftUltrasonicSensor() {
  digitalWrite(LeftTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(LeftTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(LeftTrigPin, LOW);
  Leftduration = pulseIn(LeftEchoPin, HIGH);
  float leftdata = Leftduration * 0.0133 / 2;
  left = leftdata;
  Serial.println(left);
}

void RightUltrasonicSensor() {
  digitalWrite(RightTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(RightTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(RightTrigPin, LOW);
  Rightduration = pulseIn(RightEchoPin, HIGH);
  float rightdata = Rightduration * 0.0133 / 2;
  right = rightdata;
  Serial.println(right);
}

// void function for showing trash has been inserted Message in the LCD
void ShowTrashInsertedMessage(){
  
  lcd.clear();
  lcd.setCursor(1, 0);         
  lcd.print("Trash Inserted");
  delay(3000);
}

// void function for showing pet bottle has been detected Message in the LCD
void ShowPetBottleHasBeenDetected(){
  lcd.clear();
  lcd.setCursor(3, 0);         
  lcd.print("Pet bottle");      
  lcd.setCursor(2, 1);        
  lcd.print("is detected"); 
  delay(1000);
}

// void function for showing random trash has been detected Message in the LCD
void ShowRandomTrashHasbeenDetected(){
  lcd.clear();
  lcd.setCursor(2, 0);         
  lcd.print("Random trash");      
  lcd.setCursor(2, 1);        
  lcd.print("is detected"); 
  delay(1000);
}

// void function for showing Segragating Message in the LCD
void ShowSegregatingMessage(){
  lcd.clear();
  lcd.setCursor(0, 0);         
  lcd.print("Segregating.....");
  delay(1000);
}

// void function for showing the segregation has been completed Message in the LCD
void ShowSegregationCompletedMessage(){
  lcd.clear();
  lcd.setCursor(2, 0);         
  lcd.print("Segratation");      
  lcd.setCursor(3, 1);        
  lcd.print("Completed"); 
  delay(2000);
}

// void function for showing the Segregation will not continue please empty trashbin Message in the LCD
void ShowSegregationWillNotContinueMessage(){
  lcd.clear();
  lcd.setCursor(0, 0);         
  lcd.print("Segratation will");      
  lcd.setCursor(2, 1);        
  lcd.print("not continue"); 
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);         
  lcd.print("Please empty the");      
  lcd.setCursor(0, 1);        
  lcd.print("bin immediately"); 
  delay(2000);
}

// void function that will move both motors in the clockwise direction (RIGHT TRASHBIN)
void SegregateRandomTrash(){
  // servoSorter.attach(9);
  // servoSorter.write(180); // palitan mo to ng 0 para umikot ng opposite
  // delay(3000); // palitan mo to kung need mo magdagdag or magbawas ng time ng pagikot ng servo
  // servoSorter.detach();

  servoSorter.write(RandomTrashPosition);
  delay(1500);
  //This code  will turn Motor A clockwise for 3 sec.
  analogWrite(motorAPin1, 255);
  analogWrite(motorAPin2, 0);
  delay(500); // palitan mo tong delay na to kasi eto ung time na iikot ung converyor belt tapos mag iistop siya.
  // This code will stop motor A
  analogWrite(motorAPin1, 0);
  analogWrite(motorAPin2, 0);
}

// void function that will move both motors in the counter-clockwise direction (LEFT TRASHBIN)
void SegregatePetBottles(){
  // servoSorter.attach(9);
  // servoSorter.write(0); // palitan mo to ng 180 para umikot ng opposite
  // delay(3000); // palitan mo to kung need mo magdagdag or magbawas ng time ng pagikot ng servo
  // servoSorter.detach();

  servoSorter.write(PetBottlePosition);
  delay(1500);
  //This code  will turn Motor A and B clockwise for 3 sec.
  analogWrite(motorAPin1, 255);
  analogWrite(motorAPin2, 0);
  delay(500); // palitan mo tong delay na to kasi eto ung time na iikot ung converyor belt tapos mag iistop siya.
  // This code will stop motor A
  analogWrite(motorAPin1, 0);
  analogWrite(motorAPin2, 0);
}

// void function for Showing Default Message in the LCD
void ShowDefaultLCDMessage(){
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("AUTOMATED BOTTLE"); 
  lcd.setCursor(3, 1); 
  lcd.print("SEGREGATOR"); 
  delay(500);

  lcd.clear();                 
  lcd.setCursor(1, 0);         
  lcd.print("Please insert");      
  lcd.setCursor(5, 1);        
  lcd.print("Trash!"); 
  delay(500); 
}

// void function for Showing Pet Bottle Trash bin is full Message in the LCD
void ShowTrashcanForBottlesAreFull(){
  lcd.clear();
  lcd.setCursor(3, 0);         
  lcd.print("Pet bottle");      
  lcd.setCursor(2, 1);        
  lcd.print("bin is full"); 
  digitalWrite(A0,HIGH);
  tone(buzzer,1000);
  delay(1000);
  digitalWrite(A0,LOW);
  noTone(buzzer); 
  delay(1000);
}

// void function for Showing Random Trash Trashbin is full Message in the LCD
void ShowTrashcanForRandomTrashAreFull(){
  lcd.clear();
  lcd.setCursor(2, 0);         
  lcd.print("Random trash");      
  lcd.setCursor(2, 1);        
  lcd.print("bin is full"); 
  digitalWrite(A1,HIGH);
  tone(buzzer,1000);
  delay(1000);
  digitalWrite(A1,LOW);
  noTone(buzzer); 
  delay(1000);
}

// void function for Showing Both Trashbins are full Message in the LCD
void ShowBothTrashcanIsFull(){
  lcd.clear();
  lcd.setCursor(3, 0);         
  lcd.print("Both Trash");      
  lcd.setCursor(2, 1);        
  lcd.print("bin is full");
  digitalWrite(A0,HIGH);
  digitalWrite(A1,HIGH);
  tone(buzzer,1000);
  delay(1000);
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  noTone(buzzer); 
  delay(1000);
}


void ForwardMotor(){
  analogWrite(motorAPin1, 255);
  analogWrite(motorAPin2, 0);
}

void StopMotor(){
  // This code will stop motor A and Motor B
  analogWrite(motorAPin1, 0);
  analogWrite(motorAPin2, 0);
}

void PanelUltrasonicSensor() {
  digitalWrite(PanelTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(PanelTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(PanelTrigPin, LOW);
  panelUltrasonicDuration = pulseIn(PanelEchoPin, HIGH);
  float paneldata = panelUltrasonicDuration * 0.0133 / 2;
  panel = paneldata;
  Serial.println(panel);
}
