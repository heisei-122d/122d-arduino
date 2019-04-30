#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char ssid[] = "duck_wifi";
const char passwd[] = "duckduck";

// Pub/Sub
const char* mqttHost = "192.168.1.3"; // MQTTのIPかホスト名
const int mqttPort = 1883;       // MQTTのポート
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

const char* topic = "test";     // 送信先のトピック名
char* payload;                   // 送信するデータ

const int SW_START = 34;
const int SW_1 = 35;
const int SW_2 = 32;
const int SW_3 = 33;
const int SW_4 = 25;

void setup() {
    Serial.begin(38400);

    // Connect WiFi
    connectWiFi();

    // Connect MQTT
    connectMqtt();

    pinMode(SW_START, INPUT);
    pinMode(SW_1, INPUT);
    pinMode(SW_2, INPUT);
    pinMode(SW_3, INPUT);
    pinMode(SW_4, INPUT);
}

void loop() {
    // 送信処理 topic, payloadは適宜
    payload = "payload";
    //mqttClient.publish(topic, payload);
    updateButton();
    // WiFi
    if ( WiFi.status() == WL_DISCONNECTED ) {
        connectWiFi(); 
    }
    // MQTT
    if ( ! mqttClient.connected() ) {
        connectMqtt();
    }
    mqttClient.loop();  

}
/**
 * Interrupts
 */
int previousState = 0; // 0 means no button have been pushed
int currentState = 0; 
void updateButton(){
  currentState = getState();
  if(previousState != currentState){
      previousState = currentState;
      delay(10);
      if(digitalRead(SW_START) == LOW){
        itrStart();
        delay(50);
      }
      if(digitalRead(SW_1) == LOW) {
        itr1();
      }
      if(digitalRead(SW_2) == LOW) {
        itr2();
      }
      if(digitalRead(SW_3) == LOW) {
        itr3();
      }
      if(digitalRead(SW_4) == LOW) {
        itr4();
      }
   }
  //delay(1000);
}
int getState(){
  return digitalRead(SW_START)*1 + digitalRead(SW_1)*2 + digitalRead(SW_2)*4 + digitalRead(SW_3)*8 + digitalRead(SW_4)*16; 
}
void itrStart(){
  Serial.println("SEND START");
  if(mqttClient.connected()){
    mqttClient.publish(topic, "5_dc");
  }
}
void itr1(){
  Serial.println("SEND 1");
  if(mqttClient.connected()){
    mqttClient.publish(topic, "1_dc");
  }
}
void itr2(){
  Serial.println("SEND 2");
  if(mqttClient.connected()){
    mqttClient.publish(topic, "2_dc");
  }
}
void itr3(){
  Serial.println("SEND 3");
  if(mqttClient.connected()){
    mqttClient.publish(topic, "3_dc");
  }
}
void itr4(){
  Serial.println("SEND 4");
  if(mqttClient.connected()){
    mqttClient.publish(topic, "4_dc");
  }
}
/**
 * Connect WiFi
 */
void connectWiFi()
{
    WiFi.begin(ssid, passwd);
    Serial.print("WiFi connecting...");
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.print(" connected. ");
    Serial.println(WiFi.localIP());
}
/**
 * Connect MQTT
 */
void connectMqtt()
{
    mqttClient.setServer(mqttHost, mqttPort);
    while( ! mqttClient.connected() ) {
        Serial.println("Connecting to MQTT...");
        String clientId = "ESP32-" + String(random(0xffff), HEX);
        if ( mqttClient.connect(clientId.c_str()) ) {
            Serial.println("connected"); 
        }
        delay(1000);
        randomSeed(micros());
    }
}
