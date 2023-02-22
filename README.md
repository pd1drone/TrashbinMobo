# TrashbinMobo
Segregate bottles from random trash  
Display the process and the status of the bins on the LCD  
Detects random trash  
Detects clear bottles only  
Uses motor for actuating panels and conveyor belt  
1 at a time trash segregator  



## Steps to Setup TrashbinMobo
1. Clone this repository 
```
git clone https://github.com/pd1drone/TrashbinMobo
```
2. Get the LiquidCrystal_I2C folder and copy it to the folder libraries it is located the arduino codes has been save it will have a libraries folder. The default folder of arduino is in /Documents/Arduino/libraries/
![Libraries-Directory](Libraries-Directory.png)

3. To adjust the duration of movement of the motors go to the TrashbinMobo.ino file Line 252-270, and change the delay value.
```
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
```

4. Configuration of TrashbinMobo:
```
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
```
