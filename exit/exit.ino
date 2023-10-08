#include <Servo.h>

const int RED_PIN = 5;
const int GREEN_PIN = 6;

const int TRIG1_PIN = A0;
const int ECHO1_PIN = A1;

const int TRIG2_PIN = 9;
const int ECHO2_PIN = 10;

int distance1;
int distance2;

const int SERVO_PIN = 3;

Servo myservo;

void close() {
  myservo.write(90);
}

void open() {
  myservo.write(0);
}

void turnOnLED(int LED_PIN) {
  digitalWrite(LED_PIN, HIGH);
}

void turnOffLED(int LED_PIN) {
  digitalWrite(LED_PIN, LOW);
}

int getDistance(int TRIG_PIN, int ECHO_PIN) {
  int duration;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  if(TRIG_PIN == A0) {
    distance1 = duration * 0.034 / 2;
    return distance1;
  } else {
    distance2 = duration * 0.034 / 2;
    return distance2;
  }
}


void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  pinMode(TRIG1_PIN, OUTPUT);
  pinMode(ECHO1_PIN, INPUT);

  pinMode(TRIG2_PIN, OUTPUT);
  pinMode(ECHO2_PIN, INPUT);

  myservo.attach(SERVO_PIN);

  Serial.begin(9600);

  turnOnLED(RED_PIN);
}

void loop() {
  getDistance(TRIG1_PIN, ECHO1_PIN);
  if(distance1 < 5 && distance1 > 0) {
    open();
    turnOffLED(RED_PIN);
    turnOnLED(GREEN_PIN);
    distance2 = 100;
    while(distance2 > 5) {
      getDistance(TRIG2_PIN, ECHO2_PIN);
      
      delay(10);
    }
    for(int i = 0; i < 10; i++) {
      turnOnLED(GREEN_PIN);
      delay(100);
      turnOffLED(GREEN_PIN);
      delay(100);
    }
    
    turnOnLED(RED_PIN);
    close();
  }

  delay(10);
}
