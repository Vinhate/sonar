#include <NewPing.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define trigPin 9
#define echoPin 8
#define distance 200
#define btnPin 12
boolean lastBtn;
boolean currentBtn;
boolean switchBtn;
NewPing sonar(trigPin, echoPin, distance);
LiquidCrystal_I2C lcd(0x3f, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
  pinMode(btnPin, INPUT_PULLUP);
}

boolean debounce(boolean last) {
  unsigned long int lastTime;
  boolean current = digitalRead(btnPin);
  if (last != current) {
    if (millis() - lastTime > 5) {
      lastTime = millis();
      current = digitalRead(btnPin);
    }
  }
  return current;
}

void loop() {

  currentBtn = debounce(lastBtn);
  if (lastBtn == LOW && currentBtn == HIGH) {
    if (switchBtn) switchBtn = false;
    else switchBtn = true;
  }
  lastBtn = currentBtn;

  int cm;
  int in;
  unsigned long int lastTime;
  if (millis() - lastTime > 1000) {
    lastTime = millis();
    cm = sonar.ping_cm();
    in = sonar.ping_in();
  }

  if (switchBtn) {
    unsigned long int currentTime;
    if (millis() - currentTime > 1000) {
      currentTime = millis();
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(cm);
    }
    lcd.print(" cm");
  } else {
    unsigned long int currentTime;
    if (millis() - currentTime > 1000) {
      currentTime = millis();
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print(in);
    }
    lcd.print(" inch");
  }

}
