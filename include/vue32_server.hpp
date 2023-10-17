
#include "ESPAsyncWebServer.h"
#include <Update.h>

bool cors = true;

AsyncWebServer server(80);
void InitServer(){
    // -------------------------------------------------------------------
    // Zona API REST
    // -------------------------------------------------------------------
    // URL:                         Método:
    // -------------------------------------------------------------------
    // /api/index                     GET 
    // /api/wifi                      GET 
    // /api/wifi                      PUT
    // /api/scan                      GET
    // /api/cloud                     GET
    // /api/cloud/connection          PUT
    // /api/cloud/data                PUT
    // /api/settings/id               GET
    // /api/settings/id               PUT
    // /api/settings/user             PUT
    // /api/settings/download         GET
    // /api/settings/upload           POST
    // /api/settings/firmware         POST
    // /api/settings/logout           DELETE
    // -------------------------------------------------------------------
    
    // -------------------------------------------------------------------
    // Parametros de Configuración Index
    // url: /api/index
    // Método: GET 
    // -------------------------------------------------------------------
    server.on("/api/index", HTTP_GET, [](AsyncWebServerRequest *request){
        /* if(!request->authenticate(device_old_user, device_old_password)) 
            return request->requestAuthentication(); */        
        const char* dataType = "application/json";
        String json = "";
        json = "{";
        json += "\"meta\": { \"serial\": \""+ deviceID() +"\"},";
        json += "\"data\":{";
                json += "\"device_serial\": \""+ deviceID() +"\"";               
                json += ",\"device_fw_version\": \""+String(device_fw_version)+"\"";
                json += ",\"device_sdk\": \""+ String(ESP.getSdkVersion()) +"\"";
                json += ",\"device_hw_version\": \""+ String(device_hw_version) +"\"";
                json += ",\"device_time_active\": \""+ String(longTimeStr(millis() / 1000)) +"\"";
                json += ",\"device_restart\":"+ String(device_restart);   
                json += ",\"device_ram_size\":"+ String(ESP.getHeapSize());
                json += ",\"device_ram_available\":"+ String(ESP.getFreeHeap()); 
                json += ",\"device_spiffs_total\":"+ String(SPIFFS.totalBytes());      
                json += ",\"device_spiffs_used\":"+ String(SPIFFS.usedBytes());
                json += ",\"device_cpu_clock\":"+ String(getCpuFrequencyMhz());
                json += ",\"device_flash_size\":"+ String(ESP.getFlashChipSize() / (1024.0 * 1024), 2);
                json += ",\"device_manufacturer\": \""+String(device_manufacturer)+"\"";
        mqttClient.connected() ? json += ",\"mqtt_online\": true" : json += ",\"mqtt_online\": false"; 
        mqttClient.connected() ? json += ",\"mqtt_server\": \""+ String(mqtt_server) +"\"" : json += ",\"mqtt_server\": \"server not connected\"" ;
                json += ",\"mqtt_user\": \""+ String(mqtt_user) +"\"";
                json += ",\"mqtt_cloud_id\": \""+ String(mqtt_cloud_id) +"\"";
                json += ",\"mqtt_activity\": \"Inactivo\"";
        WiFi.status() == WL_CONNECTED ? json += ",\"wifi_online\": true" : json += ",\"wifi_online\": false"; 
        wifi_mode == WIFI_STA ? json += ",\"wifi_ssid\": \""+ String(wifi_ssid) +"\"" : json += ",\"wifi_ssid\": \""+ String(ap_ssid) +"\""; 
        wifi_mode == WIFI_STA ? json += ",\"wifi_ipv4\": \""+ String(wifi_ipv4) +"\"" : json += ",\"wifi_ipv4\": \""+ String(ap_IPv4) +"\"";
                json += ",\"wifi_mac\": \""+ String(WiFi.macAddress()) +"\"";
        wifi_mode == WIFI_STA ? json += ",\"wifi_mode\": \"Cliente\"" : json += ",\"wifi_mode\": \"AP\"";
        WiFi.status() == WL_CONNECTED ? json += ",\"wifi_rssi\":"+ String(WiFi.RSSI()) : json += ",\"wifi_rssi\": 0";
        WiFi.status() == WL_CONNECTED ? json += ",\"wifi_signal\":"+ String(getRSSIasQuality(WiFi.RSSI())) : json += ",\"wifi_signal\": 0";     
            json += "},"; 
        json += "\"code\": 1 ";
        json += "}";
        request->send(200, dataType, json);
    });
}