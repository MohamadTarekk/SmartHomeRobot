//Flame sensor
#define flamePin 2
#define buzzerPin 3

//Light Sensor
#define lightPin A5
#define LEDshield 6
#define lightLimit 400  //maximum analog value read from the light sensor (value increase -> darkness increase)

//Ultrasonic Sensor
#define echoPin 4
#define trigPin 5
#define minDistance 20  //minimum distance to obstacle before maneuvering (in centimeter)
double duration;
float distance;

//Mobile Control
char carDirectionBT;

//Motors
#define right1 10
#define right2 11
#define left1 12
#define left2 13

//Robot Modes
#define STOP '0'
#define AUTOPILOT '5'
#define SOS '6'
#define MOBILE '7'

char carMode = '0';
char temp;

//Direction
#define UP '1'
#define RIGHT '2'
#define LEFT '3'
#define DOWN '4'

//Serial Counter
int counter = 0;

void setup() {
  Serial.begin(9600);
  
  //Flame sensor
  pinMode(flamePin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  //Light sensor
  pinMode(LEDshield, OUTPUT);
  
  //Ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Motors
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
}

void loop() {
  checkForFlame();
  checkLighting();
  delay(250);
  if(Serial.available() > 0)
  {
    temp = Serial.read();
  }

  if(temp >= UP && temp <= DOWN)
  {
    carDirectionBT = temp;
  }
  else if(temp >= AUTOPILOT && temp <= MOBILE || temp == STOP)
  {
    carMode = temp;
  }
  
  //Link the motor with the current control unit (autopilot - IR remote - mobile phone)
  switch (carMode){
    case STOP:
      stopMoving();
      break;
    case AUTOPILOT:
      runAutopilot();
      break;
    case SOS:
      sos();
      break;
    case MOBILE:
      mobilePhoneControl(temp);
      break;
  }
  counter++;
  if(counter%10 == 0) counter = 0;
}

void checkForFlame(){
  int fire = digitalRead(flamePin);// read FLAME sensor
  if( fire == HIGH){
    if(counter%10 == 0) Serial.println("No Fire.");
    digitalWrite(buzzerPin,LOW);// set the buzzer OFF
  }else{
    if(counter%10 == 0) Serial.println("Fire!");
    digitalWrite(buzzerPin,HIGH); // Set the buzzer ON
  }
}

void checkLighting(){
  float lightLevel = analogRead(lightPin);
  if(lightLevel > lightLimit){
    if(counter%10 == 0) Serial.println("Light on.");
    digitalWrite(LEDshield, LOW);
  } else {
    if(counter%10 == 0) Serial.println("Light off.");
    digitalWrite(LEDshield, HIGH);
  }
}

void runAutopilot(){
  if(counter%10 == 0) Serial.println("Autopilot");
  
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); // delays are required for a succesful sensor operation.
  
  delayMicroseconds(10); //this delay is required as well!
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  //Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  //Calculating the distance
  distance= duration*0.034/2;
  if (distance < 25)/*if there's an obstacle 25 centimers, ahead, do the following: */ {   
    moveRight();
  }
  else 
  {
    moveForward();
    delay(15);
  }
}

void sos() 
{
  if(counter%10 == 0) Serial.println("SOS");
  stopMoving();
  flash(200); flash(200); flash(200); // S
  delay(300);
  flash(500); flash(500); flash(500); // O
  delay(300);
  flash(200); flash(200); flash(200); // S
  delay(1000);
}

void flash(int duration)
{
  digitalWrite(LEDshield,LOW);
  digitalWrite(buzzerPin, HIGH);
  delay(duration);
  digitalWrite(LEDshield,HIGH);
  digitalWrite(buzzerPin, LOW);
  delay(duration);
}

void mobilePhoneControl(char carDirectionBT){
  if(counter%10 == 0) Serial.println("Mobile Controlled");
  
  //Link the motor with the current control unit (autopilot - IR remote - mobile phone)
  switch (carDirectionBT){
    case UP:
          moveForward();
          break;
    case RIGHT:
          moveRight();
          break;
    case LEFT:
          moveLeft();
          break;
    case DOWN:
          moveBackward();
          break;
  }
}


void moveForward(){
  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);                       
}

void moveBackward(){
  digitalWrite(right1, LOW);
  digitalWrite(right2, HIGH);
  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);                       
}

void moveRight(){
  digitalWrite(right1, LOW);
  digitalWrite(right2, HIGH);
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);                       
}

void moveLeft(){
  digitalWrite(right1, HIGH);
  digitalWrite(right2, LOW);
  digitalWrite(left1, LOW);
  digitalWrite(left2, HIGH);                       
}

void stopMoving(){
  digitalWrite(right1, LOW);
  digitalWrite(right2, LOW);
  digitalWrite(left1, LOW);
  digitalWrite(left2, LOW);
}
