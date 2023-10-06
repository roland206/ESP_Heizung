#include "config.h"

#ifdef DHT_PIN
  #include <DHT.h>
  DHT dht(DHT_PIN, DHT22);
#endif

#ifdef DS18B20_PIN
  #include <DallasTemperature.h>
  OneWire           oneWire(DS18B20_PIN);
  DallasTemperature DS18B20(&oneWire);
  DeviceAddress     sensors[N_SENSORS];
#endif


#define BUFFER_SIZE 16
typedef struct  {int n; float data[BUFFER_SIZE]; float mean;} Buffer;
Buffer buf1, buf2, buf3;

static void insertData(Buffer *buf, float data);
static void calcMean(Buffer *buf);

static void calcMean(Buffer *buf) {

  float ym = 0;
  float weight  = 1;
  float wSum = 0.0;

  if (buf->n <1) {
    buf->mean = 0;
    return;
  }
  for(int i=0; i<buf->n; i++){
    ym += weight * buf->data[i];
    wSum += weight;
    weight += 1.0;
  }

  buf->mean = ym / wSum;
  #ifdef DEBUG
    Serial.printf("New mean %5.2f\n", buf->mean);
  #endif
}

static void insertData(Buffer *buf, float data)
{

  if (buf->n < BUFFER_SIZE) {
    buf->data[buf->n] = data;
    buf->n++;
  } else {

    float *src = buf->data;
    for(int i=0; i< (BUFFER_SIZE - 1); i++) {
      *src = *(src+1);
      src++;
    }
    *src = data;
  }
  calcMean(buf);
}

///////////////////////////////////////////////////////////////////////////////////////////
void initSensors() {

#ifdef DHT_PIN
  dht.begin();
#endif

#ifdef DS18B20_PIN
  DS18B20.begin();
  if (N_SENSORS > 1) {
    oneWire.reset_search();
    oneWire.search(sensors[0]);
    oneWire.search(sensors[1]);
    if (N_SENSORS > 2) oneWire.search(sensors[2]);
  }
#endif

  buf1.n = buf2.n = buf3.n = 0;
  readSensors();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void readSensors() {

#ifdef DHT_PIN
  insertData(&buf1, dht.readTemperature());
  insertData(&buf2, dht.readHumidity());
#endif

#ifdef DS18B20_PIN
  DS18B20.requestTemperatures();
  if (N_SENSORS == 1){
    insertData(&buf1, DS18B20.getTempCByIndex(0));
  } else {
    if (N_SENSORS > 0) insertData(&buf1, DS18B20.getTempC(sensors[0]));
    if (N_SENSORS > 1) insertData(&buf2, DS18B20.getTempC(sensors[1]));
    if (N_SENSORS > 2) insertData(&buf3, DS18B20.getTempC(sensors[2]));
  }
#endif
}

static char sensorMessage[256];

char *getSensorMSG() {
  char buf[16];

#ifdef DHT_PIN
  sprintf(sensorMessage, "%s:T:%5.2f:H:%5.2f", espID, buf1.mean, buf2.mean);
#else
  #ifdef DS18B20_PIN
    if (N_SENSORS == 1)
      sprintf(sensorMessage, "%s:T:%5.2f", espID, buf1.mean);
   if (N_SENSORS == 2)
      sprintf(sensorMessage, "%s:T1:%5.2f:T2:%5.2f", espID, buf1.mean, buf2.mean);
    if (N_SENSORS == 3)
      sprintf(sensorMessage, "%s:T1:%5.2f:T2:%5.2f:T3:%5.2f", espID, buf1.mean, buf2.mean, buf3.mean);
  #else
    sprintf(sensorMessage, "%s:T:0", espID);
  #endif
#endif

 if (rebooted) strcat(sensorMessage, ":B:1");
  
#ifdef BUTTON_PIN
  sprintf(buf, ":S:%d", button);
  strcat(sensorMessage, buf);
  button = RELEASED;
#endif

#ifdef DEBUG
  LOG("Sensor read: " + String(sensorMessage) + " data in buffer = " + String(buf1.n));
#endif

  return sensorMessage;
}
