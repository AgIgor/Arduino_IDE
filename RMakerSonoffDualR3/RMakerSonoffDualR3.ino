#include "RMaker.h"
#include "WiFi.h"
#include "WiFiClient.h"
#include "WiFiProv.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define DEFAULT_POWER_MODE false
const char *service_name = "PROV_SONOFF_DUALR3";
const char *pop = "123456";
const char* mqtt_server = "mqtt.eclipseprojects.io";

#define SUB_TOPICS_SIZE 7
// char* pubTopic = "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3";
char* subTopic[SUB_TOPICS_SIZE] = {
                                    "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/switch_state_ch1",
                                    "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/switch_state_ch2",
                                    "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/switch_state_ch3",
                                    "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/switch_state_ch4",
                                    "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/light_bulb_state",
                                    "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/dimmer/dimmer_state",
                                    "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/dimmer/brilho_value"
                                  };

static uint8_t gpio_reset = 0;

static uint8_t gpio_switch1 = 26;
static uint8_t gpio_switch2 = 25;
static uint8_t gpio_switch3 = 32;
static uint8_t gpio_switch4 = 33;

static uint8_t gpio_relay1 = 13;
static uint8_t gpio_relay2 = 12;
static uint8_t gpio_relay3 = 14;
static uint8_t gpio_relay4 = 27;

static uint8_t gpio_led = 2;

bool switch_state_ch1 = false;
bool switch_state_ch2 = false;
bool switch_state_ch3 = false;
bool switch_state_ch4 = false;

bool light_bulb_state = false;
bool dimmer_state = false;


struct LightSwitch {
  const uint8_t pin;
  bool pressed;
  bool old;
};

LightSwitch switch_ch1 = {gpio_switch1, false, false};
LightSwitch switch_ch2 = {gpio_switch2, false, false};
LightSwitch switch_ch3 = {gpio_switch3, false, false};
LightSwitch switch_ch4 = {gpio_switch4, false, false};

static Switch *my_switch1 = NULL;
static Switch *my_switch2 = NULL;
static Switch *my_switch3 = NULL;
static Switch *my_switch4 = NULL;

static Device *my_dimmer = NULL;
static LightBulb *my_light_bulb = NULL;

