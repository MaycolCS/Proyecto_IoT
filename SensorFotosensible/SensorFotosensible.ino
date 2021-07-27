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
String strurl = "/Page_IoT/php/envioDatosLucesExteriores.php";
String chipid = "";


bool sensor_Activado = true;
int led1 = 16; //D0
int foto = A0;

void setup() {
  pinMode (foto, INPUT);
  pinMode (led1, OUTPUT); 

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
  int luz;
  luz = analogRead(foto);
  
  if ( luz <= 10)
  {
    Serial.print("Lectura: ");
    Serial.println(luz);
    enviardatos("chipid=" + chipid + "&LS=" + String(luz));
    digitalWrite (led1, HIGH);
  }
  else {
    digitalWrite (led1, LOW);
  }
}
