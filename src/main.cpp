#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include "heater.h"

#define DHTTYPE DHT11

const int relayPin = 14;
const int dhtPin = 16;

// MQTT-Server
const char* mqtt_server = "broker.hivemq.com";

// Pins
const int buttonPin = 18;
const int touchPin = 34;
const int ledPin = 26;

// MQTT Config
const char* mqttTopic = "IoT_Emotionsband";
const char localId = 1;

// Constants
const char requestCom = 1;
const char abortCom = 2;
const char touchStart = 3;
const char touchEnd = 4;

// State
int comStatus = 0; // 0: no com 1: com request send 2: comrequest received 3: com established
int comTimer = 0;
int ledTimer = 0;

WiFiClient espClient;
PubSubClient client(espClient);

Heater heater(dhtPin, DHTTYPE, relayPin);

// --- MQTT Callback ---
void catchMessage(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.println(topic);
    char sender = payload[0];
    char content = payload[1];

    if (sender == localId) {
        return;
    }

    Serial.printf("Message: %i\n", content);
    Serial.printf("Sender: %i\n", sender);

    if (content == requestCom) {
        if (comStatus == 0) {
            comStatus = 2;
            comTimer = 300;
        } else if (comStatus == 1) {
            comStatus = 3;
        }
    } else if (content == abortCom && comStatus != 1) {
        comStatus = 0;
        heater.deactivate();
    } else if (content == touchStart && comStatus == 3) {
        heater.activate(30);
    } else if (content == touchEnd) {
        heater.deactivate();
    }
}

// --- MQTT Reconnect ---
void reconnect() {
    while (!client.connected()) {
        Serial.print("MQTT verbinden...");
        if (client.connect("emxrxan")) {
            Serial.println("Verbunden!");
            client.subscribe(mqttTopic);
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
    Serial.begin(9600);

    pinMode(touchPin, INPUT);
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

void ledLoop() {
    if (comStatus == 0) {
        digitalWrite(ledPin, LOW);
    } else if (comStatus == 3) {
        digitalWrite(ledPin, HIGH);
    } else if (ledTimer > 0) {
        digitalWrite(ledPin, LOW);
    } else if (ledTimer < 0) {
        digitalWrite(ledPin, HIGH);
    }
    ledTimer--;
    if (ledTimer <= -10) {
        ledTimer = 10;
    }
}

void loop() {
    if (client.connected()) {
        client.loop();
    } else {
        reconnect();
    }

    bool currentButtonState = digitalRead(buttonPin);
    bool currentTouchState = digitalRead(touchPin);
    char message[3];
    message[0] = localId;
    message[2] = 0;

    if (currentButtonState == HIGH && lastButtonState == LOW) {
        if (comStatus == 3 || comStatus == 1) {
            message[1] = abortCom;
            comStatus = 0;
            heater.deactivate();
        } else if (comStatus == 0) {
            message[1] = requestCom;
            comStatus = 1;
            comTimer = 300;
        } else if (comStatus == 2) {
            message[1] = requestCom;
            comStatus = 3;
        }
        client.publish(mqttTopic, message);
    }

    if (currentTouchState == HIGH && lastTouchState == LOW) {
        message[1] = touchStart;
        client.publish(mqttTopic, message);
    }

    if (currentTouchState == LOW && lastTouchState == HIGH) {
        message[1] = touchEnd;
        client.publish(mqttTopic, message);
    }

    lastButtonState = currentButtonState;
    lastTouchState = currentTouchState;
    heater.loop();
    delay(100);
    ledLoop();
    if (comTimer > 0) {
        comTimer--;
    }
    if (comTimer == 0 && comStatus != 3 && comStatus != 0) {
        comStatus = 0;
        message[1] = abortCom;
        client.publish(mqttTopic, message);
    }
}

