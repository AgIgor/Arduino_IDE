#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

const char* ssid = "VIVOFIBRA-9501";
const char* password = "rgw7ucm3GT";
const char* url = "ftp://192.168.15.4:9999/Download/HTTP/Url_Update.ino.bin"; // Substitua pela URL do seu firmware

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  
  // Conectar-se à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");

  // Verificar se há atualizações disponíveis
  //if (checkForUpdate()) {
    // Baixar e aplicar a atualização
    downloadAndApplyUpdate();
  //}

  // Seu código principal aqui
}

long blinkLed;
void loop() {
  // Seu loop principal aqui
  if(millis() - blinkLed > 500){
    blinkLed = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

bool checkForUpdate() {
  Serial.println("Verificando atualizações...");

  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Atualização disponível");
    http.end();
    return true;
  } else {
    Serial.println("Nenhuma atualização disponível");
    http.end();
    return false;
  }
}

void downloadAndApplyUpdate() {
  Serial.println("Baixando atualização...");

  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    // Iniciar o processo de atualização
    Serial.println("Iniciando processo de atualização...");
    Update.begin(httpCode);
    Update.writeStream(http.getStream());
    Update.end();

    // Verificar se a atualização foi bem-sucedida
    if (Update.end(true)) {
      Serial.println("Atualização concluída com sucesso!");
    } else {
      Serial.println("Falha na atualização");
    }
  } else {
    Serial.println("Falha ao baixar o firmware");
  }

  http.end();
}
