
// Bitcoin Ticker with smiley/sad face.

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal.h>


const int rs = D4, en = D3, d4 = D8, d5 = D7, d6 = D6, d7 = D5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//const int rs = D7, en = D6, d4 = D5, d5 = D4, d6 = D3, d7 = D2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int previousValue = 0;
int threshold = 1;
int lastKnown = 0;

// WiFi settings
const char* ssid     = "xxxxxxx";
const char* password = "xxxxxxxxxxx";

// API server
const char* host = "api.coindesk.com";



void setup() {

  // Serial
  Serial.begin(115200);
  delay(10);

 
                                       // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


}
     
void loop() {

                                     // Connect to API
  Serial.print("connecting to ");
  Serial.println(host);
  
                                           // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
                                           // We now create a URI for the request
  String url = "/v1/bpi/currentprice.json";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
                                             // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(100);
  
                                             // Read all the lines of the reply from server and print them to Serial
  String answer;
  while(client.available()){
    String line = client.readStringUntil('\r');
    answer += line;
  }

   client.stop();
   Serial.println();
   Serial.println("closing connection");

  // Process answer
  Serial.println();
  Serial.println("Answer: ");
  Serial.println(answer);

  // Convert to JSON
  String jsonAnswer;
  int jsonIndex;

  for (int i = 0; i < answer.length(); i++) {
    if (answer[i] == '{') {
      jsonIndex = i;
      break;
    }
  }

  // Get JSON data
  jsonAnswer = answer.substring(jsonIndex);
  Serial.println();
  Serial.println("JSON answer: ");
  Serial.println(jsonAnswer);
  jsonAnswer.trim();

  // Get rate as float
  int rateIndex = jsonAnswer.indexOf("rate_float");
  String priceString = jsonAnswer.substring(rateIndex + 12, rateIndex + 18);
  priceString.trim();
  int price = priceString.toFloat();

  // Print price
  Serial.println();
  Serial.println("Bitcoin price: ");
  Serial.println(price);

  lcd.begin(16, 2);                 // Initialize 16x2 LCD Display
  lcd.clear();
 
  lcd.setCursor(0, 0);
  lcd.print("Bitcoin!!");
       lcd.setCursor(11, 0);
       lcd.print("=)");
       lcd.setCursor(1, 1);
       lcd.print("$");
       if (price > 5000) {lastKnown = price;}       // Is price greater than 5000, save last price.
    if (price == 0) {lcd.print(lastKnown);}         // Print BTC price
      else lcd.print(lastKnown);
      
 

  int sensorValue = analogRead(A0); //read the A0 pin value
  float voltage = sensorValue * (3.00 / 1023.00) * 2; //convert the value to a true voltage.
  
  lcd.setCursor(11, 1);
  lcd.print(voltage);
  lcd.print("v");
 
  // Wait 5 seconds
  delay(1000);
}
