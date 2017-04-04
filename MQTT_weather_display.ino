#include <TimeLib.h>
#include <Timezone.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include "settings.h"
#include "display.h"
#include "wifi.h"
#include "mqtt.h"
#include "time.h"


void setup() {
  Serial.begin(9600);
  Serial.println("ILI9341 Test!"); 
  pinMode(0,OUTPUT);
  analogWrite(0, 10);
 
  tft.begin();
  setupWifi();
  setupNtp();
  setupMqtt();

  current_time = myTZ.toLocal(now(), &tcr);
}

void loop(){
  ArduinoOTA.handle();
  
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      if (mqttReconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();
  }
  
  if(draw_all) drawAll();
  if(!popup) timeRefresh();
}
