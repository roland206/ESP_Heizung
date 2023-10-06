#include "config.h"

#ifdef SERVER_PORT

void zirkulationsPumpe() {
  
    digitalWrite(ZP_PIN, 1);
    delay(1000);
    digitalWrite(ZP_PIN, 0);
}


int interpreteMsg(String cmd) {
  
  int pos = cmd.indexOf("R:");
  if (pos >= 0) {
    int relais = (cmd.substring(pos+2)).toInt();
     for(int i=0;i<N_RELAIS;i++)
         digitalWrite(relaisPins[i], ((relais>>i) & 1) != 0 );
     if (relais & 0x10) zirkulationsPumpe();
  }
}


void serverLoop() {
  char cmd[128];


  WiFiClient client = wifiServer.available();

  if (client) {
    if(client.connected()){
      esp_task_wdt_reset();
      readSensors();

      client.println(String(getSensorMSG()));
      rebooted = 0;
      int p = 0;
      while (client.available()==0);
      while (client.available()>0)
        cmd[p++] = client.read();
      cmd[p] = 0;
    }
    client.stop();
    interpreteMsg(String(cmd));
    button = RELEASED;
  }
}
#endif
