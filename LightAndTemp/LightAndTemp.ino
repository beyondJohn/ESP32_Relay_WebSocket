
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHTesp.h>

const int  HTTP_PORT   = 80;
const char* HTTP_METHOD = "GET"; // or "POST"
const char* HOST_NAME = "http://switchmagic.com:8000/data"; // hostname of web server:

const char* ssid = "Fios-WVBRA_EXT";
const char* password = "pea4879carve43tony";
//const char* ssid = "Pixel_6944";
//const char* password = "jphotspot";
const char* websocket_server_host = "192.82.251.138";
const uint16_t websocket_server_port = 8888;
const int relay = 27;

const int highSpeed = 255;
const int lowSpeed = 0;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

using namespace websockets;
WebsocketsClient client;
void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
    if(message.data() == "hey hi ho"){
      digitalWrite(relay, LOW);
    }
    else if(message.data() == "on"){
      Serial.println(message.data());
    } 
    else if(message.data() == "off"){
      Serial.println(message.data());
    } 
    else if(message.data() == "just checking"){
      Serial.println(message.data());
    } 
    else{
      //digitalWrite(relay, HIGH);
      Serial.println(message.data());
    }
}

void connect(){
    
  while(!client.connect(websocket_server_host, websocket_server_port, "/")){
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("Websocket Connected!");

}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("ConnectionOpened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("ConnectionClosed");
        delay(1000);
        connect();
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("GotPong");
    }
}

/** Temperature variables*/
DHTesp dht;
int dhtPin = 26;
bool getTemperature();

void initTemp() {
  // Initialize temperature sensor
  dht.setup(dhtPin, DHTesp::DHT11);
  Serial.println("DHT initiated");

}

bool getTemperature() {
  // Reading temperature for humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  TempAndHumidity newValues = dht.getTempAndHumidity();
  // Check if any reads failed and exit early (to try again).
  if (dht.getStatus() != 0) {
    Serial.println("DHT11 error status: " + String(dht.getStatusString()));
    return false;
  }
  
  //// Send temperature readings as json string in websocket message 
  //
  //
  int temp = (newValues.temperature * 1.8) + 32;
  int humidity = (newValues.humidity);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& object1 = jsonBuffer.createObject();
  object1["T"] = temp;
  object1["H"] = humidity;
  Serial.println("T: " + String(object1.get<int>("T")));
  Serial.println("H: " + String(object1.get<int>("H")));
  Serial.println("This is my temperature from json: ");
  String myString = "";
  object1.printTo(myString);
  Serial.println("myString: ");
  Serial.println(myString);
  client.send(String(myString));
  //
  //
  ////
  
  return true;
}

void setup() {

   WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.begin(115200);

  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);

  pinMode(relay, OUTPUT);

  connect();

  initTemp();
}

void loop() {
   client.poll();
   if(client.available() != 1){
    Serial.print(client.available()); 
    connect();
   }
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = HOST_NAME;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
//        Serial.print("HTTP Response code: ");
//        Serial.println(httpResponseCode);
        String payload = http.getString();
//        Serial.println(payload);
        const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
        DynamicJsonBuffer jsonBuffer(capacity);
        Serial.println("JSON covert..");
     
        JsonObject& root = jsonBuffer.parseObject(payload);
          if (!root.success()) {
            Serial.println("parseObject() failed");
          }
               
//        Serial.println("JSON print..");
          int START = root["start"];
          int END = root["end"];
          int CURRENT = root["current"];
          int FREQUENCY = root["frequency"];
          Serial.println("timerDelay");
          Serial.println(timerDelay);
          timerDelay = (1000 * FREQUENCY);
          Serial.println("timerDelay");
          Serial.println(timerDelay);
          if(CURRENT >= START && END >= CURRENT){
            digitalWrite(relay, HIGH);
          }
          else{//shut light off
            digitalWrite(relay, LOW);
          }
          getTemperature();
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
//  delay(timerDelay);
}
