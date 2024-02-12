#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


void setup() {
    // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    Serial.begin(115200);
    pinMode(0, INPUT_PULLUP);
    WiFiManager wm;
    
    while(millis()/1000 <= 5){
      Serial.println(millis()/1000);
      if(!digitalRead(0)){
        wm.resetSettings();
        delay(500);
        ESP.restart();
      }
    }
    
    bool res;
    // wm.resetSettings();
    //res = wm.autoConnect(); // auto generated AP name from chipid
    //res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","12345678"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else { 
        Serial.println("connected...yeey :)");
    }

}

void loop() {
    // put your main code here, to run repeatedly:   
}
