#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"

#define DEFAULT_POWER_MODE LOW
const char *service_name = "PROV_SONOFF_DUALR3";
const char *pop = "123456";

// GPIO for push button
static uint8_t gpio_reset = 0;

// GPIO for switch
static uint8_t gpio_switch1 = 26;
static uint8_t gpio_switch2 = 25;
static uint8_t gpio_switch3 = 32;
static uint8_t gpio_switch4 = 33;

// GPIO for virtual device
static uint8_t gpio_relay1 = 13;
static uint8_t gpio_relay2 = 12;
static uint8_t gpio_relay3 = 14;
static uint8_t gpio_relay4 = 27;

/* Variable for reading pin status*/
bool switch_state_ch1 = false;
bool switch_state_ch2 = false;
bool switch_state_ch3 = false;
bool switch_state_ch4 = false;

// GPIO for link status LED
static uint8_t gpio_led = 2;

struct LightSwitch {
    const uint8_t pin;
    bool pressed;
    bool old;
};

// Define the light switches for channel 1 and 2
LightSwitch switch_ch1 = {gpio_switch1, false, false};
LightSwitch switch_ch2 = {gpio_switch2, false, false};
LightSwitch switch_ch3 = {gpio_switch3, false, false};
LightSwitch switch_ch4 = {gpio_switch4, false, false};

static Switch *my_switch1 = NULL;
static Switch *my_switch2 = NULL;
static Switch *my_switch3 = NULL;
static Switch *my_switch4 = NULL;

//static TemperatureSensor temperature("Temperature");
//static TemperatureSensor humidity("Humidity");

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
        digitalWrite(gpio_led, true);
        break;
    case ARDUINO_EVENT_PROV_INIT:
        wifi_prov_mgr_disable_auto_stop(10000);
        break;
    case ARDUINO_EVENT_PROV_CRED_SUCCESS:
        wifi_prov_mgr_stop_provisioning();
        break;
    default:;
    }
}

void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx){
    const char *device_name = device->getDeviceName();
    const char *param_name = param->getParamName();

    if (strcmp(device_name, "Switch_ch1") == 0) {
        Serial.printf("Lightbulb = %s\n", val.val.b ? "true" : "false");
        if (strcmp(param_name, "Power") == 0) {
            Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
            switch_state_ch1 = val.val.b;
            (switch_state_ch1 == false) ? digitalWrite(gpio_relay1, LOW) : digitalWrite(gpio_relay1, HIGH);
            param->updateAndReport(val);
        }
    }

    if (strcmp(device_name, "Switch_ch2") == 0) {
        Serial.printf("Switch value = %s\n", val.val.b ? "true" : "false");
        if (strcmp(param_name, "Power") == 0) {
            Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
            switch_state_ch2 = val.val.b;
            (switch_state_ch2 == false) ? digitalWrite(gpio_relay2, LOW) : digitalWrite(gpio_relay2, HIGH);
            param->updateAndReport(val);
        }
    }

    if (strcmp(device_name, "Switch_ch3") == 0) {
        Serial.printf("Switch value = %s\n", val.val.b ? "true" : "false");
        if (strcmp(param_name, "Power") == 0) {
            Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
            switch_state_ch3 = val.val.b;
            (switch_state_ch3 == false) ? digitalWrite(gpio_relay3, LOW) : digitalWrite(gpio_relay3, HIGH);
            param->updateAndReport(val);
        }
    }

    if (strcmp(device_name, "Switch_ch4") == 0) {
        Serial.printf("Switch value = %s\n", val.val.b ? "true" : "false");
        if (strcmp(param_name, "Power") == 0) {
            Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
            switch_state_ch4 = val.val.b;
            (switch_state_ch4 == false) ? digitalWrite(gpio_relay4, LOW) : digitalWrite(gpio_relay4, HIGH);
            param->updateAndReport(val);
        }
    }

}

/* bool read_btn(LightSwitch &sw_ch, bool switch_state){
  sw_ch.pressed = !digitalRead(sw_ch.pin);
  if(!sw_ch.pressed && sw_ch.old){
    switch_state = !switch_state;
    Serial.printf("Pin %i %s.\n",sw_ch.pin, switch_state ? "Ligado":"Desligado");
    digitalWrite(gpio_relay, switch_state);
  }
  sw_ch.old = sw_ch.pressed;
  return switch_state;
} */

