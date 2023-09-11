#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <Servo.h>
#include "root.h"

#define pwmOut 2


const char* ssid = "VIVOFIBRA-9501";
const char* password = "rgw7ucm3GT";

byte pwm;


ESP8266WebServer server(80);
//Servo myservo;

void setup() {
  //myservo.attach(2);  // Conecte o servo ao pino GPIO2 (D4
  pinMode(pwmOut, OUTPUT);
  analogWrite(pwmOut, 0);
  delay(10);

  // Conecte-se à rede Wi-Fi
  Serial.println();
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println(WiFi.localIP());

  // Inicialize o servidor web
  server.on("/", HTTP_GET, handleRoot);
  server.on("/move", HTTP_GET, handleMove);
  server.on("/read", HTTP_GET, handleRead);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Servidor web iniciado");
}

void loop() {
  server.handleClient();
}

void handleRead() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Max-Age", "10000");
  server.send(200, "text/plain", String(pwm));//String(myservo.read()));
}

void handleRoot() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Max-Age", "10000");
  server.send(200, "text/html", root);
}

void handleMove() {
  String angleValue = server.arg("angle");
  int angle = angleValue.toInt();
  if (angle >= 0 && angle <= 255) {
    //myservo.write(angle);
    pwm = angle;
    analogWrite(pwmOut, pwm);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Max-Age", "10000");
    server.send(200, "text/html", root);
  } else {
    server.send(400, "text/plain", "Ângulo inválido");
  }
}

void handleNotFound() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Max-Age", "10000");
  server.send(404, "text/plain", "Not Found");
}
