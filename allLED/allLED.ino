#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>
int led1 = D1;
int led2 = D0;

const char *ssid = "FRONGWEI";
const char *passw = "frongpsc";
const char *mqtt_server = "35.247.182.211";

#define MQTT_SUB_TOPIC "led/switch"
#define MQTT_ID ""
#define MQTT_PUB_TOPIC "led/timeStartA"
#define MQTT_PUB_TOPIC2 "led/timeStopA"
#define MQTT_PUB_TOPIC3 "led/timeStartB"
#define MQTT_PUB_TOPIC4 "led/timeStopB"

ESP8266WiFiMulti WiFiMulti;
WiFiClient esp8266Client;
PubSubClient client(esp8266Client);

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

bool stat = false;
bool stat2 = false;
void callback(char *topic, byte *payload, unsigned int length) {
  String mes = "";
  Serial.print("Message arrived [ ");
  Serial.print(topic);
  Serial.print(" ]");
  for (int i = 0; i < length; i++) {
//    Serial.println((char)payload[i]);
    mes = mes + (char)payload[i];
   
    }
     Serial.print(mes);
  if(mes == "on" && !stat) {
    digitalWrite(led1, HIGH);
          client.publish(MQTT_PUB_TOPIC, "start");

    stat = true;
    } else if (mes == "on" && stat) {
      digitalWrite(led1, LOW);
          client.publish(MQTT_PUB_TOPIC2, "stop");

      stat = false;
      }
  else if (mes == "off" && !stat2) {
    digitalWrite(led2, HIGH);
              client.publish(MQTT_PUB_TOPIC3, "start");

    stat2 = true;
    } else if (mes == "off" && stat2) {
      digitalWrite(led2, LOW);
                client.publish(MQTT_PUB_TOPIC4, "stop");

      stat2 = false;
      }
  Serial.println();
  }

void reconnect() {
  while(!client.connected()) {
    Serial.print("connecting to MQTT...");
    if(client.connect(MQTT_ID)) {
      Serial.print("connected!");
      client.subscribe(MQTT_SUB_TOPIC);
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
  client.setCallback(callback);
  pinMode(led1, OUTPUT); // declare sensor as input
  pinMode(led2, OUTPUT);  // declare LED as output
}
char msg[120];
long state = 0;
void loop(){
  if(!client.connected()) {
      reconnect();
    }
   client.loop();
}
