
#ifndef H_CONFIG
#define H_CONFIG

#define DEBUGx

#define xESP_DHT
#define xESP_DS18B20
#define xESP_HALLE_SUED
#define xESP_HALLE_MITTE
#define xESP_HALLE_NORD
#define xESP_ZIMMER_2
#define xESP_ZIMMER_3
#define xESP_KUECHE
#define ESP_BAD
#define xESP_HK_OBEN
#define xESP_HK_HEIZUNG
#define xESP_HK_KUECHE
#define xESP_OBEN_SUED
#define xESP_OBEN_NORD
#define xESP_SCHLAFZIMMER
#define xESP_OST
#define xESP_GAS
#define xESP_STROM
#define xESP_WERKSTATT

#define LED_PIN         2

#ifdef ESP_DHT
#define DHT_PIN 22
char *espID = "esp-Tester";
#endif

#ifdef ESP_DS18B20
#define DS18B20_PIN 22
#define N_SENSORS 1
char *espID = "esp-Tester";
#endif

//////////////////////////////////////////  Küche
#ifdef ESP_KUECHE
#define DHT_PIN 22
char *espID = "esp-Kueche";
#endif

//////////////////////////////////////////  Halle Sued
#ifdef ESP_HALLE_SUED
#define DHT_PIN 22
char *espID = "esp-Halle-Sued";
#endif

//////////////////////////////////////////  Halle Mitte
#ifdef ESP_HALLE_MITTE
#define DHT_PIN 22
char *espID = "esp-Halle-Mitte";
#endif

//////////////////////////////////////////  Halle Nord
#ifdef ESP_HALLE_NORD
#define DHT_PIN 22
char *espID = "esp-Halle-Nord";
#endif

//////////////////////////////////////////  Zimmer 2
#ifdef ESP_ZIMMER_2
#define DS18B20_PIN 22
#define N_SENSORS 1
char *espID = "esp-Zimmer-2";
#endif

//////////////////////////////////////////  Zimmer 3
#ifdef ESP_ZIMMER_3
#define DHT_PIN 22
char *espID = "esp-Zimmer-3";
#endif

//////////////////////////////////////////  Werkstatt
#ifdef ESP_WERKSTATT
#define DHT_PIN 22
char *espID = "esp-Werkstatt";
#endif

//////////////////////////////////////////  Oben Süed
#ifdef ESP_OBEN_SUED
#define DHT_PIN 22
#undef LED_PIN
char *espID = "esp-Oben-Sued";

#endif//////////////////////////////////////////  Oben Nord
#ifdef ESP_OBEN_NORD
#define DS18B20_PIN 22
#define N_SENSORS 1
#undef LED_PIN
char *espID = "esp-Oben-Nord";
#endif

//////////////////////////////////////////  Schalfzimmer
#ifdef ESP_SCHLAFZIMMER
#define DHT_PIN 22
#undef LED_PIN
char *espID = "esp-Schlafzimmer";
#endif

//////////////////////////////////////////  Bad
#ifdef ESP_BAD
#define DHT_PIN 22
#define BUTTON_PIN 17
char *espID = "esp-Bad";
#endif

////////////////////////////////////////  Heizkreis Wohnzimmer
#ifdef ESP_HK_OBEN
#define PUTIN_BOARD
#define N_SENSORS 1
char *espID = "esp-HK-Oben";
#endif

////////////////////////////////////////  Heizkreis Technikraum
#ifdef ESP_HK_HEIZUNG
#define ZP_PIN     13
#define PUTIN_BOARD
#define N_SENSORS 2
#define SERVER_PORT 8889
char *espID = "esp-HK-Heizung";
void serverLoop();
#endif


////////////////////////////////////////  Ostwand Aussen
#ifdef ESP_OST
#define N_RELAIS 1
int relaisPins[N_RELAIS] = {13};
#define DS18B20_PIN 14
#define N_SENSORS 2
char *espID = "esp-Ostwand";
#endif

///////////////////////////////////////  Heizkreise Küche
#ifdef ESP_HK_KUECHE
#define N_RELAIS 6
int relaisPins[N_RELAIS] = {22, 23, 0, 4, 21, 19};
#define NO_SENSOR
#define INVERT_RELAIS
char *espID = "esp-HK-Kueche";
#endif

////////////////////////////////////  Gas Aussen West
#ifdef ESP_GAS
#define DS18B20_PIN 22
#define N_SENSORS 2
#define GAS_SENSOR_PIN 34
long lastPeriod = 0;
int gasCounts = 0;
char *espID = "esp-Gas";
#endif


///////////////////////////////////// Zimmer 1 Strom
#ifdef ESP_STROM
#define DS18B20_PIN 22
#define N_SENSORS 1
#define STROM_SENSOR_PIN 26
long lastPeriod = 0;
int stromCounts = 0;
int checkStrom(uint64_t now);
char *espID = "esp-Strom";
#endif

#ifdef PUTIN_BOARD
#define N_RELAIS 4
int relaisPins[N_RELAIS] = {16, 17, 18, 19};
#define BUTTON_PIN 23
#define DS18B20_PIN 22
#endif


#ifdef DEBUG
#define LOG(MSG) Serial.println(MSG)
#else
#define LOG(MSG)
#endif

















#endif
