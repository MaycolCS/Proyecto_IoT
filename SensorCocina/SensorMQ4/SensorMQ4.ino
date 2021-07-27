#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <Wire.h>

int contconexion = 0;

const char *ssid = "LosPeces";
const char *password = "01011964";

unsigned long previousMillis = 0;

char host[48];
String strhost = "192.168.10.26";
String strurl = "/Page_IoT/php/envioDatosCocina.php";
String chipid = "";

int ledrojo= 16; //D0
int ledverde= 5; //D1
int ledamarillo= 4; //D2
int sensor = A0;
int sensorValue = 0;
int cont_Alarma = 0;

void setup() {

  pinMode(ledrojo, OUTPUT);
  pinMode(ledverde, OUTPUT);
  pinMode(ledamarillo, OUTPUT);
  
  // Inicia Serial
  Serial.begin(115200);
  Serial.print("chipId: "); 
  chipid = String(ESP.getChipId());
  Serial.println(chipid); 

  // Conexión WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { //Cuenta hasta 50 si no se puede conectar lo cancela
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      IPAddress ip(195,168,10,29); 
      IPAddress gateway(192,168,10,1); 
      IPAddress subnet(255,255,255,0); 
      WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

//-------Función para Enviar Datos a la Base de Datos SQL--------

String enviardatos(String datos) {
  String linea = "error";
  WiFiClient client;
  strhost.toCharArray(host, 49);
  if (!client.connect(host, 80)) {
    Serial.println("Fallo de conexion");
    return linea;
  }

  client.print(String("POST ") + strurl + " HTTP/1.1" + "\r\n" + 
               "Host: " + strhost + "\r\n" +
               "Accept: */*" + "*\r\n" +
               "Content-Length: " + datos.length() + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
               "\r\n" + datos);           
  //delay(10);             
  
  Serial.print("Enviando datos a SQL...");
  
  unsigned long timeout = millis();

  while (client.available() == 0) {
    if (millis() - timeout > 3000) {
      Serial.println("Cliente fuera de tiempo!");
      client.stop();
      return linea;
    }
  }
  // Lee todas las lineas que recibe del servidor y las imprime por la terminal serial
  while(client.available()){
    linea = client.readStringUntil('\r');
  }  
  Serial.println(linea);
  return linea;
}

void activarAlarma(int led, int lectura) {
  String nivel;
  if (sensorValue > 20 && sensorValue < 35){
    nivel = "Bajo";
  }
  else if (sensorValue >= 35 && sensorValue < 50){
    nivel = "Medio";
  }
  else if (sensorValue >= 50){
    nivel = "Alto";
  }
  enviardatos("chipid=" + chipid + "&NLS=" + nivel + "&LS=" + String(lectura));
  while (cont_Alarma <= 2) {
    digitalWrite(led,HIGH);
    tone(10,100,1000);
    delay (500);
    digitalWrite(led,LOW);
    tone(10,200,1000);
    cont_Alarma = cont_Alarma + 1;
    delay (500);
  }
  cont_Alarma = 0;
}

void loop() {
  sensorValue = analogRead(sensor);
  if(sensorValue > 20) {
    Serial.println(sensorValue);
  }
  digitalWrite(ledverde, LOW);
  digitalWrite(ledamarillo, LOW);
  digitalWrite(ledrojo, LOW);
  if (sensorValue > 20 && sensorValue < 35){
    activarAlarma(ledverde, sensorValue);
  }
  else if (sensorValue >= 35 && sensorValue < 50){
    activarAlarma(ledamarillo, sensorValue);
  }
  else if (sensorValue >= 50){
    activarAlarma(ledrojo, sensorValue);
  }
}
