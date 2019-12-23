#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>
#include <AsyncDelay.h>
int led = D1;
int sensor = D0;
int sensor2 = D2;
int sensor3 = D3;
int sensor4 = D5;
int sensor5 = D7;

int led2 = D8;

const char *ssid = "12yU";
const char *passw = "ryuryuryu";


void Line_Notify(String message) ;
#include <WiFiClientSecureAxTLS.h> // 
#define LINE_TOKEN "U56vzDkfKMwGzLA7bfzhvNKdstJGL4b69zc1RS2l4zY"
String message = "GrandPa haven't back to bedroom!!";



ESP8266WiFiMulti WiFiMulti;
WiFiClient esp8266Client;

AsyncDelay delay_5s;
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


  
void setup() {
  Serial.begin(9600);
  setup_wifi();
  pinMode(sensor, INPUT); // declare sensor as input
  pinMode(led, OUTPUT);  // declare LED as output
    pinMode(sensor2, INPUT); // declare sensor as input
  pinMode(led2, OUTPUT);  // declare LED as output
}
char msg[120];
long state = 0;
long checkS = 0;

  
void loop(){
  long sensorBed = digitalRead(sensor);
  long sensorWay1 = digitalRead(sensor2);
  if(sensorBed == HIGH && checkS == 0){
    
    Serial.println("Motion detected!");
    if(state == 0){
      state = 1;
      digitalWrite (led, HIGH);
      delay_5s.start(5000, AsyncDelay::MILLIS);
      delay_15s.start(15000, AsyncDelay::MILLIS);
      checkS = 1;
    }
    else {
      state = 0;
      digitalWrite(led, LOW);
      digitalWrite(led2, LOW);
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
  if (delay_15s.isExpired() && state == 1) {
      Serial.print("NOTIFICATION SUCCESSSSSSS!!");
      Line_Notify(message);
      delay_15s.repeat();
    
    }
  if(sensorWay1 == HIGH && state == 1){
    Serial.println("Motion222222222222222 DETECTEDDDD");
    digitalWrite (led2, HIGH);
    }
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
