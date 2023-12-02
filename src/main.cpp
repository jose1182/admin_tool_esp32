#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPmDNS.h> 

//Libreria para el RCT de ESP32
#include <ESP32Time.h>
//Librería para NTP
#include <NTPClient.h>

// -------------------------------------------------------------------
// Archivos *.hpp - Fragmentar el Código
// -------------------------------------------------------------------
#include "vue32_header.hpp"
#include "vue32_functions.hpp"
#include "vue32_settings.hpp"
#include "vue32_wifi.hpp"
#include "vue32_mqtt.hpp"
#include "vue32_server.hpp"
#include "vue32_websockets.hpp"

// -------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------
void setup()
{
  Serial.begin(921600);
  setCpuFrequencyMhz(240);
  // Memory EEPROM init
  EEPROM.begin(256);
  EEPROM.get(Restart_Address, device_restart);
  device_restart++;
  // Guadar valor a la memoria
  EEPROM.put(Restart_Address, device_restart);
  EEPROM.commit();
  EEPROM.end();
  log("\n[ INFO ] Iniciando Setup");
  log("[ INFO ] Reinicios " + String(device_restart));
  log("[ INFO ] Setup corriendo en el Core " + String(xPortGetCoreID()));
  // Iniciar el SPIFFS
  if (!SPIFFS.begin(true))
  {
    log("[ ERROR ] Falló la inicialización del SPIFFS");
    while (true)
      ;
  }
  // Leer el Archivo settings.json
  if (!settingsRead())
  {
    settingsSave();
  }
  // Configuración de los LEDs
  settingPines();
  // Setup WIFI
  wifi_setup();
  //Setup TIME
  timeSetup();
  // Inicializacion del Servidor WEB
  InitServer();
  // Inicializamos el WS
  InitWebSockets();
  // Fin del Setup
  log("[ INFO ] Setup completado");
}

void loop()
{
  // put your main code here, to run repeatedly:
  // -------------------------------------------------------------------
  // WIFI
  // -------------------------------------------------------------------
  if (wifi_mode == WIFI_STA)
  {
    wifiLoop();
  }
  else if (wifi_mode == WIFI_AP)
  {
    wifiAPLoop();
  }
  // -------------------------------------------------------------------
  // MQTT
  // -------------------------------------------------------------------
  if (((WiFi.status() == WL_CONNECTED)) && (wifi_mode == WIFI_STA))
  {
    if (mqtt_server != 0)
    {
      // TODO: Función para el loop principal de MQTT
      mqttLoop();
      if (mqttClient.connected() && mqtt_time_send)
      {
        // TODO: Función para enviar JSON por MQTT
        //  Funcion para enviar JSON por MQTT
        if (millis() - lastMsg > mqtt_time_interval)
        {
          lastMsg = millis();
          mqtt_publish();
        }
      }
    }
  }
  // -------------------------------------------------------------------
  // Enviar JSON por WS cada 1s
  // -------------------------------------------------------------------
  if (millis() - lastWsSend > 1000)
  {
    lastWsSend = millis();
    WsMessage(getJsonIndex(), "", "");
  }
  // -------------------------------------------------------------------
  // RTC & NTP
  // -------------------------------------------------------------------
  if((WiFi.status() == WL_CONNECTED) && (wifi_mode == WIFI_STA)){
    ntpClient.update();    
  }
  if (millis() - lastTime > 1000){
    lastTime = millis();
    WsMessage(getSendJson(getDateTime(), "time"), "", "");
  }
}
