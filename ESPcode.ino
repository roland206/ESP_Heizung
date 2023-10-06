#include "config.h"
#include <esp_task_wdt.h>
#include <WiFi.h>

#define STA_SSID "FRITZ!Box 7490"
#define STA_PASS "28769660986535732224"
#define HOST  "HeizServer"
#define PORT  8899

#ifdef SERVER_PORT
  WiFiServer wifiServer(SERVER_PORT);
  #define WDT_TIMEOUT (60 * 12)
#else
  #define WDT_TIMEOUT 60
#endif

int      errorCnt = 0;
uint64_t reportMillis = 60000;
uint64_t nextReport = millis() + 1000;
uint64_t nextSample = nextReport + 1000;

#define  PRESSED 0
#define  RELEASED 1
int      button = RELEASED;
int      rebooted;


void interpreteCmd(String cmd) {
  
  int pos = cmd.indexOf("P:");
  if (pos >= 0)
    reportMillis = (cmd.substring(pos+2)).toInt() * 1000;
  
#ifdef N_RELAIS
  pos = cmd.indexOf("R:");
  if (pos >= 0) {
    int relais = (cmd.substring(pos+2)).toInt();
     for(int i=0;i<N_RELAIS;i++)
#ifdef INVERT_RELAIS
         digitalWrite(relaisPins[i], ((relais>>i) & 1) == 0 );
#else
         digitalWrite(relaisPins[i], ((relais>>i) & 1) != 0 );
#endif
#ifdef ZP_PIN
    digitalWrite(ZP_PIN, (relais & 0x10) != 0);
#endif
  }
#endif
}

int reportServer() {
  WiFiClient   client;
  int          trialsServer = 10;
  String        msg;
  
 // LOG("Try to connect server");
 Serial.printf("Try to connect server\n");
  while (!client.connect(HOST, PORT)) {
      delay(500);
      if (trialsServer-- < 0) {
        LOG("Cannot connect server");
        return 1;
      }
  }

#ifdef ESP_GAS
  msg = String(getSensorMSG()) +  ":C:" + String(gasCounts) + ":X:" + lastPeriod;
  gasCounts = lastPeriod = 0;
#else
  #ifdef ESP_STROM
    msg = String(getSensorMSG()) +  ":C:" + String(stromCounts) + ":X:" + lastPeriod;
    stromCounts = lastPeriod = 0;
  #else
    msg = String(getSensorMSG());
  #endif
#endif

  client.print(msg);
  client.flush();
  
  String line = client.readStringUntil('\n');
  client.stop();
  interpreteCmd(line);
  rebooted = 0;
  button = RELEASED;
  return 0;
}

//////////////////////////////////////////////////7   SETUP
void setup() {
    int maxRetries = 10;

    rebooted = 1;
#ifdef DEBUG
    Serial.begin(115200);
    delay(400);
    LOG("Serial connected");
#endif

    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch
    initSensors();

#ifdef N_RELAIS
    for(int i=0;i<N_RELAIS;i++)
    {
        pinMode(relaisPins[i], OUTPUT);
        #ifdef INVERT_RELAIS
           digitalWrite(relaisPins[i], 1);
        #else
           digitalWrite(relaisPins[i], 0);
        #endif
    }
    #ifdef ZP_PIN
        pinMode(ZP_PIN, OUTPUT);
        digitalWrite(ZP_PIN, 0);
    #endif
#endif

#ifdef LED_PIN
    pinMode(LED_PIN, OUTPUT);
#endif

 #ifdef BUTTON_PIN
    pinMode(BUTTON_PIN, INPUT_PULLUP);
 #endif
 
 #ifdef GAS_SENSOR_PIN
    pinMode(GAS_SENSOR_PIN, INPUT);
 #endif

#ifdef STROM_SENSOR_PIN
  pinMode(STROM_SENSOR_PIN, INPUT);
#endif

    WiFi.hostname(espID);
    WiFi.begin(STA_SSID, STA_PASS);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        if (maxRetries-- < 0) waitAndReboot(60);
    }

    LOG("WiFi connected:" + String(espID));


#ifdef SERVER_PORT
  wifiServer.begin();
#endif

#ifdef DEBUG  
  Serial.print("Verbunden unter IP ");
  Serial.println(WiFi.localIP());
  long rssi = WiFi.RSSI();
  Serial.print("Signalstaerke(RSSI) des AP:");
  Serial.print(rssi);
  Serial.println(" dBm");
#endif

    nextReport = millis() + 2000;
}

/////////////////////////////////////////////   LOOP

void loop() {
    uint64_t now   = millis();
    bool doReport  = nextReport < now;

#ifdef LED_PIN
    digitalWrite(LED_PIN, (now & 0x100) == 0);
#endif

#ifdef BUTTON_PIN
  if (checkButton(now)) doReport = true;
#endif

#ifdef GAS_SENSOR_PIN
  if(checkBrenner(now)) doReport = true; 
#endif

#ifdef STROM_SENSOR_PIN
  if(checkStrom(now)) doReport = true; 
#endif


#ifdef SERVER_PORT
  serverLoop();
#else
  esp_task_wdt_reset();
  
  if (now > nextSample) {
    nextSample = now + 30000;
    readSensors();
  }
  
  if (doReport){
    errorCnt += reportServer();
    nextReport = millis() + reportMillis;
  }
  if (errorCnt > 5) waitAndReboot(0);
#endif
}

void waitAndReboot(int time2wait){
  if (time2wait > 0) delay(time2wait);
  ESP.restart();
}
