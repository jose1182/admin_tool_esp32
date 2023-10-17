#include "Sensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "inttypes.h"
#include "stdint.h"
#include "esp_adc_cal.h"




#define ANALOG_SENSOR ADC1_CHANNEL_4

#define voltage_divider_offset 2.174


Sensor::Sensor() {
  // Inicializa las variables aquí
  value = 0;
  samples = 1;
  adc = 33;

}

void Sensor::iniciar() {
  // Realiza la inicialización del sensor aquí si es necesario
  //adc1_config_channel_atten(ANALOG_SENSOR, ADC_ATTEN_DB_11);
  //adc1_config_width(ADC_WIDTH_BIT_12);

}

int Sensor::values() {

  return  analogRead(adc);   

}


int Sensor::reading() {
  int smooth_val = 0;
  // //smooth_val = smooth_val + value
  //  for (int i = 0; i < samples; i++)
  //  {
  //   //smooth_val += value = adc1_get_raw(ANALOG_SENSOR);
  //   smooth_val += value = analogRead(adc);   
  //  }
  
  //  smooth_val /= samples;
  //  if (smooth_val > 4095)
  //  {
  //    smooth_val = 4095;
  // }
  adc_raw = analogRead(adc);
  adc_filtrado = (alpha*adc_raw) + ((1-alpha)*adc_filtrado);
  
  // Realiza la lectura del valor analógico del sensor y actualiza valorSensor
  //value = adc1_get_raw(ANALOG_SENSOR); // Cambia A0 al pin que estás utilizando
  return  adc_filtrado;   
;
}

float Sensor::readVoltage() {
  // Realiza la lectura del valor analógico del sensor y actualiza valorSensor
  // voltage = (this->reading() * (3.3 /4095.0))+0.0; // Cambia A0 al pin que estás utilizando
  // return voltage;
  float calibration  = 1.150; // Adjust for ultimate accuracy when input is measured using an accurate DVM, if reading too high then use e.g. 0.99, too low use 1.01
  float vref = 1100;
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  vref = adc_chars.vref; // Obtain the device ADC reference voltage
  return (unadjusted_voltage() * (1100 / vref) * calibration);  // ESP by design reference voltage in mV

}

float Sensor::unadjusted_voltage(){

  //return (reading() / 4095.0 * 3.3 * voltage_divider_offset);
  return (reading() / 4095.0 * 3.3);
}
