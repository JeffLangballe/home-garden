#include <math.h>

#include "DHT.h"
#include <IRremote.h>
#include <LiquidCrystal.h>


const int dht_pin = 2;
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
const int num_row = 16;
const int num_col = 2;

typedef enum ui_mode {
  UI_STATUS = 0,
  UI_MANUAL_WATER,
  UI_MANUAL_HEAT,
  UI_INTERVAL_SET,
  UI_COUNT,
};

ui_mode ui = UI_STATUS;

typedef enum button {
  BTN_NONE,
  BTN_OK,
  BTN_NOK,
  BTN_NEXT,
  BTN_PREV,
  BTN_UP,
  BTN_DOWN,
};

button last_pressed = BTN_NONE;

float humidity = 0;
float temperature = 0;

DHT dht(dht_pin, DHT11);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int ir_rcv_pin = 13; // the pin where you connect the output pin of IR sensor     
IRrecv irrecv(ir_rcv_pin);     
decode_results results;   


void updateLastPressed() {
  switch(results.value) {
    case 0xFFA25D: Serial.println("POWER"); break;
    case 0xFFE21D: Serial.println("FUNC/STOP"); last_pressed = BTN_NOK; break;
    case 0xFF629D: Serial.println("VOL+"); break;
    case 0xFF22DD: Serial.println("FAST BACK"); last_pressed = BTN_PREV; break;
    case 0xFF02FD: Serial.println("PAUSE"); last_pressed = BTN_OK;  break;
    case 0xFFC23D: Serial.println("FAST FORWARD"); last_pressed = BTN_NEXT; break;
    case 0xFFE01F: Serial.println("DOWN"); last_pressed = BTN_DOWN; break;
    case 0xFFA857: Serial.println("VOL-"); break;
    case 0xFF906F: Serial.println("UP"); last_pressed = BTN_UP; break;
    case 0xFF9867: Serial.println("EQ"); break;
    case 0xFFB04F: Serial.println("ST/REPT"); break;
    case 0xFF6897: Serial.println("0"); break;
    case 0xFF30CF: Serial.println("1"); break;
    case 0xFF18E7: Serial.println("2"); break;
    case 0xFF7A85: Serial.println("3"); break;
    case 0xFF10EF: Serial.println("4"); break;
    case 0xFF38C7: Serial.println("5"); break;
    case 0xFF5AA5: Serial.println("6"); break;
    case 0xFF42BD: Serial.println("7"); break;
    case 0xFF4AB5: Serial.println("8"); break;
    case 0xFF52AD: Serial.println("9"); break;
    case 0xFFFFFFFF: Serial.println(" REPEAT"); break;  

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

  float h = dht.readHumidity();
  float t = dht.readTemperature();  // Celsius
  if (!isnan(h)) humidity = h;
  if (!isnan(t)) temperature = t;

  // Switch screens 
  int next_ui = (int)ui;
  if (last_pressed == BTN_NEXT) {
    next_ui++;
    next_ui %= UI_COUNT;
  } else if (last_pressed == BTN_PREV) {
    if (ui == (ui_mode)0) {
      next_ui = (int)UI_COUNT - 1;
    } else {
      next_ui--;
    }
  }
  ui = (ui_mode)next_ui;

  // Display UI
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("<              >");
  lcd.setCursor(0,0);
  switch (ui) {
  case UI_STATUS:
    lcd.print(round(t));
    lcd.print("C ");
    lcd.print(round(h));
    lcd.print("%");
    break;

  case UI_MANUAL_WATER:
    lcd.print("<M_WATER>");
    break;
  case UI_MANUAL_HEAT:
    lcd.print("M_HEAT");
    break;
  case UI_INTERVAL_SET:
    lcd.print("M_INTERVAL");
    break;
  default:
    break;
  }

  // Read input and wait
  if (irrecv.decode(&results)) {
    updateLastPressed(); 
    delay(500);                 // Do not get immediate repeat
    irrecv.resume();            // receive the next value
  } else {
    last_pressed = BTN_NONE;
    delay(500);
  }
}