#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

#define BLUE_LED 15
#define PIN_SWITCH 13

const char* ssid     = "";
const char* password = "";
char server[] = "weather-broker-cdn.api.bbci.co.uk";
char w[] = "onight";
LiquidCrystal_I2C lcd(0x27,16,2); 

bool i2CAddrTest(uint8_t addr) {
  Wire.begin();
  Wire.beginTransmission(addr);
  if (Wire.endTransmission() == 0) {
    return true;
  }
  return false;
}

WiFiClient client;
String header;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  pinMode(BLUE_LED, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_SWITCH, INPUT);
  digitalWrite(PIN_LED, HIGH);
  
  if (!i2CAddrTest(0x27)) {
    lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  }

  lcd.init();                    
  lcd.backlight();               
  lcd.setCursor(0,0);            
  lcd.print("Connecting to Wifi...");  
  
  if(digitalRead(PIN_SWITCH) == LOW){
    lcd.clear();
    lcd.print("Input Wifi password and ssid");  
    digitalWrite(BLUE_LED, HIGH);
    Serial.begin(115200);
    //while(!Serial);
    // Change Wifi details through serial here
  }

  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(500);
  }
  lcd.clear();
  lcd.print("Connected!");  

  if (client.connect(server, 80)) {
    client.println("GET /en/forecast/rss/3day/2650225 HTTP/1.1");
    client.println("Host: weather-broker-cdn.api.bbci.co.uk");
    client.println();
  }
}

void loop() {

  lcd.clear();

 /*while (client.available()) {
    client.readStringUntil('T');
    char tonight_rest[6];
    for(int i=0; i <6; i++){
      tonight_rest[i] = client.read();
    }

    bool reach = true;
    for(int j=0; j<6; j++){
      if(w[j] != tonight_rest[j]){
        reach = false;
      }
    }

    if(reach){
          client.read();
          client.read();
          String rest = client.readStringUntil(',');
          lcd.print(rest);  
          Serial.print(rest);
          lcd.setCursor(0,1);
          client.readStringUntil(':');
          String rest2 = client.readStringUntil('(');
          lcd.print(rest2);  
          Serial.print(rest2);
          lcd.print("aaaa");
          while(true);
    }
  }*/

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    while (true);
  }
}