// LightBulb
// Fan
// TemperatureSensor
// Switch
// Device

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print(topic);
  Serial.print(" ");

  String value;
  for (int i = 0; i < length; i++) {
    value += (char)payload[i];
  }
  Serial.println(value);
  
  if (strcmp(topic, subTopic[0]) == 0) value == "true" ? switch_ch1_pressed(true) : switch_ch1_pressed(false);
  if (strcmp(topic, subTopic[1]) == 0) value == "true" ? switch_ch2_pressed(true) : switch_ch2_pressed(false);
  if (strcmp(topic, subTopic[2]) == 0) value == "true" ? switch_ch3_pressed(true) : switch_ch3_pressed(false);
  if (strcmp(topic, subTopic[3]) == 0) value == "true" ? switch_ch4_pressed(true) : switch_ch4_pressed(false);

  // if (strcmp(topic, subTopic[4]) == 0) value == "true" ? NULL : NULL //light_bulb_state
  // if (strcmp(topic, subTopic[5]) == 0) value == "true" ? NULL : NULL //dimmer_state
  // if (strcmp(topic, subTopic[6]) == 0) value == "true" ? NULL : NULL //brilho_value

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...\n");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      client.subscribe("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/#");
      // for(byte i = 0; i < SUB_TOPICS_SIZE; i++){
      //   Serial.print("Sub: ");
      //   Serial.print(subTopic[i]);
      //   Serial.println(" connected");
      //   client.subscribe(subTopic[i]);
      // }
    } else {
      Serial.print(" failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

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
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          switch_state_ch1 = val.val.b;

          (switch_state_ch1 == false) ? digitalWrite(gpio_relay1, LOW) : digitalWrite(gpio_relay1, HIGH);
          client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch1", (switch_state_ch1 == false) ? "false" : "true");

          param->updateAndReport(val);
      }
    }
    else if (strcmp(device_name, "Switch_ch2") == 0) {
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          switch_state_ch2 = val.val.b;

          (switch_state_ch2 == false) ? digitalWrite(gpio_relay2, LOW) : digitalWrite(gpio_relay2, HIGH);
          client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch2", (switch_state_ch2 == false) ? "false" : "true");

          param->updateAndReport(val);
      }
    }
    else if (strcmp(device_name, "Switch_ch3") == 0) {
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          switch_state_ch3 = val.val.b;
          (switch_state_ch3 == false) ? digitalWrite(gpio_relay3, LOW) : digitalWrite(gpio_relay3, HIGH);
          client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch3", (switch_state_ch3 == false) ? "false" : "true");

          param->updateAndReport(val);
      }
    }
    else if (strcmp(device_name, "Switch_ch4") == 0) {
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          switch_state_ch4 = val.val.b;
          (switch_state_ch4 == false) ? digitalWrite(gpio_relay4, LOW) : digitalWrite(gpio_relay4, HIGH);
          client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch4", (switch_state_ch4 == false) ? "false" : "true");

          param->updateAndReport(val);
      }
    }
    else if (strcmp(device_name, "Light_Bulb") == 0) {
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          light_bulb_state = val.val.b;
          client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/light_bulb_state", val.val.b ? "true" : "false");
          param->updateAndReport(val);
      }
    }
    else if (strcmp(device_name, "Dimmer") == 0) {
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          dimmer_state = val.val.b;
          client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/dimmer/dimmer_state", val.val.b ? "true" : "false");
          param->updateAndReport(val);
      } else if (strcmp(param_name, "Brilho") == 0) {
          Serial.printf("%s - %s = %d\n", device_name, param_name,  val.val.i);

          char cstr[sizeof(val.val.i)];
          itoa(val.val.i, cstr, 10);

          client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/dimmer/brilho_value", cstr);
          param->updateAndReport(val);
      }
    }
    else {
      Serial.print("device_name: ");
      Serial.print(device_name);
      Serial.print(" val: ");
      Serial.println(val.val.b);
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

  pinMode(gpio_reset, INPUT);
  pinMode(gpio_switch1, INPUT_PULLUP);
  pinMode(gpio_switch2, INPUT_PULLUP);
  pinMode(gpio_switch3, INPUT_PULLUP);
  pinMode(gpio_switch4, INPUT_PULLUP);

  pinMode(gpio_relay1, OUTPUT);
  pinMode(gpio_relay2, OUTPUT);
  pinMode(gpio_relay3, OUTPUT);
  pinMode(gpio_relay4, OUTPUT);
  pinMode(gpio_led, OUTPUT);

  digitalWrite(gpio_relay1, DEFAULT_POWER_MODE);
  digitalWrite(gpio_relay2, DEFAULT_POWER_MODE);
  digitalWrite(gpio_relay3, DEFAULT_POWER_MODE);
  digitalWrite(gpio_relay4, DEFAULT_POWER_MODE);
  digitalWrite(gpio_led, false);

  Node my_node;
  my_node = RMaker.initNode("Sonoff Dual R3");

  my_switch1 = new Switch("Switch_ch1", NULL, false);
  my_switch2 = new Switch("Switch_ch2", NULL, false);
  my_switch3 = new Switch("Switch_ch3", NULL, false);
  my_switch4 = new Switch("Switch_ch4", NULL, false);
  my_light_bulb = new LightBulb("Light_Bulb", NULL, false);

  my_dimmer = new Device("Dimmer", "esp.device.light", NULL);
  // my_dimmer = new Device("Dimmer", "custom.device.dimmer", NULL);

  if (!my_switch1 || !my_switch2 || !my_switch3 || !my_switch4 || !my_dimmer || !my_light_bulb) {
      return;
  }

  my_dimmer->addNameParam();
  my_dimmer->addPowerParam(DEFAULT_POWER_MODE);
  my_dimmer->assignPrimaryParam(my_dimmer->getParamByName(ESP_RMAKER_DEF_POWER_NAME));

  Param level_param("Brilho", "esp.param.brightness", value(0), PROP_FLAG_READ | PROP_FLAG_WRITE);//custom.param.level
  level_param.addBounds(value(0), value(100), value(1));
  level_param.addUIType(ESP_RMAKER_UI_SLIDER);
  my_dimmer->addParam(level_param);



  my_dimmer->addCb(write_callback);
  my_switch1->addCb(write_callback);
  my_switch2->addCb(write_callback);
  my_switch3->addCb(write_callback);
  my_switch4->addCb(write_callback);
  my_light_bulb->addCb(write_callback);

  my_node.addDevice(*my_dimmer);
  my_node.addDevice(*my_switch1);
  my_node.addDevice(*my_switch2);
  my_node.addDevice(*my_switch3);
  my_node.addDevice(*my_switch4);
  my_node.addDevice(*my_light_bulb);

  RMaker.enableOTA(OTA_USING_TOPICS);
  RMaker.enableTZService();
  RMaker.enableSchedule();
  RMaker.enableScenes();

  //Service Name
  for (int i = 0; i < 17; i = i + 8) {
      chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  Serial.printf("\nChip ID:  %d Service Name: %s\n", chipId, service_name);

  //Serial.println("publisher/" + String(ESP.getChipModel()) + "/" + String(ESP.getEfuseMac())  + "/" +  String(service_name));
  // String str = "publisher/" + String(ESP.getChipModel()) + "/" + String(ESP.getEfuseMac())  + "/" +  String(service_name);
  // char chr[100];
  // str.toCharArray(chr, sizeof(chr));
  // pubTopic = chr;

  Serial.printf("\nStarting ESP-RainMaker\n");
  RMaker.start();

  WiFi.onEvent(sysProvEvent);
  #if CONFIG_IDF_TARGET_ESP32
      WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
  #else
      WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
  #endif

  // bool tesp_err_t esp_rmaker_mqtt_publish("teste/mqtt/rmaiker", "teste data", 20, 0, millis());
  // esp_err_t esp_rmaker_mqtt_publish(const char *topic, void *data, size_t data_len, uint8_t qos, int *msg_id)

  randomSeed(micros());
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void switch_ch1_pressed(bool switch_state_ch1){
  Serial.printf("Pin %i %s.\n",switch_ch1.pin, switch_state_ch1 ? "Ligado":"Desligado");
  if (my_switch1) my_switch1->updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch1);
  (switch_state_ch1 == false) ? digitalWrite(gpio_relay1, LOW) : digitalWrite(gpio_relay1, HIGH);
  client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch1", (switch_state_ch1 == false) ? "false" : "true");
}
void switch_ch2_pressed(bool switch_state_ch2){
  Serial.printf("Pin %i %s.\n",switch_ch2.pin, switch_state_ch2 ? "Ligado":"Desligado");
  if (my_switch2) my_switch2->updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch2);
  (switch_state_ch2 == false) ? digitalWrite(gpio_relay2, LOW) : digitalWrite(gpio_relay2, HIGH);
  client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch2", (switch_state_ch2 == false) ? "false" : "true");
}
void switch_ch3_pressed(bool switch_state_ch3){
  Serial.printf("Pin %i %s.\n",switch_ch3.pin, switch_state_ch3 ? "Ligado":"Desligado");
  if (my_switch3) my_switch3->updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch3);
  (switch_state_ch3 == false) ? digitalWrite(gpio_relay3, LOW) : digitalWrite(gpio_relay3, HIGH);
  client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch3", (switch_state_ch3 == false) ? "false" : "true");
}
void switch_ch4_pressed(bool switch_state_ch4){
  Serial.printf("Pin %i %s.\n",switch_ch4.pin, switch_state_ch4 ? "Ligado":"Desligado");
  if (my_switch4) my_switch4->updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, switch_state_ch4);
  (switch_state_ch4 == false) ? digitalWrite(gpio_relay4, LOW) : digitalWrite(gpio_relay4, HIGH);
  client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch4", (switch_state_ch4 == false) ? "false" : "true");
}

