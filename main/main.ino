#include <math.h>

#include "DHT.h"
#include <LiquidCrystal.h>


const int dht_pin = 2;
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
const int num_row = 16;
const int num_col = 2;

DHT dht(dht_pin, DHT11);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(num_row, num_col);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();  // Celsius

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  lcd.setCursor(0, 0);  // Reset cursor to top of screen

  lcd.print(round(t));
  lcd.print("C ");
  lcd.print(round(h));
  lcd.print("% humidity");
}