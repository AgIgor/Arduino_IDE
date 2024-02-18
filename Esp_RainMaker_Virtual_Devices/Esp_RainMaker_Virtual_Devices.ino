#include "RMaker.h"
#include "WiFi.h"
#include "WiFiClient.h"
#include "WiFiProv.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define GPIO_LED 2
#define GPIO_RESET 0
#define DEFAULT_POWER_MODE false
const char *service_name = "VIRTUAL_DEVICES";
const char *pop = "123456";
const char* mqtt_server = "mqtt.eclipseprojects.io";

char* subTopic = "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/virtual_devices/#";
#define SUB_TOPICS_SIZE 4
char* pubTopic[SUB_TOPICS_SIZE] = {
                                    "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/virtual_devices/virtual_switch_1",
                                    "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/virtual_devices/virtual_switch_2",
                                    "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/virtual_devices/virtual_switch_3",
                                    "ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/mqtt/virtual_devices/virtual_switch_4"
                                  };


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
  
  // if (strcmp(topic, subTopic[0]) == 0) value == "true" ? switch_ch1_pressed(true) : switch_ch1_pressed(false);
  // if (strcmp(topic, subTopic[1]) == 0) value == "true" ? switch_ch2_pressed(true) : switch_ch2_pressed(false);
  // if (strcmp(topic, subTopic[2]) == 0) value == "true" ? switch_ch3_pressed(true) : switch_ch3_pressed(false);
  // if (strcmp(topic, subTopic[3]) == 0) value == "true" ? switch_ch4_pressed(true) : switch_ch4_pressed(false);

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...\n");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      client.subscribe(subTopic);
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
        digitalWrite(GPIO_LED, true);
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

    if (strcmp(device_name, "Virtual_Switch_1") == 0) {
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          //switch_state_ch1 = val.val.b;
          //client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch1", (switch_state_ch1 == false) ? "false" : "true");
          param->updateAndReport(val);
      }
    }
    else if (strcmp(device_name, "Virtual_Switch_2") == 0) {
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          //switch_state_ch2 = val.val.b;
          //client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch2", (switch_state_ch2 == false) ? "false" : "true");
          param->updateAndReport(val);
      }
    }
    else if (strcmp(device_name, "Virtual_Switch_3") == 0) {
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          //switch_state_ch3 = val.val.b;
          //client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch3", (switch_state_ch3 == false) ? "false" : "true");
          param->updateAndReport(val);
      }
    }
    else if (strcmp(device_name, "Virtual_Switch_4") == 0) {
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          //switch_state_ch4 = val.val.b;
          //client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/switch_state_ch4", (switch_state_ch4 == false) ? "false" : "true");
          param->updateAndReport(val);
      }
    }
    else if (strcmp(device_name, "Virtual_Light_Bulb") == 0) {
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          //light_bulb_state = val.val.b;
          //client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/light_bulb_state", val.val.b ? "true" : "false");
          param->updateAndReport(val);
      }
    }
    else if (strcmp(device_name, "Virtual_Dimmer") == 0) {
      if (strcmp(param_name, "Power") == 0) {
          Serial.printf("%s - %s = %s\n", device_name, param_name, val.val.b ? "true" : "false");
          //dimmer_state = val.val.b;
          //client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/dimmer/dimmer_state", val.val.b ? "true" : "false");
          param->updateAndReport(val);
      } else if (strcmp(param_name, "Brilho") == 0) {
          Serial.printf("%s - %s = %d\n", device_name, param_name,  val.val.i);
          char cstr[sizeof(val.val.i)];
          itoa(val.val.i, cstr, 10);
          //client.publish("ESP32-D0WDQ6-V3/17596493859364/PROV_SONOFF_DUALR3/app/dimmer/brilho_value", cstr);
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

void setup(){
  uint32_t chipId = 0;
  Serial.begin(115200);
  pinMode(GPIO_RESET, INPUT_PULLUP);
  pinMode(GPIO_LED, OUTPUT);

  Node my_node;
  my_node = RMaker.initNode(service_name);

  my_switch1 = new Switch("Virtual_Switch_1", NULL, false);
  my_switch2 = new Switch("Virtual_Switch_2", NULL, false);
  my_switch3 = new Switch("Virtual_Switch_3", NULL, false);
  my_switch4 = new Switch("Virtual_Switch_4", NULL, false);
  // my_light_bulb = new LightBulb("Virtual_Light_Bulb", NULL, false);

  // my_dimmer = new Device("Virtual_Dimmer", "esp.device.light", NULL);
  // my_dimmer = new Device("Virtual_Dimmer", "custom.device.dimmer", NULL);

  if (!my_switch1 || !my_switch2 || !my_switch3 || !my_switch4 /* || !my_dimmer || !my_light_bulb */) {
      return;
  }

  // my_dimmer->addNameParam();
  // my_dimmer->addPowerParam(DEFAULT_POWER_MODE);
  // my_dimmer->assignPrimaryParam(my_dimmer->getParamByName(ESP_RMAKER_DEF_POWER_NAME));

  // Param level_param("Brilho", "esp.param.brightness", value(0), PROP_FLAG_READ | PROP_FLAG_WRITE);//custom.param.level
  // level_param.addBounds(value(0), value(100), value(1));
  // level_param.addUIType(ESP_RMAKER_UI_SLIDER);
  // my_dimmer->addParam(level_param);

  my_switch1->addCb(write_callback);
  my_switch2->addCb(write_callback);
  my_switch3->addCb(write_callback);
  my_switch4->addCb(write_callback);
  // my_dimmer->addCb(write_callback);
  // my_light_bulb->addCb(write_callback);

  my_node.addDevice(*my_switch1);
  my_node.addDevice(*my_switch2);
  my_node.addDevice(*my_switch3);
  my_node.addDevice(*my_switch4);
  // my_node.addDevice(*my_dimmer);
  // my_node.addDevice(*my_light_bulb);

  RMaker.enableOTA(OTA_USING_TOPICS);
  RMaker.enableTZService();
  RMaker.enableSchedule();
  RMaker.enableScenes();

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

  randomSeed(micros());
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop(){

  if (!client.connected()) reconnect();
  client.loop();
  // Read GPIO0 (external button to reset device
  if (digitalRead(GPIO_RESET) == LOW) { //Push button pressed
      Serial.printf("Reset Button Pressed!\n");
      // Key debounce handling
      delay(100);
      int startTime = millis();
      while (digitalRead(GPIO_RESET) == LOW) {
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
