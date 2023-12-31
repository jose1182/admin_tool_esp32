// -------------------------------------------------------------------
// Definiciones
// -------------------------------------------------------------------
#define WIFILED 12 // GPIO12 LED WIFI
#define MQTTLED 13 // GPIO13 LED MQTT

// CALCULAR LA CAPACIDAD DEL JSON
// Asistente ArduinoJson: https://arduinojson.org/v6/assistant/
// Documentación: https://arduinojson.org/v6/api/json/deserializejson/
// -------------------------------------------------------------------
const size_t capacitySettings = 3072;
// -------------------------------------------------------------------
// Versión de Firmware desde las variables de entorno platformio.ini
// -------------------------------------------------------------------
#define TEXTIFY(A) #A
#define ESCAPEQUOTE(A) TEXTIFY(A)
String device_fw_version = ESCAPEQUOTE(BUILD_TAG);
// -------------------------------------------------------------------
// Version de Hardware y Fabricante
// -------------------------------------------------------------------
#define device_hw_version "iSumotex v1 00000000" // Versión del hardware
#define device_manufacturer "SUMCAB ROBOTICS"     // Fabricante
// -------------------------------------------------------------------
// Zona configuración Dispositivo
// -------------------------------------------------------------------
boolean device_config_file;    // Identificador para archivo de configuración
char device_config_serial[30]; // Numero de serie de cada Archivo configuración unico
char device_id[30];            // ID del dispositivo
int device_restart;            // Número de reinicios
char device_old_user[15];      // Usuario para acceso al servidor Web
char device_old_password[15];  // Contraseña del usuario servidor Web
// -------------------------------------------------------------------
// Zona configuración WIFI modo Cliente
// -------------------------------------------------------------------
int wifi_mode = WIFI_STA;

boolean wifi_ip_static;      // Uso de IP Estática DHCP
char wifi_ssid[30];          // Nombre de la red WiFi
char wifi_password[30];      // Contraseña de la Red WiFi
char wifi_ipv4[15];          // Dir IPv4 Estático
char wifi_gateway[15];       // Dir IPv4 Gateway
char wifi_subnet[15];        // Dir IPv4 Subred
char wifi_dns_primary[15];   // Dir IPv4 DNS primario
char wifi_dns_secondary[15]; // Dir IPv4 DNS secundario
// -------------------------------------------------------------------
// Zona configuración WIFI modo AP
// -------------------------------------------------------------------
boolean ap_mode;      // Uso de Modo AP
char ap_ssid[31];     // Nombre del SSID AP
char ap_password[63]; // Contraseña del AP
int ap_chanel;        // Canal AP
int ap_visibility;    // Es visible o no el AP  (0 - Visible  1 - Oculto)
int ap_connect;       // Número de conexiones en el AP máx 8 conexiones ESP32
// -------------------------------------------------------------------
// Zona configuración MQTT
// -------------------------------------------------------------------
boolean mqtt_cloud_enable; // Habilitar MQTT Broker
char mqtt_cloud_id[30];    // Cliente ID MQTT Broker
char mqtt_user[30];        // Usuario MQTT Broker
char mqtt_password[39];    // Contraseña del MQTT Broker
char mqtt_server[39];      // Servidor del MQTT Broker
int mqtt_port;             // Puerto servidor MQTT Broker
boolean mqtt_retain;       // Habilitar mensajes retenidos
int mqtt_qos;              // Calidad del servicio
boolean mqtt_time_send;    // Habilitar en envio de datos
int mqtt_time_interval;    // Tiempo de envio por MQTT en segundos
boolean mqtt_status_send;  // Habilitar en envio de estados
// -------------------------------------------------------------------
// Zona Firmware Update
// -------------------------------------------------------------------
size_t content_len;
#define U_PART U_SPIFFS
// -------------------------------------------------------------------
// Zona EEPROM para contador de reinicios
// -------------------------------------------------------------------
#define Start_Address 0
#define Restart_Address Start_Address + sizeof(int)
// -------------------------------------------------------------------
// Zona TIME
// -------------------------------------------------------------------
ESP32Time rtc; // Clase ESP32Time

bool time_ajuste;     // 0 - Manual - 1 - Automático internet SRV NTP
char time_date[18];   // 2023-11-28T10:35
long time_z_horaria;  // Zona Horaria GMT 0 = 0 - GMT +1/4 = 3600*4 - GMT -1 = -3600
char time_server[39]; // Servidor NTP Ejemplo: time.nist.gov

int time_hr; // Hora 0 - 23
int time_mn; // Minutos 0 -59
int time_sc; // Segundos 0 - 59
int time_dy; // Días 1 -31
int time_mt; // Meses 1 -12
int time_yr; // Año 2023

// NTP Server
WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP);

long lastTime = 0; // Variable para tiempo de envio por WS

// -------------------------------------------------------------------
// Zona Isumotex variables
// -------------------------------------------------------------------

double PRESSURE_TARGET = 0.0;            // Valor objetivo de presión en bares
double PRESSURE_THRESHOLD_HIGH = 0.0;    // Umbral de presión alta en bares
double PRESSURE_THRESHOLD_LOW = 0.0;     // Umbral de presión baja en bares
double PRESSURE_TOLERANCE = 0.0;         // Tolerancia de presión en bares
double THRESHOLD_WITHOUT_PRESSURE = 0.0; // Minima presion de aire
int WARNING_LIMIT;                       // Límite de advertencias antes de considerar que la manguera es defectuosa
int MAX_VALVE_OPEN_COUNT;                //
bool MONITORING_ENABLED;

int VALVE_OPEN_DURATION;    // Timeout de válvula abierta sin habaer alcanzado a cerrar
int TIMEOUT_CHECK_PRESSURE; // Ventana de timepo de comprobración si la válvula ha intenado ah abierto y cerrado
int RETRY_OPEN_VALVE;       // Intentos que abrirá la valvula en un tiempo determinado -> VAL_OPEN_DURATION
// -------------------------------------------------------------------
// Zona Isumotex I/O
// -------------------------------------------------------------------

// Definitions
//Analog inputs
#define AI_0 12 // GPIOXX Analog input for sensor 1 o 2 4-20mA
#define AI_1 13 // GPIOXX Analog input for sensor 1 o 2 4-20mA
//Digital inputs
#define DI_0 13 // GPIOXX Digital input for readng from external input
#define DI_1 13 // GPIOXX Digital input for readng from external input
//Outpus Relays
#define DO_0 32 // GPIOXX Digital output for readng activate a external device
#define DO_1 33 // GPIOXX Digital output for readng activate a external device
#define DO_2 34 // GPIOXX Digital output for readng activate a external device
#define DO_3 35 // GPIOXX Digital output for readng activate a external device
//Definitons pines outputs
uint8_t relay_pin[] = {DO_0, DO_1, DO_2, DO_3};
// Variables Analog inputs
long type_sensor_ai_0;
long type_sensor_ai_1;
double val_ai_sensor_0 = 0.0;
double val_ai_sensor_1 = 0.0;
bool enable_ai_sensor_0;
bool enable_ai_sensor_1;

// Variables Digital inputs
String name_sensor_di_0;
String name_sensor_di_1;
bool status_di_sensor_0;
bool status_di_sensor_1;
bool enable_di_sensor_0;
bool enable_di_sensor_1;

// Variables Digital outputs
String name_sensor_do[4];
bool activate_do_sensor[4];
bool enable_do_sensor[4];

//Delete if is not using
bool analog_read_sensor;