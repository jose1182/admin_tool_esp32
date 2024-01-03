
boolean settingsRead();
void settingsReset();
boolean settingsSave();

// -------------------------------------------------------------------
// Leer settings.json
// -------------------------------------------------------------------
boolean settingsRead()
{

    DynamicJsonDocument jsonSettings(capacitySettings);

    File file = SPIFFS.open("/settings.json", "r");

    if (deserializeJson(jsonSettings, file))
    {
        // Tomar los valores de Fábrica
        settingsReset();
        DeserializationError error = deserializeJson(jsonSettings, file);
        log("[ ERROR ] Falló la lectura de las configuraciones, tomando valores por defecto");
        if (error)
        {
            log("[ ERROR ] " + String(error.c_str()));
        }
        return false;
    }
    else
    {
        // -------------------------------------------------------------------
        // Dispositivo settings.json
        // -------------------------------------------------------------------
        device_config_file = jsonSettings["device_config_file"];
        strlcpy(device_config_serial, jsonSettings["device_config_serial"], sizeof(device_config_serial));
        strlcpy(device_id, jsonSettings["device_id"], sizeof(device_id));
        strlcpy(device_old_user, jsonSettings["device_old_user"], sizeof(device_old_user));
        strlcpy(device_old_password, jsonSettings["device_old_password"], sizeof(device_old_password));
        // -------------------------------------------------------------------
        // WIFI Cliente settings.json
        // -------------------------------------------------------------------
        wifi_ip_static = jsonSettings["wifi_ip_static"];
        strlcpy(wifi_ssid, jsonSettings["wifi_ssid"], sizeof(wifi_ssid));
        strlcpy(wifi_password, jsonSettings["wifi_password"], sizeof(wifi_password));
        strlcpy(wifi_ipv4, jsonSettings["wifi_ipv4"], sizeof(wifi_ipv4));
        strlcpy(wifi_subnet, jsonSettings["wifi_subnet"], sizeof(wifi_subnet));
        strlcpy(wifi_gateway, jsonSettings["wifi_gateway"], sizeof(wifi_gateway));
        strlcpy(wifi_dns_primary, jsonSettings["wifi_dns_primary"], sizeof(wifi_dns_primary));
        strlcpy(wifi_dns_secondary, jsonSettings["wifi_dns_secondary"], sizeof(wifi_dns_secondary));
        // -------------------------------------------------------------------
        // WIFI AP settings.json
        // -------------------------------------------------------------------
        ap_mode = jsonSettings["ap_mode"];
        strlcpy(ap_ssid, jsonSettings["ap_ssid"], sizeof(ap_ssid));
        strlcpy(ap_password, jsonSettings["ap_password"], sizeof(ap_password));
        ap_visibility = jsonSettings["ap_visibility"];
        ap_chanel = jsonSettings["ap_chanel"];
        ap_connect = jsonSettings["ap_connect"];
        // -------------------------------------------------------------------
        // Cloud settings.json
        // -------------------------------------------------------------------
        mqtt_cloud_enable = jsonSettings["mqtt_cloud_enable"];
        strlcpy(mqtt_user, jsonSettings["mqtt_user"], sizeof(mqtt_user));
        strlcpy(mqtt_password, jsonSettings["mqtt_password"], sizeof(mqtt_password));
        strlcpy(mqtt_server, jsonSettings["mqtt_server"], sizeof(mqtt_server));
        strlcpy(mqtt_cloud_id, jsonSettings["mqtt_cloud_id"], sizeof(mqtt_cloud_id));
        mqtt_port = jsonSettings["mqtt_port"];
        mqtt_retain = jsonSettings["mqtt_retain"];
        mqtt_qos = jsonSettings["mqtt_qos"];
        mqtt_time_send = jsonSettings["mqtt_time_send"];
        mqtt_time_interval = jsonSettings["mqtt_time_interval"];
        mqtt_status_send = jsonSettings["mqtt_status_send"];
        // -------------------------------------------------------------------
        // Time settings.json
        // -------------------------------------------------------------------
        time_ajuste = jsonSettings["time_ajuste"];
        strlcpy(time_date, jsonSettings["time_date"], sizeof(time_date));
        time_z_horaria = jsonSettings["time_z_horaria"];
        strlcpy(time_server, jsonSettings["time_server"], sizeof(time_server));
        // -------------------------------------------------------------------
        // iSumotex paramters settings.json
        // -------------------------------------------------------------------
        PRESSURE_TARGET = jsonSettings["PRESSURE_TARGET"];
        PRESSURE_THRESHOLD_HIGH = jsonSettings["PRESSURE_THRESHOLD_HIGH"];
        PRESSURE_THRESHOLD_LOW = jsonSettings["PRESSURE_THRESHOLD_LOW"];
        PRESSURE_TOLERANCE = jsonSettings["PRESSURE_TOLERANCE"];
        THRESHOLD_WITHOUT_PRESSURE = jsonSettings["THRESHOLD_WITHOUT_PRESSURE"];
        WARNING_LIMIT = jsonSettings["WARNING_LIMIT"].as<int>();
        MAX_VALVE_OPEN_COUNT = jsonSettings["MAX_VALVE_OPEN_COUNT"].as<int>();
        MONITORING_ENABLED = jsonSettings["MONITORING_ENABLED"].as<bool>();
        VALVE_OPEN_DURATION = jsonSettings["VALVE_OPEN_DURATION"].as<int>();
        TIMEOUT_CHECK_PRESSURE = jsonSettings["TIMEOUT_CHECK_PRESSURE"].as<int>();
        RETRY_OPEN_VALVE = jsonSettings["RETRY_OPEN_VALVE"].as<int>();
        // -------------------------------------------------------------------
        // iSumotex i/o settings.json
        // -------------------------------------------------------------------
        type_sensor_ai_0        = jsonSettings["io"]["analog"]["ai_0"][0];
        type_sensor_ai_1        = jsonSettings["io"]["analog"]["a2_0"][0];
        val_ai_sensor_0         = jsonSettings["io"]["analog"]["ai_0"][1];
        val_ai_sensor_1         = jsonSettings["io"]["analog"]["a2_0"][1];
        enable_ai_sensor_0      = jsonSettings["io"]["analog"]["ai_0"][2].as<bool>();
        enable_ai_sensor_1      = jsonSettings["io"]["analog"]["a2_0"][2].as<bool>();
        name_sensor_di_0        = jsonSettings["io"]["input"]["di_0"][0].as<String>();
        name_sensor_di_1        = jsonSettings["io"]["input"]["di_1"][0].as<String>();
        status_di_sensor_0      = jsonSettings["io"]["input"]["di_0"][1].as<bool>();
        status_di_sensor_1      = jsonSettings["io"]["input"]["di_1"][1].as<bool>();
        enable_di_sensor_0      = jsonSettings["io"]["input"]["di_0"][2].as<bool>();
        enable_di_sensor_1      = jsonSettings["io"]["input"]["di_1"][2].as<bool>();
        name_sensor_do[0]       = jsonSettings["io"]["relay"]["do_0"][0].as<String>();
        name_sensor_do[1]       = jsonSettings["io"]["relay"]["do_1"][0].as<String>();
        name_sensor_do[2]       = jsonSettings["io"]["relay"]["do_2"][0].as<String>();
        name_sensor_do[3]       = jsonSettings["io"]["relay"]["do_3"][0].as<String>();
        activate_do_sensor[0]   = jsonSettings["io"]["relay"]["do_0"][1].as<bool>();
        activate_do_sensor[1]   = jsonSettings["io"]["relay"]["do_1"][1].as<bool>();
        activate_do_sensor[2]   = jsonSettings["io"]["relay"]["do_2"][1].as<bool>();
        activate_do_sensor[3]   = jsonSettings["io"]["relay"]["do_3"][1].as<bool>();
        enable_do_sensor[0]     = jsonSettings["io"]["relay"]["do_0"][2].as<bool>();
        enable_do_sensor[1]     = jsonSettings["io"]["relay"]["do_1"][2].as<bool>();
        enable_do_sensor[2]     = jsonSettings["io"]["relay"]["do_2"][2].as<bool>();
        enable_do_sensor[3]     = jsonSettings["io"]["relay"]["do_3"][2].as<bool>();
        // -------------------------------------------------------------------
        // temporal settings.json
        // -------------------------------------------------------------------
        // analog_read_sensor = jsonSettings["analog_read_sensor"];

        file.close();
        log("[ INFO ] Lectura de las configuraciones correcta");
        return true;
    }
}
// -------------------------------------------------------------------
// Valores de Fábrica al settings.json
// -------------------------------------------------------------------
void settingsReset()
{
    // -------------------------------------------------------------------
    // Dispositivo settings.json
    // -------------------------------------------------------------------
    device_config_file = true;
    strlcpy(device_config_serial, deviceID().c_str(), sizeof(device_config_serial));
    strlcpy(device_id, "adminvue32", sizeof(device_id));
    strlcpy(device_old_user, "admin", sizeof(device_old_user));
    strlcpy(device_old_password, "admin", sizeof(device_old_password));
    // -------------------------------------------------------------------
    // WIFI Cliente settings.json
    // -------------------------------------------------------------------
    wifi_ip_static = false;
    // strlcpy(wifi_ssid, "MOVISTAR_E82F", sizeof(wifi_ssid));
    // strlcpy(wifi_password, "ED3A98227762FB3FEC7E", sizeof(wifi_password));
    strlcpy(wifi_ssid, "TP-Link_CCD4", sizeof(wifi_ssid));
    strlcpy(wifi_password, "66689850", sizeof(wifi_password));
    strlcpy(wifi_ipv4, "192.168.0.177", sizeof(wifi_ipv4));
    strlcpy(wifi_subnet, "255.255.255.0", sizeof(wifi_subnet));
    strlcpy(wifi_gateway, "192.168.0.1", sizeof(wifi_gateway));
    strlcpy(wifi_dns_primary, "8.8.8.8", sizeof(wifi_dns_primary));
    strlcpy(wifi_dns_secondary, "8.8.4.4", sizeof(wifi_dns_secondary));
    // -------------------------------------------------------------------
    // WIFI AP settings.json
    // -------------------------------------------------------------------
    ap_mode = false;
    strlcpy(ap_ssid, deviceID().c_str(), sizeof(ap_ssid));
    strlcpy(ap_password, "adminVUE32", sizeof(ap_password));
    ap_visibility = false;
    ap_chanel = 9;
    ap_connect = 4;
    // -------------------------------------------------------------------
    // Cloud settings.json
    // -------------------------------------------------------------------
    mqtt_cloud_enable = false;
    strlcpy(mqtt_user, "superuser", sizeof(mqtt_user));
    strlcpy(mqtt_password, "superpass", sizeof(mqtt_password));
    // strlcpy(mqtt_server, "192.168.1.136", sizeof(mqtt_server));
    strlcpy(mqtt_server, "192.168.0.148", sizeof(mqtt_server));
    strlcpy(mqtt_cloud_id, deviceID().c_str(), sizeof(mqtt_cloud_id));
    mqtt_port = 1883;
    mqtt_retain = false;
    mqtt_qos = 0;
    mqtt_time_send = true;
    mqtt_time_interval = 30000;
    mqtt_status_send = true;

    // -------------------------------------------------------------------
    // Time settings.json
    // -------------------------------------------------------------------
    time_ajuste = true;
    strlcpy(time_date, "2022-09-22T00:00", sizeof(time_date));
    time_z_horaria = -14400;
    strlcpy(time_server, "ntp.roa.es", sizeof(time_server));
    // -------------------------------------------------------------------
    // iSumotex settings.json
    // -------------------------------------------------------------------
    PRESSURE_TARGET = 2.0;            // Valor objetivo de presión en bares
    PRESSURE_THRESHOLD_HIGH = 2.0;    // Umbral de presión alta en bares
    PRESSURE_THRESHOLD_LOW = 1.8;     // Umbral de presión baja en bares
    PRESSURE_TOLERANCE = 0.1;         // Tolerancia de presión en bares
    THRESHOLD_WITHOUT_PRESSURE = 1.0; // Minima presion sin aire
    WARNING_LIMIT = 5;                // Límite de advertencias antes de considerar que la manguera es defectuosa
    MAX_VALVE_OPEN_COUNT = 5;
    MONITORING_ENABLED = false;
    VALVE_OPEN_DURATION = 10000;
    TIMEOUT_CHECK_PRESSURE = 30000;
    RETRY_OPEN_VALVE = 5;
    // -------------------------------------------------------------------
    // iSumotex i/o settings.json
    // -------------------------------------------------------------------
    // Variables Analog inputs
    type_sensor_ai_0 = 0;
    type_sensor_ai_1 = 0;

    val_ai_sensor_0 = 0.0;
    val_ai_sensor_1 = 0.0;
    enable_ai_sensor_0 = false;
    enable_ai_sensor_1 = false;
    // Variables Digital inputs
    name_sensor_di_0        = "Name tag sensor di_0";
    name_sensor_di_1        = "Name tag sensor di_1";
    status_di_sensor_0      = false;
    status_di_sensor_1      = false;
    enable_di_sensor_0      = false;
    enable_di_sensor_1      = false;
    // Variables Digital outputs
    name_sensor_do[0]          = "Name tag sensor d0_0";
    name_sensor_do[1]          = "Name tag sensor d0_1";
    name_sensor_do[2]          = "Name tag sensor d0_2";
    name_sensor_do[3]          = "Name tag sensor d0_3";
    activate_do_sensor[0]   = false;
    activate_do_sensor[1]   = false;
    activate_do_sensor[2]   = false;
    activate_do_sensor[3]   = false;
    enable_do_sensor[0]     = false;
    enable_do_sensor[1]     = false;
    enable_do_sensor[2]     = false;
    enable_do_sensor[3]     = false;
    
    log("[ INFO ] Se reiniciaron todos los valores por defecto");
    //
    analog_read_sensor = true;
}

