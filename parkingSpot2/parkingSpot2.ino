#include <SoftwareSerial.h>

const int RED1_PIN = 11;
const int GREEN1_PIN = 3;

const int RED2_PIN = 6;
const int GREEN2_PIN = 5;

const int TRIG1_PIN = 9;
const int ECHO1_PIN = 10;

const int TRIG2_PIN = A0;
const int ECHO2_PIN = A1;

const int RX_PIN = A2;
const int TX_PIN = A3;

int distance1;
int distance2;

char state[2] = "00";

int getDistance(int TRIG_PIN, int ECHO_PIN) {
  int duration;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  if(TRIG_PIN == 9) {
    distance1 = duration * 0.034 / 2;
    return distance1;
  } else {
    distance2 = duration * 0.034 / 2;
    return distance2;
  }
}

void turnOnLED(int LED_PIN) {
  digitalWrite(LED_PIN, HIGH);
}

void turnOffLED(int LED_PIN) {
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  pinMode(RED1_PIN, OUTPUT);
  pinMode(GREEN1_PIN, OUTPUT);

  pinMode(RED2_PIN, OUTPUT);
  pinMode(GREEN2_PIN, OUTPUT);

  pinMode(TRIG1_PIN, OUTPUT);
  pinMode(ECHO1_PIN, INPUT);

  pinMode(TRIG2_PIN, OUTPUT);
  pinMode(ECHO2_PIN, INPUT);

  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);

  Serial.begin(9600);
}

void writeFromBT(String sentence) {
  SoftwareSerial BTSerial(RX_PIN, TX_PIN);
  BTSerial.begin(38400);

  BTSerial.print(sentence);
}

void loop() {
  getDistance(TRIG1_PIN, ECHO1_PIN);

  Serial.println("Dist1=" + String(distance1));
  if(distance1 < 10 && distance1 > 0) {
    turnOnLED(RED1_PIN);
    turnOffLED(GREEN1_PIN);
    state[0] = '1';
  } else {
    turnOnLED(GREEN1_PIN);
    turnOffLED(RED1_PIN);
    state[0] = '0';
  }

  delay(500);

  getDistance(TRIG2_PIN, ECHO2_PIN);
  Serial.println("Dist2=" + String(distance2));
  if(distance2 < 10 && distance2 > 0) {
    turnOnLED(RED2_PIN);
    turnOffLED(GREEN2_PIN);
    state[1] = '1';
  } else {
    turnOnLED(GREEN2_PIN);
    turnOffLED(RED2_PIN);
    state[1] = '0';
  }

  writeFromBT(state);
  delay(500);
}
