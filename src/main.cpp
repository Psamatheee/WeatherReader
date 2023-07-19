#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

#define BLUE_LED 15

const char* ssid     = "networkname";
const char* password = "networkpass";
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
String PIN_LEDState = "OFF";

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
 // while(!Serial);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, HIGH);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(BLUE_LED, LOW);
    delay(500);
    Serial.print(".");
    digitalWrite(BLUE_LED, HIGH);


  WiFi.begin(ssid, password);


  }

  if (client.connect(server, 80)) {
    client.println("GET /en/forecast/rss/3day/2650225 HTTP/1.1");
    client.println("Host: weather-broker-cdn.api.bbci.co.uk");
    client.println();
  }

    if (!i2CAddrTest(0x27)) {
    lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  }
  lcd.init();                     // LCD driver initialization
  lcd.backlight();                // Open the backlight
  lcd.setCursor(0,0);             // Move the cursor to row 0, column 0
  lcd.print(WiFi.localIP());  
}
void loop() {


 // lcd.setCursor(1,0);   

  lcd.clear();
  while (client.available()) {
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

          while(true);
    }
    

    
  }
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    while (true);
  }
}