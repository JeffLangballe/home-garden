#include <math.h>

#include "DHT.h"
#include <IRremote.h>
#include <LiquidCrystal.h>


const int dht_pin = 2;
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
const int num_row = 16;
const int num_col = 2;

DHT dht(dht_pin, DHT11);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ir_rcv_pin = 13; // the pin where you connect the output pin of IR sensor     
IRrecv irrecv(ir_rcv_pin);     
decode_results results;   


void translateIR() {
  switch(results.value) {
    case 0xFFA25D: Serial.println("POWER"); break;
    case 0xFFE21D: Serial.println("FUNC/STOP"); break;
    case 0xFF629D: Serial.println("VOL+"); break;
    case 0xFF22DD: Serial.println("FAST BACK");    break;
    case 0xFF02FD: Serial.println("PAUSE");    break;
    case 0xFFC23D: Serial.println("FAST FORWARD");   break;
    case 0xFFE01F: Serial.println("DOWN");    break;
    case 0xFFA857: Serial.println("VOL-");    break;
    case 0xFF906F: Serial.println("UP");    break;
    case 0xFF9867: Serial.println("EQ");    break;
    case 0xFFB04F: Serial.println("ST/REPT");    break;
    case 0xFF6897: Serial.println("0");    break;
    case 0xFF30CF: Serial.println("1");    break;
    case 0xFF18E7: Serial.println("2");    break;
    case 0xFF7A85: Serial.println("3");    break;
    case 0xFF10EF: Serial.println("4");    break;
    case 0xFF38C7: Serial.println("5");    break;
    case 0xFF5AA5: Serial.println("6");    break;
    case 0xFF42BD: Serial.println("7");    break;
    case 0xFF4AB5: Serial.println("8");    break;
    case 0xFF52AD: Serial.println("9");    break;
    case 0xFFFFFFFF: Serial.println(" REPEAT");break;  

  default: 
    Serial.print(" other button   ");
    Serial.println(results.value);

  }
}


void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(num_row, num_col);
  irrecv.enableIRIn();
}

void loop() {
  // // Wait a few seconds between measurements.
  // delay(2000);

  // // Reading temperature or humidity takes about 250 milliseconds!
  // // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // float h = dht.readHumidity();
  // float t = dht.readTemperature();  // Celsius

  // // Check if any reads failed and exit early (to try again).
  // if (isnan(h) || isnan(t)) {
  //   Serial.println(F("Failed to read from DHT sensor!"));
  //   return;
  // }

  // lcd.setCursor(0, 0);  // Reset cursor to top of screen

  // lcd.print(round(t));
  // lcd.print("C ");
  // lcd.print(round(h));
  // lcd.print("% humidity");

  // TODO: Ordering of things
  if (irrecv.decode(&results))   // have we received an IR signal?
  {
    translateIR(); 
    delay(500);                 // Do not get immediate repeat
    irrecv.resume();            // receive the next value
  }  
}