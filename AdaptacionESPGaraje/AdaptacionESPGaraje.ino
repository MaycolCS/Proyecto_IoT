#include <Servo.h>
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <SPI.h>

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
String strurl = "/Page_IoT/php/envioDatosGaraje.php";
String chipid = "";


Servo servo_1; //Puerta derecha - pin 6
Servo servo_2; //Puerta izquierda - pin 5

 /* ------------------------------------
 *             MFRC522      ESP
 *             Reader/PCD   8266
 * Signal      Pin          F
 * ------------------------------------
 * RST/Reset   RST          D8 - 15
 * SPI SS      SDA(SS)      D7 - 13
 * SPI MOSI    MOSI         D6 - 12
 * SPI MISO    MISO         D5 - 14
 * SPI SCK     SCK          D4 - 2
 */

constexpr uint8_t RST_PIN = 15;
constexpr uint8_t SS_PIN = 13;

MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522 al cual llamamos Lector1


int datosRFID[4];
int tarjetaUno[] = {132,143,224,43};
int tarjetaDos[] = {82,37,223,43};
int tarjetaTres[] = {150,1,183,172};
int tarjetaCuatro[] = {6,225,91,172};
int j = 0;
int CodigoAceptado = 0;
int PuertasAbiertas = 0;

void setup()
{
  servo_1.attach(0); //D3
  servo_2.attach(2); //D2
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  // Inicia Serial
  Serial.begin(115200);
  Serial.println("");
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

void servo_angulo_aumenta (int servo)// Función del servo aumenta
{
  int cont = 0;
  while (cont < 90) {
    if (servo == 1) {
      servo_1.write(cont);
    } else if (servo == 2) {
      servo_2.write(cont);
    }
    cont = cont + 1;
    delay(25);
  }
}

void servo_angulo_disminuye (int servo)// Función del servo disminuye
{
  int cont = 90;
  while (cont > 0) {
    if (servo == 1) {
      servo_1.write(cont);
    } else if (servo == 2) {
      servo_2.write(cont);
    }
    cont = cont - 1;
    delay(25);
  }
}

void servo_inicializar_angulo ()// Función del servo al angulo cero
{
  Serial.println("Inicializando angulo puerta derecha");
  //Puerta derecha
  int cont_1 = servo_1.read();
  while (cont_1 > 0) {
    cont_1 = cont_1 - 1;
    servo_1.write(cont_1);
    delay(25);
  }
  Serial.println("Inicializando angulo puerta izquierda");
  //Puerta izquierda
  int cont_2 = servo_2.read();
  while (cont_2 > 90) {
    cont_2 = cont_2 - 1;
    servo_2.write(cont_2);
    delay(25);
  }
  Serial.println("Angulo puertas inicializado");
}

void loop() {
  if (PuertasAbiertas == 0) {
    if (servo_1.read() != 0 && servo_2.read() != 90)
      servo_inicializar_angulo();
  }
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  Serial.println("IDENTIFICACION");
  while (j< mfrc522.uid.size) {
    datosRFID[j] = (mfrc522.uid.uidByte[j]);
    Serial.println(datosRFID[j]);
    j++;
  }

  if (j==4){
    //Lectura tarjeta 1
    if (datosRFID[0] == tarjetaUno[0] && datosRFID[1] == tarjetaUno[1] &&datosRFID[2] == tarjetaUno[2] && datosRFID[3] == tarjetaUno[3]) {
      CodigoAceptado = 1;
      if (PuertasAbiertas == 0) {
        enviardatos("chipid=" + chipid + "&NP=" + "Maycol" + "&AP=" + "Canastero" + "&ITP=" + String(String(tarjetaUno[0])+"-"+String(tarjetaUno[1])+"-"+String(tarjetaUno[2])+"-"+String(tarjetaUno[3])));
      }
      Serial.println("Acceso correcto");
      j = 0;
    }
    //Lectura tarjeta 2
    else if (datosRFID[0] == tarjetaDos[0] && datosRFID[1] == tarjetaDos[1] &&datosRFID[2] == tarjetaDos[2] && datosRFID[3] == tarjetaDos[3]) {
      CodigoAceptado = 1;
      if (PuertasAbiertas == 0) {
        enviardatos("chipid=" + chipid + "&NP=" + "Andres" + "&AP=" + "Soler" + "&ITP=" + String(String(tarjetaDos[0])+"-"+String(tarjetaDos[1])+"-"+String(tarjetaDos[2])+"-"+String(tarjetaDos[3])));
      }
      Serial.println("Acceso correcto");
      j = 0;
    }
    //Lectura tarjeta 3
    else if (datosRFID[0] == tarjetaTres[0] && datosRFID[1] == tarjetaTres[1] &&datosRFID[2] == tarjetaTres[2] && datosRFID[3] == tarjetaTres[3]) {
      CodigoAceptado = 1;
      if (PuertasAbiertas == 0) {
        enviardatos("chipid=" + chipid + "&NP=" + "Andres" + "&AP=" + "Jimenez" + "&ITP=" + String(String(tarjetaTres[0])+"-"+String(tarjetaTres[1])+"-"+String(tarjetaTres[2])+"-"+String(tarjetaTres[3])));
      }
      Serial.println("Acceso correcto");
      j = 0;
    }
    //Lectura tarjeta 4
    else if (datosRFID[0] == tarjetaCuatro[0] && datosRFID[1] == tarjetaCuatro[1] &&datosRFID[2] == tarjetaCuatro[2] && datosRFID[3] == tarjetaCuatro[3]) {
      CodigoAceptado = 1;
      if (PuertasAbiertas == 0) {
        enviardatos("chipid=" + chipid + "&NP=" + "Mario" + "&AP=" + "Gonzalez" + "&ITP=" + String(String(tarjetaCuatro[0])+"-"+String(tarjetaCuatro[1])+"-"+String(tarjetaCuatro[2])+"-"+String(tarjetaCuatro[3])));
      }
      Serial.println("Acceso correcto");
      j = 0;
    }
  }

  if (CodigoAceptado==1) {
    if (PuertasAbiertas == 0) {
      Serial.println("Abriendo puerta derecha");
      servo_angulo_aumenta(1); //Puerta derecha
      Serial.println("Abriendo puerta izquierda");
      servo_angulo_disminuye(2); //Puerta izquierda
      Serial.println("Puertas abiertas");
      PuertasAbiertas = 1;
    }
    else if (PuertasAbiertas == 1) {
      Serial.println("Cerrando puerta derecha");
      servo_angulo_disminuye(1); //Puerta derecha
      Serial.println("Cerrando puerta izquierda");
      servo_angulo_aumenta(2); //Puerta izquierda
      Serial.println("Puertas cerradas");
      PuertasAbiertas = 0;
    }
  }
  else {
    Serial.println("Acceso incorrecto");
    CodigoAceptado = 0;
    j = 0;
    datosRFID[4] = 0;
  }
}
