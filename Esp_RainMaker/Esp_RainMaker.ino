#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include <wifi_provisioning/manager.h>
#include "AppInsights.h"

#define DEFAULT_RELAY_MODE false
#define DEFAULT_Temperature 0
#define DEFAULT_Humidity 0

const char *service_name = "PROV_12345";
const char *pop = "1234567";
bool wifi_connected = 0;

static uint8_t gpio_reset = 0;
static uint8_t relay_1 = 2;
static uint8_t relay_2 = 3;
static uint8_t relay_3 = 4;
static uint8_t relay_4 = 5;

//------------------------------------------- Declaring Devices -----------------------------------------------------//
//The framework provides some standard device types like switch, lightbulb, fan, temperature sensor.
//static TemperatureSensor temperature("Temperature");
//static TemperatureSensor humidity("Humidity");


static Switch my_switch1("Relay_1", &relay_1);
static Switch my_switch2("Relay_2", &relay_2);
static Switch my_switch3("Relay_3", &relay_3);
static Switch my_switch4("Relay_4", &relay_4);

void sysProvEvent(arduino_event_t *sys_event){
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_PROV_START:
  #if CONFIG_IDF_TARGET_ESP32
        Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
        printQR(service_name, pop, "ble");
  #else
        Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
        printQR(service_name, pop, "softap");
  #endif
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.printf("\nConnected to Wi-Fi!\n");
      wifi_connected = 1;
      delay(500);
      break;
    case ARDUINO_EVENT_PROV_CRED_RECV: {
        Serial.println("\nReceived Wi-Fi credentials");
        Serial.print("\tSSID : ");
        Serial.println((const char *) sys_event->event_info.prov_cred_recv.ssid);
        Serial.print("\tPassword : ");
        Serial.println((char const *) sys_event->event_info.prov_cred_recv.password);
        break;
      }
    case ARDUINO_EVENT_PROV_INIT:
      wifi_prov_mgr_disable_auto_stop(10000);
      break;
    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
      Serial.println("Stopping Provisioning!!!");
      wifi_prov_mgr_stop_provisioning();
      break;
  }
}

void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx){
  const char *device_name = device->getDeviceName();
  const char *param_name = param->getParamName();
  
  if (strcmp(device_name, "Relay_1") == 0){
    if (strcmp(param_name, "Power") == 0){
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      bool relay_state1 = val.val.b;
      (relay_state1 == false) ? digitalWrite(relay_1, LOW) : digitalWrite(relay_1, HIGH);
      param->updateAndReport(val);
    }
  }  

  if (strcmp(device_name, "Relay_2") == 0){
    if (strcmp(param_name, "Power") == 0){
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      bool relay_state2 = val.val.b;
      (relay_state2 == false) ? digitalWrite(relay_2, LOW) : digitalWrite(relay_2, HIGH);
      param->updateAndReport(val);
    }
  }

  if (strcmp(device_name, "Relay_3") == 0){
    if (strcmp(param_name, "Power") == 0){
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      bool relay_state3 = val.val.b;
      (relay_state3 == false) ? digitalWrite(relay_3, LOW) : digitalWrite(relay_3, HIGH);
      param->updateAndReport(val);
    }
  }

  if (strcmp(device_name, "Relay_4") == 0){
    if (strcmp(param_name, "Power") == 0){
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      bool relay_state4 = val.val.b;
      (relay_state4 == false) ? digitalWrite(relay_4, LOW) : digitalWrite(relay_4, HIGH);
      param->updateAndReport(val);
    }
  }
}

/* void Send_Sensor(){
  float h = millis();
  float t = millis() * millis();

  Serial.print("Temperature - "); Serial.println(t);
  Serial.print("Humidity - "); Serial.println(h);

  temperature.updateAndReportParam("Temperature", t);
  humidity.updateAndReportParam("Temperature", h);
} */

void setup(){
  Serial.begin(115200);
  pinMode(gpio_reset, INPUT_PULLUP);
  
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);
  
  digitalWrite(relay_1, DEFAULT_RELAY_MODE);
  digitalWrite(relay_2, DEFAULT_RELAY_MODE);
  digitalWrite(relay_3, DEFAULT_RELAY_MODE);
  digitalWrite(relay_4, DEFAULT_RELAY_MODE);
  //------------------------------------------- Declaring Node -----------------------------------------------------//
  Node my_node;
  my_node = RMaker.initNode("Saídas Digitais");
  my_switch1.addCb(write_callback);
  my_switch2.addCb(write_callback);
  my_switch3.addCb(write_callback);
  my_switch4.addCb(write_callback);
  //------------------------------------------- Adding Devices in Node -----------------------------------------------------//
//  my_node.addDevice(temperature);
//  my_node.addDevice(humidity);
  
  my_node.addDevice(my_switch1);
  my_node.addDevice(my_switch2);
  my_node.addDevice(my_switch3);
  my_node.addDevice(my_switch4);

  RMaker.enableOTA(OTA_USING_PARAMS);
  RMaker.enableTZService();
  RMaker.enableSchedule();

  Serial.printf("\nStarting ESP-RainMaker\n");
  RMaker.start();

  WiFi.onEvent(sysProvEvent);

  #if CONFIG_IDF_TARGET_ESP32
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
  #else
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
  #endif

}

long delayMs;
void loop(){

  /* if (millis() - delayMs > 5000 && wifi_connected) {
    delayMs = millis();
    Serial.println("Sending Sensor's Data");
    //Send_Sensor();
  } */



  //-----------------------------------------------------------  Logic to Reset RainMaker
  // Read GPIO0 (external button to reset device
  if (digitalRead(gpio_reset) == LOW) { //Push button pressed
    Serial.printf("Reset Button Pressed!\n");
    // Key debounce handling
    delay(100);
    int startTime = millis();
    while (digitalRead(gpio_reset) == LOW) delay(50);
    int endTime = millis();

    if ((endTime - startTime) > 7000) {
      // If key pressed for more than 7secs, reset all
      Serial.printf("Reset to factory.\n");
      wifi_connected = 0;
      RMakerFactoryReset(2);
    } else if ((endTime - startTime) > 3000) {
      Serial.printf("Reset Wi-Fi.\n");
      wifi_connected = 0;
      // If key pressed for more than 3secs, but less than 10, reset Wi-Fi
      RMakerWiFiReset(2);
    }
  }
  delay(100);
}
