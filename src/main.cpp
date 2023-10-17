#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <EEPROM.h>
#include <TimeLib.h>

// -------------------------------------------------------------------
// Archivos *.hpp - Fragmentar el Código
// -------------------------------------------------------------------
#include "vue32_header.hpp"
#include "vue32_functions.hpp"
#include "vue32_settings.hpp"
#include "vue32_wifi.hpp"
#include "vue32_mqtt.hpp"
#include "vue32_server.hpp"
#include "Sensor.h"

//Only for example, create a class o function to use
#include <ADS1X15.h>

Sensor miSensor;
const float alfa = .80; // Low Pass Filter alpha (0.0 - 1.0 )
const float calCorrection = 0.1; // correct steady state error (ina260.readCurrent()
float voltsLowPassFilter = 0.0;
ADS1115 ADS(0x48);
uint16_t count = 0;
uint16_t value = 0;
float volts = 0;
uint16_t prev  = 0;
uint32_t lastTime = 0;
uint32_t lastSample = 0;
boolean flag = true;
long previousMillis = 0;
long On = 50; // tiempo que dura encendido, en milisegundos
long Off = 100; // tiempo que dura apagado, en milisegundos



int mySensorLast = 0;
int prev_temp = 0;
float i = 0;
// -------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------
void setup() {
  Serial.begin(921600);
  setCpuFrequencyMhz(240);
  // Memory EEPROM init
  EEPROM.begin(256);
  EEPROM.get(Restart_Address, device_restart);
  device_restart++;
  //Guadar valor a la memoria
  EEPROM.put(Restart_Address, device_restart);
  EEPROM.commit();
  EEPROM.end();
  log("\n[ INFO ] Iniciando Setup");
  log("[ INFO ] Reinicios " + String(device_restart)); 
  log("[ INFO ] Setup corriendo en el Core "+ String(xPortGetCoreID()));
    // Iniciar el SPIFFS
  if(!SPIFFS.begin(true)){
    log("[ ERROR ] Falló la inicialización del SPIFFS");
    while(true);
  }
    // Leer el Archivo settings.json
  if(!settingsRead()){
    settingsSave();
  }
    // Configuración de los LEDs
  settingPines();
  // Setup WIFI
  wifi_setup();
  // Inicializacion del Servidor WEB
  InitServer();

  miSensor.iniciar();


  //test con el ADS1115
  Serial.println(

  );
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);

  ADS.begin();
  ADS.setGain(2);      //  6.144 volt
  ADS.setDataRate(7);  //  0 = slow   4 = medium   7 = fast
  //ADS.setMode(0);      //  continuous mode
  //ADS.readADC(0);      //  first read to trigger
}

