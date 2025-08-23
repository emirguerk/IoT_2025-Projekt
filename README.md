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
- Kabel, Widerstände und Steckbrett  

### Anschluss

1. Touchsensor → Eingangspin Microcontroller  
2. Button → Eingangspin Microcontroller  
3. LED → Ausgangspin mit Widerstand  
4. Wärmepad → Ausgang über Relais/Transistor  
5. Microcontroller → Stromversorgung  

---

## Funktionsweise

1. Nutzer drückt **Button**, um zu signalisieren, dass er kommunizieren möchte.  
2. **LED** beim Gegenüber leuchtet auf.  
3. Beide Nutzer berühren den **Touchsensor**, woraufhin sich das **Wärmepad** bei beiden Endpunkten aktiviert.  
4. Die Signale werden über **MQTT** ausgetauscht, sodass die Kommunikation über das Internet erfolgt.  

---

## Software & Installation

1. Microcontroller mit der bereitgestellten Firmware programmieren  
2. WiFiManager starten, um **WLAN-Zugang** zu konfigurieren  
3. MQTT-Broker einstellen (lokal oder Cloud)  
4. Endpunkte verbinden und testen: Button → LED, Touch → Wärmepad  

---

## Nutzung

- Drücke den **Button**, um Kontakt anzufordern  
- Berühre den **Touchsensor**, um Wärme zu aktivieren  
- Sieh die **LED** beim Gegenüber aufleuchten und fühle die Wärme am Pad  
