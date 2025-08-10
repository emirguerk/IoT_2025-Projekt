#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include "heater.h"

#define DHTTYPE DHT11

const int relayPin = 26;
const int dhtPin = 16;

// MQTT-Server
const char* mqtt_server = "broker.hivemq.com";

// Pins
const int buttonPin = 18;
const int touchPin = 34;
const int ledPin = 26;
const int ledPin2 = 33;

WiFiClient espClient;
PubSubClient client(espClient);

Heater heater(dhtPin, DHTTYPE, relayPin);

// --- MQTT Callback ---
void catchMessage(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.println(topic);

    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.print("Message: ");
    Serial.println(message);

    if (message == "BUTTON_PRESSED") {
        digitalWrite(ledPin, HIGH);
        delay(1000);
        digitalWrite(ledPin, LOW);
    } else if (message == "TOUCH_DETECTED") {
        digitalWrite(ledPin2, HIGH);
        delay(1000);
        digitalWrite(ledPin2, LOW);
    }
}

// --- MQTT Reconnect ---
void reconnect() {
    while (!client.connected()) {
        Serial.print("MQTT verbinden...");
        if (client.connect("emxrxan")) {
            Serial.println("Verbunden!");
            client.subscribe("IoT_Emotionsband");
        } else {
            Serial.print("Fehler, rc=");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

bool lastButtonState = LOW;
bool lastTouchState = LOW;

void setup() {
    Serial.begin(115200);

    pinMode(touchPin, INPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT);

    // --- WiFiManager Setup ---
    WiFiManager wm;
    wm.setConfigPortalBlocking(true); // blockiert bis WLAN konfiguriert
    wm.setConfigPortalTimeout(180);   // 3 Minuten Zeitfenster

    // Falls keine gespeicherten Daten
    if (!wm.autoConnect("ESP_Setup", "12345678")) {
        Serial.println("Keine Verbindung möglich → Neustart");
        ESP.restart();
    }

    Serial.println("Mit WLAN verbunden: " + WiFi.SSID());

    // MQTT-Setup
    client.setServer(mqtt_server, 1883);
    client.setCallback(catchMessage);

    reconnect();

    heater.begin();
    heater.set_threshold(3);
}

void loop() {
    if (client.connected()) {
        client.loop();
    } else {
        reconnect();
    }

    bool currentButtonState = digitalRead(buttonPin);
    bool currentTouchState = digitalRead(touchPin);

    if (currentButtonState == HIGH && lastButtonState == LOW) {
        client.publish("IoT_Emotionsband", "BUTTON_PRESSED");
    }

    if (currentTouchState == HIGH && lastTouchState == LOW) {
        client.publish("IoT_Emotionsband", "TOUCH_DETECTED");
    }

    lastButtonState = currentButtonState;
    lastTouchState = currentTouchState;
    heater.loop();
}