void loop() {
  // put your main code here, to run repeatedly:
  // -------------------------------------------------------------------
// WIFI
// -------------------------------------------------------------------
  if(wifi_mode == WIFI_STA){
    wifiLoop();
  }else if (wifi_mode == WIFI_AP){
    wifiAPLoop();
  }
  // -------------------------------------------------------------------
  // MQTT
  // -------------------------------------------------------------------
  if (((WiFi.status() == WL_CONNECTED)) && (wifi_mode == WIFI_STA))
  {
    if (mqtt_server != 0)
    {
      //TODO: Función para el loop principal de MQTT
      mqttLoop();
      if (mqttClient.connected() && mqtt_time_send)
      {
        //TODO: Función para enviar JSON por MQTT
        // Funcion para enviar JSON por MQTT
          if(millis() - lastMsg > mqtt_time_interval){
            lastMsg = millis();
            mqtt_publish();
          }
      }
      
    }
    
  }
  if (!analog_read_sensor)
  {
    if (millis() - mySensorLast >100)
    {
      mySensorLast = millis();
      // int valorAnalogico = miSensor.leerValorAnalogico();
      // int dif = valorAnalogico - prev_temp;

    //int reading = miSensor.reading();
    //float adjusted_voltage = miSensor.readVoltage();
    //float unadjusted_voltage = miSensor.unadjusted_voltage();

      
      // if(reading < 0){
      //   reading*=-1;
      // }


      //Variación de corriente en mA entre 4mA y 20mA.
      //20mA - 4mA = 16mA
      //Variación de voltaje en V entre 0,5V y 3V.
      //3V - 0,5V = 2,5
      //Relación entre el cambio de corriente y el cambio de voltaje
      //Dividir dividir la variación de voltaje (2,5V) entre la variación de corriente (16mA)
      //Relación = 2,5V / 16mA ≈ 0,15625 V/mA
      //corriente = corriente mA *Relación(V7/mA)
      //voltage = 12mA * 0,15625V/mA ≈ 1,875V -> encontrar valor más proximo -> 12mA corriente de entrada
      //corriente = Voltage(V)/Relación(V7/mA)

      int reading = miSensor.reading();
      float voltage = miSensor.readVoltage();


      float variacionVC = 0.2525;  //V/mA   
      // float fk = 0.2; // Factor corrección de visualización
      float current = miSensor.readVoltage()  / variacionVC; // Corriente a partir del factor de correción
      float x  = (current - 4.0)/(16/10); // Presión en Bares. 1000Mpa -> 10 Bar
      float y  = voltage/variacionVC; // mA

      Serial.print(">Reading:");
      Serial.println(reading);       
      Serial.print(">adjusted_voltage:");
      Serial.println(voltage); 
      Serial.print(">bares:");
      Serial.println(x); 
      Serial.print(">mA:");
      Serial.println(y); 
      // log("[ INFO ] Presión " + String(x) + "Bares"); 
      // log("[ INFO ] Current " + String(y) + "mA"); 

      // if (x >= 2.0 - 1.0 && x <= 2.0 + 1.0)
      // {
      //   log("[ INFO ] Cerrar valcula"); 
      //   setOffSingle(SENSORLED); 
      //   digitalWrite(SELENOIDE, LOW); 
      // }

      // if(x < 1.80 ){
      // // Poner en ON el Led de la activación de la valvula
      //   log("[ INFO ] led ON"); 
      //   setOnSingle(SENSORLED); 
      //   log("[ INFO ] Abrir valvula"); 
      //   digitalWrite(SELENOIDE, HIGH); 
      // }

      //prev_temp = valorAnalogico;
    }
    
  }


  if (millis() - lastSample >= 100)   //  almost exact 860 SPS
  {
    lastSample = millis();
    value = ADS.readADC();
    volts = ADS.toVoltage(1);
    voltsLowPassFilter = (alfa * value) + ((1.0 - alfa) * voltsLowPassFilter);// Low Pass Filter

    // Serial.print("\tAnalog: "); Serial.print(value); Serial.print('\t'); Serial.println(value * volts, 3);
    // Serial.print("Voltage: ");
    // Serial.println((value * 0.125) / 1000, 3);
    // Serial.print("Voltage Corrected: ");
    // Serial.println((currentVal * 0.125) / 1000, 3);
    
    float v1 = value * volts;
    float v2 = (voltsLowPassFilter * 0.0625) / 1000 *  0.9917181818181818;
    
    // Serial.print(">Reading:");    
    // Serial.println(value);      
    // Serial.print(">V:");
    // Serial.println(v1);      
    // Serial.print(">Vc:");
    // Serial.println(v2);    




    //y = mX + b
    //m= 6.5384939
    //b= -2.381057355

    float psi1 =   (6.5384939 * (v1)) + (-2.381057355);
    float psi2 =   ((6.5384939 * (v2)) + (-2.381057355)) * 0.96216216;
    // Serial.print(">Psi1:");
    // Serial.println(psi1, 3);   
    // Serial.print(">Psi2:");
    // Serial.println(psi2, 3);       

    if (psi2 >= 2.0)
    {
      log("[ INFO ] Presión " + String(psi2) + " Bares"); 
      log("[ INFO ] Valvula cerrada"); 
      setOffSingle(SENSORLED);
      vTaskDelay(200); 
      digitalWrite(SELENOIDE, LOW); 
    } else if (psi2 < 2.0  && psi2 > 1.8) 
    {
      log("[ INFO ] Presión " + String(psi2) + " Bares"); 
      log("[ INFO ] Presión esta diminuyendo..."); 
      setOffSingle(SENSORLED); 
      vTaskDelay(200);
      setOnSingle(SENSORLED); 
    } else 
    {
      log("[ INFO ] Presión " + String(psi2) + " Bares"); 
      log("[ INFO ] led ON"); 


      while (psi2 <= 2.05)
      {
        setOnSingle(SENSORLED); 
        log("[ INFO ] Valvula abierta..."); 
        digitalWrite(SELENOIDE, HIGH); 
        v2 = (voltsLowPassFilter * 0.0625) / 1000 *  0.9917181818181818;
        psi2 =   ((6.5384939 * (v2)) + (-2.381057355)) * 0.96216216;
      }
      

    }

     
  }

  
}
