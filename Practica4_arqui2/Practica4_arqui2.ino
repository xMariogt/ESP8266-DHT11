#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <DHT_U.h>

//Configuracion para el DHT11
int D1 = 5;
int D2 = 4;
float temperatura=0;
DHT dht(D1, DHT11);
float tempc, tempf, hume, htc, htf;

//Wifi
//const char *ssid = "CM-_-";
//const char *password = "ARRIS4749.*";
//const char *ssid = "iPhone de Mario";
//const char *password = "mariochava";
const char *ssid = "CLARO_CGy3jj";
const char *password = "DBA7811F8A";
//const char *ssid = "Jeff";
//const char *password = "j04a07d99";

//MQTT
const char *mqtt_server = "192.168.1.45";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

//Controlar mensajes recibidos

int convertirASCII(String numero){
  int entero = 0;
  if (numero == "48"){
    entero = 0;
  }else if(numero == "49"){
    entero = 1;
  }else if(numero == "50"){
    entero = 2;
  }else if(numero == "51"){
    entero = 3;
  }else if(numero == "52"){
    entero = 4;
  }else if(numero == "53"){
    entero = 5;
  }else if(numero == "54"){
    entero = 6;
  }else if(numero == "55"){
    entero = 7;
  }else if(numero == "56"){
    entero = 8;
  }else if(numero == "57"){
    entero = 9;
  }

  return entero;  
}

//
void setup_wifi(){
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);

  while (WiFi.status () != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mssage arrived [");
  Serial.print(topic);
  Serial.print("]");
  for (int i = 0; i<length; i++){
    Serial.print((char*)payload);
  }
  Serial.println();

  String dig1=(String)payload[0];
  String dig2= (String)payload[1];
  String dec1= (String)payload[3];
  String dec2= (String)payload[4];
  Serial.println(dig1);
  Serial.println(dig2);
  
  int num1 = convertirASCII(dig1);
  int num2 = convertirASCII(dig2);
  int num01 = convertirASCII(dec1);
  int num02 = convertirASCII(dec2);

  
  temperatura = (num1*10)+num2+(num01*0.1)+(num02*0.01);
}


void reconnect () {
  while (!client.connected()){
        Serial.print("Attemping MQTT connection... ");

    String clientId = "Chava-ESP8266";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())){
      Serial.println("connected");
      client.subscribe("UserTemp");
    }else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(500);
    }
  }
}

void setup(){
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback); //Esta funcion sirve para mostrar los mensajes que se reciben en el topico.
 
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT); //Este es el pin de salida para el relé
  dht.begin();
}

void loop(){
  if(!client.connected()){
    reconnect();
  }
  client.loop();
    hume = dht.readHumidity();
  //readTemperature() CELSIUS
  tempc = dht.readTemperature();
  //readTemperature(true) Fahrenheit
  tempf = dht.readTemperature(true);
  //Sensacion Termica
  htc = dht.computeHeatIndex(tempc, hume);
  htf = dht.computeHeatIndex(tempf, hume);
 String payloadData = "{\"temperature\": " + String(tempc) + ", \"humidity\": " + String(hume) + ", \"feels\": " + String(htc) + "}";

  client.publish("inTopic", payloadData.c_str());

  Serial.println("Celsius: " + String(tempc)+ " | Fahrenheit: " + String(tempf) + " Hume: "+ String(hume));
  Serial.println("");

  //Sensacion termica
  if (tempc >= temperatura){
    digitalWrite(D2, LOW);
  }else{
    digitalWrite(D2, HIGH);
  }

  //Serial.println("Sensacion termica en C: " + String(htc) + " | Sensacion termica en Fahrenheit: " + String(htf));

  delay(5000);
}
