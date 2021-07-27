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
String strurl = "/Page_IoT/php/envioDatosPasillos.php";
String chipid = "";

int sensor_PIR_1 = 16; //D0
int sensor_PIR_2 = 5; //D1
int led_Pasillos_1 = 4; //D2
int led_Pasillos_2 = 0; //D3
int buzzer = A0;
bool alarma_Encendida = true;
int data_PIR_1;
int data_PIR_2;
int cont_PIR = 0;

void setup() {
  pinMode (sensor_PIR_1, INPUT);
  pinMode (sensor_PIR_2, INPUT);
  pinMode (led_Pasillos_1, OUTPUT);
  pinMode (led_Pasillos_2, OUTPUT);
  
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

void loop() {
  data_PIR_1 = digitalRead(sensor_PIR_1);
  data_PIR_2 = digitalRead(sensor_PIR_2);

  if (cont_PIR <= 1) {
    if (data_PIR_1 == 1) { //Pasillo principal
      digitalWrite(led_Pasillos_1,HIGH);
      digitalWrite(led_Pasillos_2,LOW);
      enviardatos("chipid=" + chipid + "&ELS=" + "Alarma apagada" + "&LS=" + data_PIR_1);
    }
    else if (data_PIR_2 == 1) { //Pasillo sala - comedor
      digitalWrite(led_Pasillos_1,LOW);
      digitalWrite(led_Pasillos_2,HIGH);
      enviardatos("chipid=" + chipid + "&ELS=" + "Alarma apagada" + "&LS=" + data_PIR_2);
    }
    else{
      digitalWrite(led_Pasillos_1,LOW);
      digitalWrite(led_Pasillos_2,LOW);
    }
    cont_PIR = cont_PIR + 1;
  }
  else {
    if (data_PIR_1 == 1 || data_PIR_2 == 1) {
      if (data_PIR_1 == 1) {
        enviardatos("chipid=" + chipid + "&ELS=" + "Alarma encendida" + "&LS=" + data_PIR_1);
      } else if (data_PIR_2 == 1) {
        enviardatos("chipid=" + chipid + "&ELS=" + "Alarma encendida" + "&LS=" + data_PIR_2);
      }
      while (alarma_Encendida) {
        tone(A0,100,1000);
        delay (500);
        tone(A0,200,1000);
        delay (500);
        tone(A0,300,1000);
        delay (500);
        tone(A0,400,1000);
        delay (500);
        tone(A0,300,1000);
        delay (500);
        tone(A0,200,1000);
        delay (500);
        tone(A0,100,1000);
        delay (500);
        tone(A0,50,1000);
        delay (500);
      }
    }
    if (cont_PIR == 2) {
      cont_PIR = 0;
    }
  }
}
