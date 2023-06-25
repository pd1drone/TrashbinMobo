

// left ultrasonic sensor for trashcan
const int PanelTrigPin = 6;
const int PanelEchoPin = 4;

long panelUltrasonicDuration;

float panel = 0.0;


void setup() {
  // initialize serial comms
  Serial.begin(9600);
  pinMode(PanelTrigPin, OUTPUT);  // left trig pin = 4
  pinMode(PanelEchoPin, INPUT);  // left echo pin = 6

}

void loop(){
    PanelUltrasonicSensor();
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
