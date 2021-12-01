#include <DHT.h> 
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <string>


DHT dht(D3, DHT11);


float temp;
String ssid = "Iot_prueba";
String password = "Prueba12";

byte cont = 0;
byte max_intentos= 50;

ESP8266WiFiMulti wifiMulti;

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(1000);
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("Iot_prueba", "Prueba12");
  int cont = 0;
  
     
}
String post = "http://code-med-iot.herokuapp.com/api/contenedores/";

void loop(){
 
    temp = dht.readTemperature();
    Serial.println("Temperatura: " + String(temp));
    if(temp > 4){
      digitalWrite(D0, HIGH);
    }  

  
  if((wifiMulti.run()==WL_CONNECTED)){
    if(cont == 0){
      
      StaticJsonBuffer<200> jsonBuffer;
      WiFiClient client;
      HTTPClient http_postinicial;
      Serial.println("[HTTP] Iniciando...");
      http_postinicial.begin(client, post);
      Serial.print("[HTTP] Post...");
      http_postinicial.addHeader("Content-Type", "application/json");
      http_postinicial.POST("{\"contenido\": \"Corazón\",\"from\": \"Hospital general de Huichapan\",\"to\": \"Hospital San José\",\"temperaturaOptima\": \"4\",\"duracion\": \"8\"}");
      String respuesta = http_postinicial.getString();
      Serial.print(respuesta);
      JsonObject& root = jsonBuffer.parseObject(respuesta);
      const char* id_char = root["id"];
      String id_string(id_char);
      post.concat(id_string);
      http_postinicial.end();
      cont  = 1;
    }
    else{
      WiFiClient client;
      HTTPClient http;
      Serial.println("[HTTP] Iniciando...");
      http.begin(client,post);
      Serial.print("[HTTP] Post...");
      http.addHeader("Content-Type", "application/json");
      http.POST("{\"temperatura\":" + String(temp) + " }");
      String respuesta = http.getString();
      Serial.print(respuesta);
      http.end();
      }
  }
   
   delay(5000);
}
