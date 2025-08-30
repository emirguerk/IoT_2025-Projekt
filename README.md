# VREUNDE – DIY Wärmearmband

Dieses Projekt beschreibt das **VREUNDE-Wärmearmband**, ein System, das Nähe und Verbundenheit über Distanz vermittelt. Berührung und Wärme werden zwischen zwei Endpunkten über **MQTT** übertragen, und die Geräte können über **WiFiManager** einfach mit dem Internet verbunden werden.

---

## Features

- **Touchsensor**: erkennt Berührung  
- **Button**: signalisiert Kommunikationswunsch  
- **LED**: zeigt beim Gegenüber, dass jemand kommunizieren möchte  
- **Wärmepad**: erwärmt sich bei gegenseitiger Berührung  
- **MQTT**: verbindet die Endpunkte über das Internet  
- **WiFiManager**: ermöglicht einfache Konfiguration des WLAN-Zugangs  

---

## Aufbauanleitung

### Hardware-Komponenten

- 1x Microcontroller mit Wi-Fi (z. B. ESP32)  
- 1x Touchsensor  
- 1x Button  
- 1x LED  
- 1x Wärmepad (z. B. Peltier-Modul)  
- 1x Relais
- Kabel, Widerstände und Steckbrett  
- Stromquelle, 9V (nicht vom ESP!)

### Anschluss

- Touchsensor → Pin 34
- Button → Pin 18
- LED → Pin 26
- Relais → Pin 14
- Wärmesensor (DHT) → Pin 16
- Wärmepad an Relais und externer Stromquelle anschließen
- Relais mit externer Stromquelle verbinden
- Microcontroller an Stromversorgung anschließen
- Alle Pins können im Code geändert werden

---

## Funktionsweise

1. Nutzer drückt **Button**, um zu signalisieren, dass er kommunizieren möchte.  
2. **LED** beim Gegenüber und bei sich selbst blinkt.
3. Gegenüber bestätigt mit Druck auf **Button**.
4. **LED** leuchtet bei beiden durchgehend.
5. Nutzer berührt den **Touchsensor**, woraufhin sich das **Wärmepad** beim bGegenüber aktiviert.  
6. Kommunikation kann durch einen Nutzer durch Druck auf **Button** beendet werden.
7. Die Signale werden über **MQTT** ausgetauscht, sodass die Kommunikation über das Internet erfolgt.  

---

## Software & Installation

1. Code anpassen → mqtt_server, mqttTopic und localId konfigurieren, localId muss auf beiden Geräten unterschiedlich sein
2. Code auf ESP installieren
3. WiFiManager starten, um **WLAN-Zugang** zu konfigurieren  

---

## Nutzung

- Drücke den **Button**, um Kontakt anzufordern und zu akzeptieren
- Berühre den **Touchsensor**, um Wärme zu aktivieren  
- **LED** zeigt an, ob Kommunikation angefordert wurde (blinken) oder bereits besteht (leuchten)
