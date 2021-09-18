#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <Servo.h>
#include <String>
int data;
Servo myservo;
WebSocketsClient webSocket;
const char* ssid = "TriDang111"; //Đổi thành wifi của bạn
const char* password = "24681234"; //Đổi pass luôn
const char* ip_host = "192.168.1.105"; //Đổi luôn IP host của PC nha
const uint16_t port = 3000; //Port thích đổi thì phải đổi ở server nữa
const int LEDkhach = D6;
const int LEDngu = D7;
const int LEDvs = D8;
int pos=0;
int pos_pre=0;
bool isNumber(const String& str);
String convertToString(char* a, int size);
static const int servoPin = D5;
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        Serial.printf("[WSc] Connected to url: %s\n", payload);
      }
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);
      if (strcmp((char*)payload, "LED_phong_khach_tat") == 0) {
        digitalWrite(LEDkhach, 0); // Khi client phát sự kiện "LED_OFF" thì server sẽ bật LED
      } else if (strcmp((char*)payload, "LED_phong_khach_bat") == 0) {
        digitalWrite(LEDkhach, 1); // Khi client phát sự kiện "LED_ON" thì server sẽ tắt LED
      } else if (strcmp((char*)payload, "LED_phong_ngu_bat") == 0) {
        digitalWrite(LEDngu, 1); // Khi client phát sự kiện "LED_ON" thì server sẽ tắt LED
      } else if (strcmp((char*)payload, "LED_phong_ngu_tat") == 0) {
        digitalWrite(LEDngu, 0); // Khi client phát sự kiện "LED_OFF" thì server sẽ tắt LED
      } else if (strcmp((char*)payload, "LED_phong_vs_bat") == 0) {
        digitalWrite(LEDvs, 1); // Khi client phát sự kiện "LED_ON" thì server sẽ tắt LED
      } else if (strcmp((char*)payload, "LED_phong_vs_tat") == 0) {
        digitalWrite(LEDvs, 0); // Khi client phát sự kiện "LED_OFF" thì server sẽ tắt LED
      } else if(isNumber(convertToString((char*)payload,sizeof((char*)payload)))){
        pos= atoi((char*)payload);
        Serial.println(pos);
          if(pos==90){
            for(int i=0;i<=pos;i++){
              myservo.write(i);
              delay(20);
            }
          }
      else if(pos==0){
        for(int i=pos_pre;i>=0;i--){
          myservo.write(i);
          delay(20);
        }  
      }
      else{
        if(pos > pos_pre){
          for(int i=pos_pre;i<=pos;i++){
          myservo.write(i);
          delay(20);
          }
        }else{
          for(int i=pos_pre;i>=pos;i--){
          myservo.write(i);
          delay(20);
          }        
        }
      }
      pos_pre=pos;
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      break;
  }
  }
}
void setup() {
  pinMode(LEDkhach, OUTPUT);
  pinMode(LEDngu, OUTPUT);
  pinMode(LEDvs, OUTPUT);
  myservo.attach(servoPin);
  Serial.begin(115200);
  Serial.println("ESP8266 Websocket Client");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  webSocket.begin(ip_host, port);
  webSocket.onEvent(webSocketEvent);
}
void loop() {
  while (Serial.available()){
    data = Serial.read();
  }
  webSocket.loop();
  static bool isKhachOn = false;
  if (!isKhachOn && digitalRead(LEDkhach) == 0) { //tat den phong khach
    isKhachOn = true;
    webSocket.sendTXT("Led_khach_off");
  } else if (isKhachOn && digitalRead(LEDkhach)) { //bat den phong khach
    isKhachOn = false;
    webSocket.sendTXT("Led_khach_on");
  }
  static bool isNguOn = false;
  if (!isNguOn && digitalRead(LEDngu) == 0) { //tat den phong ngu
    isNguOn = true;
    webSocket.sendTXT("Led_ngu_off");
  } else if (isNguOn && digitalRead(LEDngu)) { //bat den phong ngu
    isNguOn = false;
    webSocket.sendTXT("Led_ngu_on");
  }
  static bool isvsOn = false;
  if (!isvsOn && digitalRead(LEDvs) == 0) { //tat den ve sinh
    isvsOn = true;
    webSocket.sendTXT("Led_vs_off");
  } else if (isvsOn && digitalRead(LEDvs)) { //bat den ve sinh
    isvsOn = false;
    webSocket.sendTXT("Led_vs_on");
  }
}
bool isNumber(const String& str)
{
    for (char const &c : str) {
        if (isdigit(c) == 0) return false;
    }
    return true;
}
String convertToString(char* a, int size)
{
    String s(a);
    return s;
}
