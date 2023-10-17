#ifndef Sensor_h
#define Sensor_h



#include <Arduino.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_adc_cal.h"
#include "inttypes.h"
#include "stdint.h"

#define alpha 0.01

class Sensor {
public:

  // Constructor para la inicialización de variables
  Sensor();
  

  // Función de inicialización
  void iniciar();

  // Función para leer el valor analógico del sensor
  int values();

  // Función para leer el valor analógico del sensor
  int reading();
  // Función para leer el valor analógico del sensor
  float readVoltage();
  
  float unadjusted_voltage();

private:
  // Variables para almacenar datos
  int  value;
  float  voltage;
  long samples;
  int adc;

  int adc_filtrado = 0;
  int adc_raw = 0;
  //float alpha = 0.1;
  
};

#endif