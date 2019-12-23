#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>
#include <AsyncDelay.h>
int led = D1;
int sensor = D3;

int sensor2 = D0;
int sensor3 = D5;
int sensor4 = D7;
//int sensor5 = D7;

int led2 = D4;
int led3 = D6;
int led4 = D8;
//int led5 = A0;

const char *ssid = "12yU";
const char *passw = "ryuryuryu";
const char *mqtt_server = "35.247.182.211";

#define MQTT_PUB_TOPIC "led/timeStart"

#define MQTT_PUB_TOPIC2 "led/timeStop"
#define MQTT_ID ""
ESP8266WiFiMulti WiFiMulti;
WiFiClient esp8266Client;
PubSubClient client(esp8266Client);

void Line_Notify(String message) ;
#include <WiFiClientSecureAxTLS.h> // 
#define LINE_TOKEN "U56vzDkfKMwGzLA7bfzhvNKdstJGL4b69zc1RS2l4zY"
String message = "GrandPa hasn't been back to bedroom!!";





AsyncDelay delay_5s;
//AsyncDelay delay_2s_1;
//AsyncDelay delay_2s_2;
AsyncDelay delay_15s;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFiMulti.addAP(ssid,passw);
  delay(10);

  while (WiFiMulti.run() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    
    }


    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
  
  }


 void reconnect() {
  while(!client.connected()) {
    Serial.print("connecting to MQTT...");
    if(client.connect(MQTT_ID)) {
      Serial.print("connected!");
//      client.subscribe(MQTT_SUB_TOPIC);
      }
    else {
      Serial.print("Connecting to MQTT failed!");
      Serial.print(client.state());
      delay(2000);
      }
    }
  }
  
void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(sensor, INPUT); // declare sensor as input
  pinMode(led, OUTPUT);  // declare LED as output
  pinMode(sensor2, INPUT); // declare sensor as input
  pinMode(led2, OUTPUT);  // declare LED as output
  pinMode(sensor3, INPUT); // declare sensor as input
  pinMode(led3, OUTPUT);  // declare LED as output
  pinMode(sensor4, INPUT); // declare sensor as input
  pinMode(led4, OUTPUT);  // declare LED as output
//    pinMode(sensor5, INPUT); // declare sensor as input
//  pinMode(led5, OUTPUT);  // declare LED as output
}
char msg[120];
int state = 0;
int checkS = 0;

  
void loop(){

  if(!client.connected())
  {
    reconnect();
  }
  client.loop();
  long sensorBed = digitalRead(sensor);
  //long sensorWay1 = digitalRead(sensor2);
  //long sensorWay2 = digitalRead(sensor3);
  //long sensorWay3 = digitalRead(sensor4);
  //Serial.print("WAY3: ");
  //Serial.println(sensorWay3);

//  long sensorWay4 = digitalRead(sensor5);
  if(sensorBed == HIGH && checkS == 0){
    
    Serial.println("Motion detected!");
    if(state == 0){
      state = 1;
      digitalWrite (led, HIGH);
      client.publish(MQTT_PUB_TOPIC, "start");
      delay_5s.start(5000, AsyncDelay::MILLIS);
      delay_15s.start(15000, AsyncDelay::MILLIS);
      checkS = 1;
    }
    else {
      state = 0;
      digitalWrite(led, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      client.publish(MQTT_PUB_TOPIC2, "stop");
      checkS = 1;
      delay_5s.start(5000, AsyncDelay::MILLIS);
      }
  } else {
    
    Serial.println("Motion absent!");
    delay(500);
  }
  if (delay_5s.isExpired()) {
    checkS = 0;
    }
  if (delay_15s.isExpired() && state > 0) {
      Serial.println("NOTIFICATION SUCCESSSSSSS!!");
      Line_Notify(message);
      delay_15s.repeat();
    
    }
   long sensorWay1 = digitalRead(sensor2);
  if(sensorWay1 == HIGH && state == 1){
    Serial.println("Motion2222222 DETECTEDDDD");
    state = 2;
    digitalWrite (led2, HIGH);
    }
    long sensorWay2 = digitalRead(sensor3);
  if(sensorWay2 == HIGH && state == 2){
    Serial.println("Motion3333333 DETECTEDDDD");
    state = 3;
    digitalWrite (led3, HIGH);
    //delay_2s_1.start(100, AsyncDelay::MILLIS);
    }
    long sensorWay3 = digitalRead(sensor4);
  if(sensorWay3 == HIGH && state == 3 ){
    Serial.println(state , sensorWay3);


    state = 4;
    digitalWrite (led4, HIGH);
    //delay_2s_2.start(100, AsyncDelay::MILLIS);
//    delay(500);
    }
//  if(sensorWay4 == HIGH && state == 4 ){
//    Serial.println("Motion5555555 DETECTEDDDD");
//    state = 5;
//    analogWrite(A0, 255);
//    }
delay(1000);
Serial.println("1 loop");

}

void Line_Notify(String message) {
  axTLS::WiFiClientSecure client; // กรณีขึ้น Error ให้ลบ axTLS:: ข้างหน้าทิ้ง

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}
