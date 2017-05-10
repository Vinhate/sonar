// Подключаем библиотеки
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define btnPin 12
#define ledcm 11
#define ledin 10
#define trigPin 9
#define echoPin 8
#define distance 200
boolean lastBtn;
boolean currentBtn;
boolean switchBtn;
NewPing sonar(trigPin, echoPin, distance); //Инициализируем датчик расстояния HC-SR04
LiquidCrystal_I2C lcd(0x3f, 16, 2); //Инициализируем дисплей

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(ledcm, OUTPUT);
  pinMode(ledin, OUTPUT);
}

//Функция подавления дребезга на кнопке
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

  //Проверяем нажата ли кнопка
  currentBtn = debounce(lastBtn);
  if (lastBtn == LOW && currentBtn == HIGH) {
    if (switchBtn) switchBtn = false;
    else switchBtn = true;
  }
  lastBtn = currentBtn;

  //Считываем даные расстояния с датчика HC-SR04
  int cm;
  int in;
  unsigned long int lastTime;
  if (millis() - lastTime > 1000) {
    lastTime = millis();
    cm = sonar.ping_cm();
    in = sonar.ping_in();
  }

  //Проверяем режим (сантиметры/дюймы) и выводим на дисплей
  if (switchBtn) {
    digitalWrite(ledcm, HIGH);
    digitalWrite(ledin, LOW);
    unsigned long int currentTime;
    if (millis() - currentTime > 1000) {
      currentTime = millis();
      lcd.print("      ");
      lcd.setCursor(0, 0);
      lcd.print(cm);
    }
    lcd.print(" cm");
  } else {
    digitalWrite(ledin, HIGH);
    digitalWrite(ledcm, LOW);
    unsigned long int currentTime;
    if (millis() - currentTime > 1000) {
      currentTime = millis();
      lcd.print("      ");
      lcd.setCursor(0, 0);
      lcd.print(in);
    }
    lcd.print(" inch");
  }
  
}
