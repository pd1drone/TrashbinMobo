
// IR Sensors
const int IR1 = 11;

void setup() {
  // initialize serial comms
  Serial.begin(9600);
  pinMode(IR1, INPUT); // ir sensor = 11

}

void loop(){
    int IR1State = digitalRead(IR1); 
    Serial.println(IR1State);
}