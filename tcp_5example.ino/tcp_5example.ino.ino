#include <ESP8266WiFi.h>


#define MAX_SRV_CLIENTS 3
const char* ssid = "DFKI-IOT";
const char* password = "yGLr?gF7";

WiFiServer server(21);
WiFiClient serverClients[MAX_SRV_CLIENTS];
size_t len = 0; 
int prevTime = 0; 
int Time = 0; 
byte message[28] = "Time ";

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial1.print("Could not connect to"); Serial1.println(ssid);
    while(1) delay(500);
  }
  server.begin();
  server.setNoDelay(true);
}

void loop() {
  uint8_t i;
  
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        continue;
      }
    }
    //no free spot
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){
        while(serverClients[i].available()) Serial.write(serverClients[i].read());
        //you can reply to the client here
      }
    }
  }
    Time = micros() ;
    prevTime  = Time - prevTime;   
    String(Time, DEC).getBytes(message+6, 10);    
    message[16]= ' ';    
    String(prevTime, DEC).getBytes(message+17, 10);
    message[27]= 10;  
    len = sizeof(message);   

    
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
         serverClients[i].write((const uint8_t *)message, 28  );
         }
    }
    prevTime  = Time;
  
}