void loop(){

  if (!client.connected()) reconnect();
  client.loop();

  // temperature.updateAndReportParam("Temperature", t);
  // humidity.updateAndReportParam("Temperature", h);

  switch_ch1.pressed = !digitalRead(switch_ch1.pin);
  if(!switch_ch1.pressed && switch_ch1.old){
    switch_state_ch1 = !switch_state_ch1;
    switch_ch1_pressed(switch_state_ch1);
  }
  switch_ch1.old = switch_ch1.pressed;

  switch_ch2.pressed = !digitalRead(switch_ch2.pin);
  if(!switch_ch2.pressed && switch_ch2.old){
    switch_state_ch2 = !switch_state_ch2;
    switch_ch2_pressed(switch_state_ch2);
  }
  switch_ch2.old = switch_ch2.pressed;

  switch_ch3.pressed = !digitalRead(switch_ch3.pin);
  if(!switch_ch3.pressed && switch_ch3.old){
    switch_state_ch3 = !switch_state_ch3;
    switch_ch3_pressed(switch_state_ch3);
  }
  switch_ch3.old = switch_ch3.pressed;

  switch_ch4.pressed = !digitalRead(switch_ch4.pin);
  if(!switch_ch4.pressed && switch_ch4.old){
    switch_state_ch4 = !switch_state_ch4;
    switch_ch4_pressed(switch_state_ch4);
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
