#include <Arduino.h>

int nRelays = 4;

struct Relay
{
    uint8_t PIN;
    String NAME;
    String DESCRIPTION;
    int MODE;
};

Relay relays[4];

// -------------------------------------------------------------------
// SETUP DEL PIN RELAY
// -------------------------------------------------------------------
void setupPinRelay()
{

    // define structur of relays
    for (int i = 0; i < nRelays; i++)
    {
        relays[i] = {relay_pin[i], name_sensor_do[i], "Descrption not using", OUTPUT};
        pinMode(relays->PIN, relays->MODE);

        if (activate_do_sensor[i] == true)
        {
            setOnSingle(relays[i].PIN);
        }
        else
        {
            setOffSingle(relays[i].PIN);
        }
    }

    log("[ INFO ] Salida a Relay inicializada");
}
// -------------------------------------------------------------------
// CHECK AND SET RELYAS ON DEVICE RESTART
// -------------------------------------------------------------------
void chackRelaysAfterRestart()
{
    // Check if some relays need to be enabled after restart ESP32
    for (int i = 0; i < nRelays; i++)
    {
    }
}
// -------------------------------------------------------------------
// CONTROL DE LOS RELAYS DESDE MQTT & WS
// -------------------------------------------------------------------
void OnOffRelays(String commnad)
{

    DynamicJsonDocument JsonDoc(1024);

    deserializeJson(JsonDoc, commnad);

    int index = JsonDoc["relay"].as<int>();

    log("[ INFO ]Index  => " + String(index));
    
    if (JsonDoc["protocol"] == "WS")
    {
        log("[ INFO ] Comando por WS => " + commnad);
    }
    else
    {
        log("[ INFO ] Comando por MQTT => " + commnad);
    }

    if (JsonDoc["value"])
    {
        setOnSingle(relays[index].PIN);
        activate_do_sensor[index] = true;
    }
    else
    {
        setOffSingle(relays[index].PIN);
        activate_do_sensor[index] = false;
    }

    // Save status in memery spiffs
    settingsSave();
}