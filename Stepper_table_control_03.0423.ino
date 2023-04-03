/*
Version 2
change log:
  - Homing was renamed to homed and changed from int to bool
  - commented
  - new params for encoders added
  - pins for direction and steps were changed to accomodate for the addition of encoder
  - pin for homing sensor (pos) was changed to intrupt pin to accomodate for intrupt homing if needed
*/

// Initialising pin
// Stepper pins and parameters
const int en=11;    // enable Pin (not in use?)
const int stp=9;    // Step Pin
const int dir=8;    // Direction Pin
int ptn;            // Current positions
int angleToRotate;  // The desired angle to rotate
int steps;          // number of steps 
int dt = 10000;      // delay time in us

// Homing parameters
const int pos=2;    // Hall effect homing sensor for homing the turntable
bool Homed = false;    // Homing status 
int homedt = 30;

// placement variables 
int angleTest = 0;
int posVal;
volatile int delayTime;   // delay time will be removed when encoder is added to close the loop.
const int enLED=13;


void setup() {
  Serial.begin(115200);
  // Defining pins
  pinMode(en, OUTPUT);
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(pos, INPUT);
  pinMode(enLED, OUTPUT);
  digitalWrite(en, LOW);
  digitalWrite(enLED, HIGH);
  Serial.println("I am good");
}

void loop() {
  // Start of the homing sequence if needed
  if (Homed == false){
  posVal= digitalRead(pos);
    while (posVal == 0){
      posVal= digitalRead(pos);
      Serial.println(posVal);
      Serial.println("I am here loop");
      digitalWrite(stp, HIGH);
      delay(homedt);
      digitalWrite(stp, LOW);
      delay(homedt);
      ptn = 0;
    }
    Homed = true;
    Serial.println("I am here outloop");
  } 

  // Start of the main sequence for position control
  else{
    while(true){
        // Reading the analog input voltage for desired position
        int bits = analogRead(A0);

        // Converting input voltage to angle
        int angle = int(bits/93)*30;
        int valInt = angle;

        if (valInt == 0 && ptn <=180){
          valInt = 360;
        }
        if (valInt == 0 && ptn >180){
          valInt = -360;
        }
        
        
        if (valInt>0 && valInt<=360){
          if (valInt!=ptn){
            angleToRotate = (valInt-ptn);
            ptn = valInt;
          }
           else{
            angleToRotate = 0;
          }            
          if (angleToRotate>180){
            angleToRotate = angleToRotate-360;
          }
          if (angleToRotate<-180){
            angleToRotate = 360+angleToRotate;
          }
          steps = angleToRotate*1.1;
          if (angleToRotate>0){
            digitalWrite(dir, LOW);
            for(int i =0; i<=steps;i++){
              digitalWrite(stp, HIGH);
              delayMicroseconds(dt);
              digitalWrite(stp, LOW);
              delayMicroseconds(dt);
            }
          }
          if (angleToRotate<0){
            digitalWrite(dir, HIGH);
            for(int i =0; i<=abs(steps);i++){
              digitalWrite(stp, HIGH);
              delayMicroseconds(dt);
              digitalWrite(stp, LOW);
              delayMicroseconds(dt);
            }
          }
          
          // Delay period to be replaced by the encoder output
          delayTime = abs(angleToRotate)*10;
          delay(delayTime);
          
          Serial.println("done");
          if (ptn==360){
            ptn=0;
          }
          Serial.println(ptn); 
        }
        if (valInt<0 && valInt>=-360){
          valInt = 360 + valInt;
          if (valInt!=ptn){
            angleToRotate = 360-(valInt-ptn);
            ptn = valInt;
          }
           else{
            angleToRotate = 0;
          }
          if (angleToRotate>360){
            angleToRotate = -720+angleToRotate;
          }
          
          if (angleToRotate>180){
            angleToRotate = angleToRotate-360;
          }
          if (angleToRotate<-180){
            angleToRotate = 360+angleToRotate;
          }
          steps = angleToRotate*1.1;
          if (angleToRotate>0){
            digitalWrite(dir, HIGH);
            for(int i =0; i<=abs(steps);i++){
              digitalWrite(stp, HIGH);
              delayMicroseconds(dt);
              digitalWrite(stp, LOW);
              delayMicroseconds(dt);
            }
          }
          if (angleToRotate<0){
            digitalWrite(dir, LOW);
            for(int i =0; i<=abs(steps);i++){
              digitalWrite(stp, HIGH);
              delayMicroseconds(dt);
              digitalWrite(stp, LOW);
              delayMicroseconds(dt);
            }
          }
          delayTime = abs(angleToRotate)*10;
          delay(delayTime);
          
          Serial.println("done");
          Serial.println(ptn);
        }
      }
   }
  }
