#include <SoftwareSerial.h>

const int RED1_PIN = 11;
const int GREEN1_PIN = 3;

const int RED2_PIN = 6;
const int GREEN2_PIN = 5;

const int TRIG1_PIN = 9;
const int ECHO1_PIN = 10;

const int TRIG2_PIN = A0;
const int ECHO2_PIN = A1;

const int TX1_PIN = 7;
const int RX1_PIN = 8;

const int TX2_PIN = 12;
const int RX2_PIN = 13;

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

  pinMode(RX1_PIN, INPUT);
  pinMode(TX1_PIN, OUTPUT);

  pinMode(RX2_PIN, INPUT);
  pinMode(TX2_PIN, OUTPUT);

  Serial.begin(9600);
}

String readFromBT1() {
  String sentence = "";
  char c;

  SoftwareSerial BTSerial(RX1_PIN, TX1_PIN);
  BTSerial.begin(38400);

  while(!BTSerial.available()) {}

  delay(50);

  while(BTSerial.available()) {
    c = BTSerial.read();
    sentence += c;
  }

  return sentence;
}

void writeFromBT2(String sentence) {
  SoftwareSerial BTSerial(RX2_PIN, TX2_PIN);
  BTSerial.begin(38400);

  BTSerial.println(sentence);
}

void loop() {
  String message = "";
  message = readFromBT1();
  if(message != ""){
    getDistance(TRIG1_PIN, ECHO1_PIN);
    if(distance1 < 5 && distance1 > 0) {
      turnOnLED(RED1_PIN);
      turnOffLED(GREEN1_PIN);
      state[0] = '1';
    } else {
      turnOnLED(GREEN1_PIN);
      turnOffLED(RED1_PIN);
      state[0] = '0';
    }

    getDistance(TRIG2_PIN, ECHO2_PIN);
    if(distance2 < 5 && distance2 > 0) {
      turnOnLED(RED2_PIN);
      turnOffLED(GREEN2_PIN);
      state[1] = '1';
    } else {
      turnOnLED(GREEN2_PIN);
      turnOffLED(RED2_PIN);
      state[1] = '0';
    }
    Serial.println(state);
    Serial.println(message + state);
    writeFromBT2(message + state);
  }
}
