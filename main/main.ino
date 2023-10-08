#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const int RED_PIN = 5;
const int GREEN_PIN = 6;

const int SERVO_PIN = 3;

const int RX1_PIN = 10;
const int TX1_PIN = 11;

const int RX2_PIN = A1;
const int TX2_PIN = A0;

String state;

int oc[4] = {0};
int carsInside;
int carsToday = 0;

unsigned long ocMillis[4];
unsigned long updateTime, stateTime;

Servo myservo;

LiquidCrystal_I2C lcd(0x20, 16, 2); //0x20

String readFromBT1() {
  String sentence = "";
  char c;

  SoftwareSerial BTSerial(RX1_PIN, TX1_PIN);
  BTSerial.begin(38400);

  while(!BTSerial.available());

  delay(50);

  while(BTSerial.available()) {
    c = BTSerial.read();
    sentence += c;
  }

  return sentence;
}

String updateLCD() {
  state = readFromBT1();

  carsInside = 0;
  for(int i = 0; i < 4; i++) {
    if(oc[i] == 1 || state[i] == '1') {
      carsInside++;
    }
  }
  writeMessageLCD("Free slots: " + String(4 - carsInside), "Cars today: " + String(carsToday));
  updateTime = millis();

  Serial.println("Free slots: " + String(4 - carsInside));
  Serial.println("Cars today: " + String(carsToday));

  return readFromBT2();
}

String readFromBT2() {
  String sentence = "";
  char c;

  SoftwareSerial BTSerial(RX2_PIN, TX2_PIN);
  BTSerial.begin(38400);

  while(!BTSerial.available()) {  
    if(millis() - updateTime > 60000) {
      return updateLCD();
    }
  }

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

  BTSerial.print(sentence);
}

void turnOnLED(int LED_PIN) {
  digitalWrite(LED_PIN, HIGH);
}

void turnOffLED(int LED_PIN) {
  digitalWrite(LED_PIN, LOW);
}

void close() {
  myservo.write(90);
}

void open() {
  myservo.write(0);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  pinMode(RX1_PIN, INPUT);
  pinMode(TX1_PIN, OUTPUT);

  pinMode(RX2_PIN, INPUT);
  pinMode(TX2_PIN, OUTPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.clear();
  lcd.backlight();
  updateLCD();

  turnOnLED(RED_PIN);
  myservo.attach(SERVO_PIN);
  close();
}

void writeMessageLCD(String message, String message2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  lcd.setCursor(0, 1);
  lcd.print(message2);
}

void loop() {
  String message = "";
  message = readFromBT2();

  if(message != "") {
    state = readFromBT1();
    int i;
    bool placeGiven = false;
    for(i = 0; state[i] != '\0'; i++) {
      if(state[i] == '0' && oc[i] == 0) {
        writeFromBT2("Place " + String(i + 1) + " is waiting for you.\n");
        open();
        turnOffLED(RED_PIN);
        turnOnLED(GREEN_PIN);
        delay(3000);
        for(int j = 0; j < 10; j++) {
          turnOnLED(GREEN_PIN);
          delay(100);
          turnOffLED(GREEN_PIN);
          delay(100);
        }
        close();
        turnOnLED(RED_PIN);
        oc[i] = 1;
        ocMillis[i] = millis();
        placeGiven = true;
        carsToday++;
        break;
      }
    }
    if(placeGiven == false) {
      writeFromBT2("No more places.\n");
    }
  }

  carsInside = 0;
  for(int i = 0; i < 4; i++) {
    if(oc[i] == 1 && millis() - ocMillis[i] > 30000) {
      oc[i] = 0;
    }

    if(oc[i] == 1 || state[i] == '1') {
      carsInside++;
    }
  }

  writeMessageLCD("Free slots: " + String(4 - carsInside), "Cars today: " + String(carsToday));
}