// Only for example, create a class o function to use
#include <ADS1X15.h>

ADS1115 ADS(0x48);
const float alfa = .80;          // Low Pass Filter alpha (0.0 - 1.0 )
const float calCorrection = 0.1; // correct steady state error (ina260.readCurrent()
float output = 0.0;
uint16_t value = 0;
uint32_t lastSample = 0;
float pressure = 0;
unsigned long currentTimeOpenValve = 0;
unsigned long startTimeOpenValve = 0;
bool valveOpen = false; // Variable para rastrear el estado de la válvula
bool pressureIsLowing = false;
bool pressureNotReached = false;
unsigned long lastValveOpenTime = 0;
unsigned long currentTime = 0;

void openValve();
void closeValve();
float readPressure();

void isumotex_setup()
{
  ADS.begin();
  ADS.setGain(2);     //  6.144 volt
  ADS.setDataRate(7); //  0 = slow   4 = medium   7 = fast
  // ADS.setMode(0);         //  continuous mode
  //  ADS.readADC(0);      //  first read to trigger
  log("[ INFO ] Isumotex Setup inicializada");
}

//calculateOutput: cálcula el valor de salida con filtro de paso bajo.
 float calculateOutput(float alfa, float value, float previousOutput){
  return (alfa * value) + ((1.0 - alfa) * previousOutput); // Low Pass Filter
 }

//calculateVoltage: cálcula el voltage a partir del valor de salida.
float calculateVoltage(float output){
  return (output * 0.0625) / 1000 * 0.9917181818181818;
}

//calculatePressure: cálcula la presión a partir del voltage
float calculatePressure(float  voltage){
  float slope = 6.5384939;
  float intercept = -2.381057355;
  return ((slope * voltage) + intercept) * 0.96216216;
}

void printResults(float voltage, float pressure){
  log("\n[ INFO ] Voltage: " + String(voltage));
  log("\n[ INFO ] Pressure: " + String(pressure));

  Serial.print(">Voltage:");
  Serial.println(voltage);
  Serial.print(">Pressure:");
  Serial.println(pressure);
}


float readPressure()
{
  value = ADS.readADC();
  output = calculateOutput(alfa, value, output); // Low Pass Filter

  float voltage = calculateVoltage(output);
  float pressure = calculatePressure(voltage);

  printResults(voltage, pressure);

  return pressure;
}

void loopIsumotex()
{
  // Comprueba si la presión está bajando
  if (pressure > PRESSURE_THRESHOLD_LOW && pressure < PRESSURE_THRESHOLD_HIGH)
  {
    pressureIsLowing = true;
    // Control de la válvula si la presión es menor que el umbral
  }
  else if (pressure < PRESSURE_THRESHOLD_LOW && pressure > 1.6)
  {
    // Abre la válvula si la presión es muy baja
    openValve();
  }
  else if (pressure > PRESSURE_THRESHOLD_HIGH + PRESSURE_TOLERANCE)
  {
    // Cierra la válvula si la presión es alta
    closeValve();
    pressureIsLowing = false;
    pressureNotReached = false;
  }
  else if (pressure < THRESHOLD_WITHOUT_PRESSURE)
  {
    int inHomePosition = digitalRead(27);
    if (inHomePosition == HIGH)
    {
      // Abre la válvula si la presión es muy baja
      openValve();
    }
    else
    {
      log("\n[ INFO ] The robot is not en HOME position");
    }
  }

  // Verifica si la válvula se ha abierto durante un tiempo específico
  if (valveOpen)
  {
    currentTime = millis();
    if (currentTime - lastValveOpenTime >= VALVE_OPEN_DURATION)
    {
      lastValveOpenTime = currentTime;
      pressureNotReached = true;
    }
  }

  if (pressureNotReached)
  {
    log("\n[ ALERT ]  No fué posible alcanzar la presión obtima +/- 2 Bar en " + String(VALVE_OPEN_DURATION) + " ms");
  }
  else if (RETRY_OPEN_VALVE > 5)
  {
    log("\n[ WARNING ] El tubo tiene un posible defecto. Revise.");
  }

  if (!pressureIsLowing && !pressureNotReached)
  {
    log("\n[ INFO ] La presión está en el rango deseado." + String(pressure) + " Bar");
  }
  else if (pressureIsLowing)
  {
    log("\n[ ALERT ] ¡La presión está bajando." + String(pressure) + " Bar!");
  }
}

void openValve()
{
  if (!valveOpen)
  {
    // digitalWrite(SELENOIDE, HIGH);
    valveOpen = true;
    lastValveOpenTime = millis();
    log("\n[ INFO ] válvula esta abierta");
    startTimeOpenValve = millis();
  }
}

void closeValve()
{
  if (valveOpen)
  {
    // digitalWrite(SELENOIDE, LOW);
    valveOpen = false;
    log("\n[ INFO ] válvula esta cerrada");
    if (currentTimeOpenValve <= TIMEOUT_CHECK_PRESSURE)
    {
      currentTimeOpenValve += millis() - startTimeOpenValve;
      RETRY_OPEN_VALVE++;
    }
    else
    {
      currentTimeOpenValve = 0;
    }
  }
}