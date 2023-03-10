/*
PIN CONFIGURATIONS:
MOTOR DRIVER OUT1 = 9
MOTOR DRIVER OUT2 = 6
MOTOR DRIVER OUT3 = 5
MOTOR DRIVER OUT4 = 3

IR SENSOR 1 (SENSOR FOR INSERTED TRASH) = 11 
IR SENSOR 2 (SENSOR FOR PET BOTTLE TRASHBIN) = 12
IR SENSOR 3 (SENSOR FOR RANDOM TRASH TRASHBIN) = 13

PROXIMITY SENSOR = 10

LCD VIN = 5V
LCD GND = GND
LCD SDA = A4
LCD SCL = A5

ACTUAL PROTOTYPE TRASHBIN CONFIGURATION:
LEFT TRASHBIN = PET BOTTLE TRASHBIN
RIGHT TRASHBIN = RANDOM TRASH TRASHBIN

MOVEMENT OF MOTORS CONFIGURATION:
CLOCKWISE = RIGHT DIRECTION
COUNTER-CLOCKWISE = LEFT DIRECTION

*/

#include <LiquidCrystal_I2C.h> // Library for LCD

// Motor A
const int motorAPin1  = 9; 
const int motorAPin2  = 6; 
// Motor B
const int motorBPin1  = 5; 
const int motorBPin2  = 3;  

// Proximity Sensor
const int ProximitySensor = 10;

// IR Sensors
const int IR1 = 11;
const int IR2 = 12;
const int IR3 = 13;

// I2C address 0x27, 16 column and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  // initialize serial comms
  Serial.begin(9600);
  lcd.init(); // initialize the lcd
  lcd.backlight(); // open lcd backlight

 // Setup of PinModes for Motors, IR's, and Proximity Sensor
  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(motorBPin1, OUTPUT);
  pinMode(motorBPin2, OUTPUT);
  pinMode(ProximitySensor, INPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);

}

void loop() {
  // Initialize and read IR Sensor State variables
  int IR1State = digitalRead(IR1); 
  int IR2State = digitalRead(IR2); 
  int IR3State = digitalRead(IR3); 

  // Print IR Sensor states for testing 
  Serial.println(IR1State);
  Serial.println(IR2State);
  Serial.println(IR3State);

  // Check if IR1State is 0 then Trash has been Inserted and will proceed into segregation
  while (IR1State == 0){
    // Show Message that Trash has been Inserted
    ShowTrashInsertedMessage();
    // initialize and read Proximity Sensor state variable
    int ProximitySensorState = digitalRead(ProximitySensor);
    // Print Proximity Sensor state for testing
    Serial.println(ProximitySensorState);
    
    // Check if Proximity Sensor not sensing anything meaning transparent pet bottle has been inserted 
    if (ProximitySensorState == 1){
      // Show message that pet bottle has been inserted
      ShowPetBottleHasBeenDetected();

      int IR2State = digitalRead(IR2);
      while (IR2State == 0){
        ShowTrashcanForBottlesAreFull();
        int IR2State = digitalRead(IR2);
        if (IR2State == 1){
          break;
        }
      }

      // Show Segragating message in the LCD
      ShowSegregatingMessage();
      // Segregate the pet bottles by moving the Motors in Counter-Clockwise Direction (LEFT SIDE TRASHBIN)
      SegregatePetBottles();
      // Stop the Motors from moving.
      StopSegratation();
      // Show Segregation has been completed in the LCD
      ShowSegregationCompletedMessage();
      // Exit the while loop;
      break;
    }else{ // Proximity Sensor is sensing data meaning random trash has been inserted
      // Show message that pet bottle has been inserted
      ShowRandomTrashHasbeenDetected();

      int IR3State = digitalRead(IR3);
      if (IR3State == 0){
        ShowTrashcanForRandomTrashAreFull();
        int IR3State = digitalRead(IR3);
        if (IR3State == 1){
          break;
        }
      }

      // Show Segragating message in the LCD
      ShowSegregatingMessage();
      // Segregate the random by moving the Motors in Clockwise Direction (RIGHT SIDE TRASHBIN)
      SegregateRandomTrash();
      // Stop the Motors from moving.
      StopSegratation();
      // Show Segregation has been completed in the LCD
      ShowSegregationCompletedMessage();
      // Exit the while loop;
      break;
    }
  }

  // // Check if Trash Bins are full or not
  // if (IR2State == 0 && IR3State != 0 ){ // If Trashbin for Pet bottles are full and Random Trash is not full (LEFT TRASHBIN)
  //   // Show Pet Bottles Trash Bin are full in LCD
  //   ShowTrashcanForBottlesAreFull();
  // }else if (IR3State == 0 && IR2State != 0 ){ // If Trashbin for random trash are full and Pet bottles is not full (RIGHT TRASHBIN)
  //   // Show Random Trash Trash Bin are full in LCD
  //   ShowTrashcanForRandomTrashAreFull();
  // }else 
  if (IR2State == 0 && IR3State == 0 ) { // If both trash bin for random trash and pet bottles are full. (BOTH TRASHBIN)
    // Show Both Trashbins are full in the LCD
    ShowBothTrashcanIsFull();
  }else{
    // Show Default Message of LCD ("AUTOMATED BOTTLE SEGRAGATOR" and "Please Insert Trash" Message)
    ShowDefaultLCDMessage();
  }

}

