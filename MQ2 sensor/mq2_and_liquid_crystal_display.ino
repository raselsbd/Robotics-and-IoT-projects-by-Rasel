#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD I2C address 0x27 or 0x3F

int mq2Pin = A0;   // MQ2 analog output connected to A0
int gasValue = 0;
int gasPercent = 0;

void setup() {
  lcd.begin(16, 2);          // Initialize LCD
  lcd.backlight();     // Turn on LCD backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas Detector");
  delay(2000);
  lcd.clear();
}

void loop() {
  gasValue = analogRead(mq2Pin);               // Read sensor value (0–1023)
  gasPercent = map(gasValue, 0, 1023, 0, 100); // Convert to percentage (0–100%)

  lcd.setCursor(0, 0);
  lcd.print("Gas Level: ");
  lcd.print(gasPercent);
  lcd.print("%  "); // spaces clear old digits

  lcd.setCursor(0, 1);
  if (gasPercent > 60) {
    lcd.print("!!! GAS ALERT !!!");
  } else if (gasPercent > 30) {
    lcd.print("Moderate Level  ");
  } else {
    lcd.print("Safe Level      ");
  }

  delay(500);
}