// -------------------------------------------------------------------
// Guardar settings.json
// -------------------------------------------------------------------
boolean settingsSave()
{
    // StaticJsonDocument<capacitySettings> jsonSettings;
    DynamicJsonDocument jsonSettings(capacitySettings);

    File file = SPIFFS.open("/settings.json", "w+");

    if (file)
    {
        // -------------------------------------------------------------------
        // Dispositivo settings.json
        // -------------------------------------------------------------------
        jsonSettings["device_config_file"] = device_config_file;
        jsonSettings["device_config_serial"] = device_config_serial;
        jsonSettings["device_id"] = device_id;
        jsonSettings["device_old_user"] = device_old_user;
        jsonSettings["device_old_password"] = device_old_password;
        // -------------------------------------------------------------------
        // WIFI Cliente settings.json
        // -------------------------------------------------------------------
        jsonSettings["wifi_ip_static"] = wifi_ip_static;
        jsonSettings["wifi_ssid"] = wifi_ssid;
        jsonSettings["wifi_password"] = wifi_password;
        jsonSettings["wifi_ipv4"] = wifi_ipv4;
        jsonSettings["wifi_subnet"] = wifi_subnet;
        jsonSettings["wifi_gateway"] = wifi_gateway;
        jsonSettings["wifi_dns_primary"] = wifi_dns_primary;
        jsonSettings["wifi_dns_secondary"] = wifi_dns_secondary;
        // -------------------------------------------------------------------
        // WIFI AP settings.json
        // -------------------------------------------------------------------
        jsonSettings["ap_mode"] = ap_mode;
        jsonSettings["ap_ssid"] = ap_ssid;
        jsonSettings["ap_password"] = ap_password;
        jsonSettings["ap_visibility"] = ap_visibility;
        jsonSettings["ap_chanel"] = ap_chanel;
        jsonSettings["ap_connect"] = ap_connect;
        // -------------------------------------------------------------------
        // Cloud settings.json
        // -------------------------------------------------------------------
        jsonSettings["mqtt_cloud_enable"] = mqtt_cloud_enable;
        jsonSettings["mqtt_user"] = mqtt_user;
        jsonSettings["mqtt_password"] = mqtt_password;
        jsonSettings["mqtt_server"] = mqtt_server;
        jsonSettings["mqtt_cloud_id"] = mqtt_cloud_id;
        jsonSettings["mqtt_port"] = mqtt_port;
        jsonSettings["mqtt_retain"] = mqtt_retain;
        jsonSettings["mqtt_qos"] = mqtt_qos;
        jsonSettings["mqtt_time_send"] = mqtt_time_send;
        jsonSettings["mqtt_time_interval"] = mqtt_time_interval;
        jsonSettings["mqtt_status_send"] = mqtt_status_send;
        // -------------------------------------------------------------------
        // TIME settings.json
        // -------------------------------------------------------------------
        jsonSettings["time_ajuste"] = time_ajuste;
        jsonSettings["time_date"] = time_date;
        jsonSettings["time_z_horaria"] = time_z_horaria;
        jsonSettings["time_server"] = time_server;
        // -------------------------------------------------------------------
        // Isumotex settings.json
        // -------------------------------------------------------------------
        jsonSettings["PRESSURE_TARGET"] = PRESSURE_TARGET;
        jsonSettings["PRESSURE_THRESHOLD_HIGH"] = PRESSURE_THRESHOLD_HIGH;
        jsonSettings["PRESSURE_THRESHOLD_LOW"] = PRESSURE_THRESHOLD_LOW;
        jsonSettings["PRESSURE_TOLERANCE"] = PRESSURE_TOLERANCE;
        jsonSettings["THRESHOLD_WITHOUT_PRESSURE"] = THRESHOLD_WITHOUT_PRESSURE;
        jsonSettings["WARNING_LIMIT"] = WARNING_LIMIT;
        jsonSettings["MAX_VALVE_OPEN_COUNT"] = MAX_VALVE_OPEN_COUNT;
        jsonSettings["MONITORING_ENABLED"] = MONITORING_ENABLED;
        jsonSettings["VALVE_OPEN_DURATION"] = VALVE_OPEN_DURATION;
        jsonSettings["TIMEOUT_CHECK_PRESSURE"] = TIMEOUT_CHECK_PRESSURE;
        jsonSettings["RETRY_OPEN_VALVE"] = RETRY_OPEN_VALVE;
        // -------------------------------------------------------------------
        // iSumotex i/o settings.json
        // -------------------------------------------------------------------
        jsonSettings["io"]["analog"]["ai_0"][0]   = type_sensor_ai_0;    
        jsonSettings["io"]["analog"]["a2_0"][0]   = type_sensor_ai_1;    
        jsonSettings["io"]["analog"]["ai_0"][1]   = val_ai_sensor_0;     
        jsonSettings["io"]["analog"]["a2_0"][1]   = val_ai_sensor_1;     
        jsonSettings["io"]["analog"]["ai_0"][2]   = enable_ai_sensor_0;  
        jsonSettings["io"]["analog"]["a2_0"][2]   = enable_ai_sensor_1;  
        jsonSettings["io"]["input"]["di_0"][0]    = name_sensor_di_0;    
        jsonSettings["io"]["input"]["di_1"][0]    = name_sensor_di_1;    
        jsonSettings["io"]["input"]["di_0"][1]    = status_di_sensor_0;  
        jsonSettings["io"]["input"]["di_1"][1]    = status_di_sensor_1;  
        jsonSettings["io"]["input"]["di_0"][2]    = enable_di_sensor_0;  
        jsonSettings["io"]["input"]["di_1"][2]    = enable_di_sensor_1;  
        jsonSettings["io"]["relay"]["do_0"][0]    = name_sensor_do[0];       
        jsonSettings["io"]["relay"]["do_1"][0]    = name_sensor_do[1];       
        jsonSettings["io"]["relay"]["do_2"][0]    = name_sensor_do[2];       
        jsonSettings["io"]["relay"]["do_3"][0]    = name_sensor_do[3];      
        jsonSettings["io"]["relay"]["do_0"][1]    = activate_do_sensor[0];
        jsonSettings["io"]["relay"]["do_1"][1]    = activate_do_sensor[1];
        jsonSettings["io"]["relay"]["do_2"][1]    = activate_do_sensor[2];
        jsonSettings["io"]["relay"]["do_3"][1]    = activate_do_sensor[3];
        jsonSettings["io"]["relay"]["do_0"][2]    = enable_do_sensor[0];  
        jsonSettings["io"]["relay"]["do_1"][2]    = enable_do_sensor[1];  
        jsonSettings["io"]["relay"]["do_2"][2]    = enable_do_sensor[2];  
        jsonSettings["io"]["relay"]["do_3"][2]    = enable_do_sensor[3];                          
        // -------------------------------------------------------------------
        // Application settings.json
        // -------------------------------------------------------------------
        jsonSettings["analog_read_sensor"] = analog_read_sensor;

        serializeJsonPretty(jsonSettings, file);
        file.close();
        log("[ INFO ] Configuración Guardada correctamente");
        serializeJsonPretty(jsonSettings, Serial);
        return true;
    }
    else
    {
        log("[ ERROR ] Falló el guardado de la configuración");
        return false;
    }
}