void setup(){

    uint32_t chipId = 0;

    Serial.begin(115200);

    // Configure the input GPIOs
    pinMode(gpio_reset, INPUT);
    pinMode(gpio_switch1, INPUT_PULLUP);
    pinMode(gpio_switch2, INPUT_PULLUP);
    pinMode(gpio_switch3, INPUT_PULLUP);
    pinMode(gpio_switch4, INPUT_PULLUP);

    // Set the Relays GPIOs as output mode
    pinMode(gpio_relay1, OUTPUT);
    pinMode(gpio_relay2, OUTPUT);
    pinMode(gpio_relay3, OUTPUT);
    pinMode(gpio_relay4, OUTPUT);
    pinMode(gpio_led, OUTPUT);

    // Write to the GPIOs the default state on booting
    digitalWrite(gpio_relay1, DEFAULT_POWER_MODE);
    digitalWrite(gpio_relay2, DEFAULT_POWER_MODE);
    digitalWrite(gpio_relay3, DEFAULT_POWER_MODE);
    digitalWrite(gpio_relay4, DEFAULT_POWER_MODE);
    digitalWrite(gpio_led, false);

    Node my_node;
    my_node = RMaker.initNode("Sonoff Dual R3");

    //Initialize switch device
    my_switch1 = new Switch("Switch_ch1", &gpio_relay1);
    my_switch2 = new Switch("Switch_ch2", &gpio_relay2);
    my_switch3 = new Switch("Switch_ch3", &gpio_relay3);
    my_switch4 = new Switch("Switch_ch4", &gpio_relay4);

    if (!my_switch1 || !my_switch2 || !my_switch3 || !my_switch4) {
        return;
    }
    my_switch1->addCb(write_callback);
    my_switch2->addCb(write_callback);
    my_switch3->addCb(write_callback);
    my_switch4->addCb(write_callback);

    //  my_node.addDevice(temperature);
    //  my_node.addDevice(humidity);

    my_node.addDevice(*my_switch1);
    my_node.addDevice(*my_switch2);
    my_node.addDevice(*my_switch3);
    my_node.addDevice(*my_switch4);

    RMaker.enableOTA(OTA_USING_TOPICS);
    RMaker.enableTZService();
    RMaker.enableSchedule();
    RMaker.enableScenes();

    //Service Name
    for (int i = 0; i < 17; i = i + 8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }

    Serial.printf("\nChip ID:  %d Service Name: %s\n", chipId, service_name);

    Serial.printf("\nStarting ESP-RainMaker\n");
    RMaker.start();

    WiFi.onEvent(sysProvEvent);
  #if CONFIG_IDF_TARGET_ESP32
      WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
  #else
      WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
  #endif
}

void loop(){

  // switch_state_ch1 = read_btn(switch_ch1, switch_state_ch1);
  // switch_state_ch2 = read_btn(switch_ch2, switch_state_ch2);
  // switch_state_ch3 = read_btn(switch_ch3, switch_state_ch3);
  // switch_state_ch4 = read_btn(switch_ch4, switch_state_ch4);

  // temperature.updateAndReportParam("Temperature", t);
  // humidity.updateAndReportParam("Temperature", h);


  switch_ch1.pressed = !digitalRead(switch_ch1.pin);
  if(!switch_ch1.pressed && switch_ch1.old){
    switch_state_ch1 = !switch_state_ch1;
    Serial.printf("Pin %i %s.\n",switch_ch1.pin, switch_state_ch1 ? "Ligado":"Desligado");
    if (my_switch1) my_switch1->updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch1);
    (switch_state_ch1 == false) ? digitalWrite(gpio_relay1, LOW) : digitalWrite(gpio_relay1, HIGH);
  }
  switch_ch1.old = switch_ch1.pressed;

  switch_ch2.pressed = !digitalRead(switch_ch2.pin);
  if(!switch_ch2.pressed && switch_ch2.old){
    switch_state_ch2 = !switch_state_ch2;
    Serial.printf("Pin %i %s.\n",switch_ch2.pin, switch_state_ch2 ? "Ligado":"Desligado");
    if (my_switch2) my_switch2->updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch2);
    (switch_state_ch2 == false) ? digitalWrite(gpio_relay2, LOW) : digitalWrite(gpio_relay2, HIGH);
  }
  switch_ch2.old = switch_ch2.pressed;

  switch_ch3.pressed = !digitalRead(switch_ch3.pin);
  if(!switch_ch3.pressed && switch_ch3.old){
    switch_state_ch3 = !switch_state_ch3;
    Serial.printf("Pin %i %s.\n",switch_ch3.pin, switch_state_ch3 ? "Ligado":"Desligado");
    if (my_switch3) my_switch3->updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch3);
    (switch_state_ch3 == false) ? digitalWrite(gpio_relay3, LOW) : digitalWrite(gpio_relay3, HIGH);
  }
  switch_ch3.old = switch_ch3.pressed;

  switch_ch4.pressed = !digitalRead(switch_ch4.pin);
  if(!switch_ch4.pressed && switch_ch4.old){
    switch_state_ch4 = !switch_state_ch4;
    Serial.printf("Pin %i %s.\n",switch_ch4.pin, switch_state_ch4 ? "Ligado":"Desligado");
    if (my_switch4) my_switch4->updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch4);
    (switch_state_ch4 == false) ? digitalWrite(gpio_relay4, LOW) : digitalWrite(gpio_relay4, HIGH);
  }
  switch_ch4.old = switch_ch4.pressed;


  // Read GPIO0 (external button to reset device
  if (digitalRead(gpio_reset) == LOW) { //Push button pressed
      Serial.printf("Reset Button Pressed!\n");
      // Key debounce handling
      delay(100);
      int startTime = millis();
      while (digitalRead(gpio_reset) == LOW) {
          delay(50);
      }
      int endTime = millis();

      if ((endTime - startTime) > 10000) {
          // If key pressed for more than 10secs, reset all
          Serial.printf("Reset to factory.\n");
          RMakerFactoryReset(2);
      } else if ((endTime - startTime) > 3000) {
          Serial.printf("Reset Wi-Fi.\n");
          // If key pressed for more than 3secs, but less than 10, reset Wi-Fi
          RMakerWiFiReset(2);
      }
  }
  delay(100);
}
