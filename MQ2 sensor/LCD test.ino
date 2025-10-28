#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.begin(16, 2);    // Init display (for New-LiquidCrystal)
  lcd.setBacklight(HIGH); // Force backlight on (this library uses this function)
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Hello, World!");
  lcd.setCursor(0, 1);
  lcd.print("Rasel IoT");
}

void loop() {}