// void function for Showing Default Message in the LCD
void ShowDefaultLCDMessage(){
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("AUTOMATED BOTTLE"); 
  lcd.setCursor(3, 1); 
  lcd.print("SEGREGATOR"); 
  delay(2000);

  lcd.clear();                 
  lcd.setCursor(1, 0);         
  lcd.print("Please insert");      
  lcd.setCursor(5, 1);        
  lcd.print("Trash!"); 
  delay(2000); 
}

// void function for Showing Pet Bottle Trash bin is full Message in the LCD
void ShowTrashcanForBottlesAreFull(){
  lcd.clear();
  lcd.setCursor(3, 0);         
  lcd.print("Pet bottle");      
  lcd.setCursor(2, 1);        
  lcd.print("bin is full"); 
  delay(1000);
}

// void function for Showing Random Trash Trashbin is full Message in the LCD
void ShowTrashcanForRandomTrashAreFull(){
  lcd.clear();
  lcd.setCursor(2, 0);         
  lcd.print("Random trash");      
  lcd.setCursor(2, 1);        
  lcd.print("bin is full"); 
  delay(1000);
}

// void function for Showing Both Trashbins are full Message in the LCD
void ShowBothTrashcanIsFull(){
  lcd.clear();
  lcd.setCursor(3, 0);         
  lcd.print("Both Trash");      
  lcd.setCursor(2, 1);        
  lcd.print("bin is full"); 
  delay(1000);
}

// void function for showing trash has been inserted Message in the LCD
void ShowTrashInsertedMessage(){
  
  lcd.clear();
  lcd.setCursor(1, 0);         
  lcd.print("Trash Inserted");
  delay(5000);
}

// void function for showing pet bottle has been detected Message in the LCD
void ShowPetBottleHasBeenDetected(){
  lcd.clear();
  lcd.setCursor(3, 0);         
  lcd.print("Pet bottle");      
  lcd.setCursor(2, 1);        
  lcd.print("is detected"); 
  delay(2000);
}

// void function for showing random trash has been detected Message in the LCD
void ShowRandomTrashHasbeenDetected(){
  lcd.clear();
  lcd.setCursor(2, 0);         
  lcd.print("Random trash");      
  lcd.setCursor(2, 1);        
  lcd.print("is detected"); 
  delay(2000);
}

// void function for showing Segragating Message in the LCD
void ShowSegregatingMessage(){
  lcd.clear();
  lcd.setCursor(0, 0);         
  lcd.print("Segregating.....");
  delay(3000);
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

// void function that will move both motors in the clockwise direction (RIGHT TRASHBIN)
void SegregateRandomTrash(){
  //This code  will turn Motor A and B clockwise for 5 sec.
  analogWrite(motorAPin1, 255);
  analogWrite(motorAPin2, 0);
  analogWrite(motorBPin1, 255);
  analogWrite(motorBPin2, 0);
  delay(5000); 
}

// void function that will move both motors in the counter-clockwise direction (LEFT TRASHBIN)
void SegregatePetBottles(){
  //This code will turn Motor A and B counter-clockwise for 5 sec.
  analogWrite(motorAPin1, 0);
  analogWrite(motorAPin2, 255);
  analogWrite(motorBPin1, 0);
  analogWrite(motorBPin2, 255);
  delay(5000);
}

// void function that will stop both motors after moving in counter-clockwise or clockwise direction.
void StopSegratation(){
  // This code will stop motor A and Motor B
  analogWrite(motorAPin1, 0);
  analogWrite(motorAPin2, 0);
  analogWrite(motorBPin1, 0);
  analogWrite(motorBPin2, 0);
}
