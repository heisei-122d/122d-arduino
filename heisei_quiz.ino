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

const int SW_RED = 34;
const int SW_BLUE = 35;
const int SW_YELLOW = 32;
const int SW_GREEN = 33;

void setup() {
    Serial.begin(38400);

    // Connect WiFi
    connectWiFi();

    // Connect MQTT
    connectMqtt();

    pinMode(SW_RED, INPUT);
    pinMode(SW_BLUE, INPUT);
    pinMode(SW_YELLOW, INPUT);
    pinMode(SW_GREEN, INPUT);
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
void updateButton(){
  if(Serial.available() > 0) {
    previousByte = incomingByte;
    incomingByte = Serial.read();
    Serial.println(incomingByte);
    if(incomingByte != previousByte){
      if(incomingByte == 0x00 || incomingByte == '0') itrRed();
      else if(incomingByte == 0x01 || incomingByte == '1') itrBlue();
      else if(incomingByte == 0x02 || incomingByte == '2') itrYellow();
      else if(incomingByte == 0x03 || incomingByte == '3') itrGreen();
      //delay(1000);
    }
  }
}
void itrRed(){
  Serial.println("SEND 0");
  if(mqttClient.connected()){
    mqttClient.publish(topic, "0");
  }
}
void itrBlue(){
  Serial.println("SEND 1");
  if(mqttClient.connected()){
    mqttClient.publish(topic, "1");
  }
}
void itrGreen(){
  Serial.println("SEND 2");
  if(mqttClient.connected()){
    mqttClient.publish(topic, "2");
  }
}
void itrYellow(){
  Serial.println("SEND 3");
  if(mqttClient.connected()){
    mqttClient.publish(topic, "3");
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
    mqttClient.setCallback(callback);
    while( ! mqttClient.connected() ) {
        Serial.println("Connecting to MQTT...");
        String clientId = "ESP32-" + String(random(0xffff), HEX);
        if ( mqttClient.connect(clientId.c_str()) ) {
            Serial.println("connected"); 
            mqttClient.subscribe(subtopic);
        }
        delay(1000);
        randomSeed(micros());
    }
}