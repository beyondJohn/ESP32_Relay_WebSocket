#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <HTTPClient.h>

const int  HTTP_PORT   = 80;
const char* HTTP_METHOD = "GET"; // or "POST"
const char* HOST_NAME = "http://switchmagic.com:8000/data"; // hostname of web server:

//const char* ssid = "Fios-WVBRA_EXT";
//const char* password = "pea4879carve43tony";
const char* ssid = "Pixel_6944";
const char* password = "jphotspot";
const char* websocket_server_host = "192.82.251.138";
const uint16_t websocket_server_port = 8888;
const int relay = 26;

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
    }else if(message.data() == "just checking"){
      Serial.println(message.data());
    } 
    else{
      digitalWrite(relay, HIGH);
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
        digitalWrite(relay, LOW);
        delay(1000);
        connect();
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("GotPong");
    }
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
}

void loop() {
   client.poll();
   if(client.available() != 1){
    Serial.print(client.available()); 
    connect();
   }
   ////
   ////
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
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
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
   ////
   ////
   
}
