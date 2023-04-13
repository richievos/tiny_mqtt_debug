#include <Arduino.h>
#include <TinyMqtt.h>

#include <memory>

/***********************
 * INPUTS
 ***********************/
const unsigned int OUTPUT_INTERVAL_MS = 1000;
const int MQTT_BROKER_PORT = 1883;

// const char *WIFI_SSID = "<FILL_ME_IN>";
// const char *WIFI_PASSWORD = "<FILL_ME_IN>";

namespace misc {
void connectWifi(const std::string hostname, const std::string wifiSSID, const std::string wifiPassword) {
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(hostname.c_str());
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());

    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }

    Serial.print("Connected ip=");
    Serial.print(WiFi.localIP());
    Serial.println();
}
}  // namespace misc

namespace mqtt {

std::shared_ptr<MqttBroker> mqttBroker = std::make_shared<MqttBroker>(MQTT_BROKER_PORT);

void setupMQTT(std::shared_ptr<MqttBroker> mqttBroker) {
    Serial.print("Starting MQTT broker");
    Serial.print("...");

    if (mqttBroker) mqttBroker->begin();

    Serial.println(" done");
}

void loopMQTT(std::shared_ptr<MqttBroker> mqttBroker) {
    if (mqttBroker) mqttBroker->loop();
}
}  // namespace mqtt

void setup() {
    Serial.begin(115200);
    misc::connectWifi("debug-mqtt", WIFI_SSID, WIFI_PASSWORD);
    mqtt::setupMQTT(mqtt::mqttBroker);
}

int lastIntervalValue = 0;
void loop() {
    mqtt::loopMQTT(mqtt::mqttBroker);

    const unsigned long time = millis();
    const int newIntervalValue = time / OUTPUT_INTERVAL_MS;

    if (newIntervalValue != lastIntervalValue) {
        Serial.print(time);

        Serial.print(" free_heap=");
        Serial.print(xPortGetFreeHeapSize());
        Serial.println();

        lastIntervalValue = newIntervalValue;
    }
}