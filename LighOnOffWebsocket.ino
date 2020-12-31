#include <WiFi.h>
#include <ArduinoWebsockets.h>

const char* ssid = "Fios-WVBRA_EXT";
const char* password = "";
const char* websocket_server_host = "192.82.251.138";
const uint16_t websocket_server_port = 8888;
const int relay = 26;

using namespace websockets;
WebsocketsClient client;
void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
    if(message.data() == "hey hi ho"){
      digitalWrite(relay, LOW);
    } else{
      digitalWrite(relay, HIGH);
    }

}

void connect(){
    
  while(!client.connect(websocket_server_host, websocket_server_port, "/")){
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("Websocket Connected!");
  
  Serial.begin(115200);
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("ConnectionOpened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("ConnectionClosed");
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

  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);

  connect();

  pinMode(relay, OUTPUT);
}

void loop() {
   client.poll();
}
