#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <HTTPClient.h>

//const char* ssid1 = "Fios-WVBRA";
//const char* password = "SpectrotelSolutions123";
//const char* ssid = "Pixel_6944";
//const char* password = "jphotspot";
const char* ssid = "Fios-WVBRA_EXT";
const char* password = "pea4879carve43tony";

const char* websocket_server_host = "192.82.251.138";
const uint16_t websocket_server_port = 8888;
const int relay = 26;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
//unsigned long timerDelay = 600000;
unsigned long timerDelay = 5000;

using namespace websockets;
WebsocketsClient client;

void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
    if(message.data() == "NukeOn"){
      digitalWrite(relay, HIGH);
    }
    else if(message.data() == "NukeOff"){
      digitalWrite(relay, LOW);
    } 
}

void connect(){
  while (WiFi.status() != WL_CONNECTED) {
      Wifi();  
      delay(5000);
      Serial.println("WiFi Connected!");
  }
  
  
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

void Wifi(){
  WiFi.begin(ssid, password);
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
void setup() {
  Serial.begin(115200);
  Wifi();
  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);
  pinMode(relay, OUTPUT);
  connect();
}

void loop() {
  // put your main code here, to run repeatedly:
  client.poll();
   if(client.available() != 1){
    Serial.print(client.available()); 
    if(WiFi.status() != WL_CONNECTED){
      Wifi();
    }
    connect();
   }
  //Send an HTTP POST request every 5 seconds //10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      client.send("still here");
    }
    else {
      Serial.println("WiFi Disconnected");
      Wifi();
      connect();
    }
    lastTime = millis();
  }

}
