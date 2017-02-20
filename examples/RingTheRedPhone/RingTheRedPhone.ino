//  Copyright (C) 2017 Marc Stoecklin
//
//  This file is part of EspProwl: iOS push notifications through Prowl
//
//  EspProwl is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as
//  published by the Free Software Foundation, either version 3 of
//  the License, or (at your option) any later version.
//
//  EspProwl is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with EspProwl. If not, see
//  <http://www.gnu.org/licenses/>.
//

// Sends a push notification to your iOS device via Prowl

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <EspProwl.h>

const char* WIFI_SSID =  "SSID";
const char* WIFI_PWD =  "PASSWORD";

#define DEBUG 1

int counterValue;

void setup() {
  counterValue = 10;
  Serial.begin(115200);
  if (DEBUG) Serial.println("Attempting to obtain a DHCP lease...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  EspProwl.begin();
  // For Prowl, go to
  //   https://www.prowlapp.com/api_settings.php
  // to create an API key.
  // If you don't, the server will return a 401 error code.
  EspProwl.setApiKey("PROWL_KEY");

  EspProwl.setApplicationName("EspProwl");
}

void loop() {
  if (counterValue > 0) {
    if (DEBUG) Serial.println(counterValue);
    counterValue--;
    delay(300);
  } else if (counterValue == 0) {
    counterValue--;
    if (DEBUG) Serial.print("Sending push notification...");
    int returnCode = EspProwl.push("RingTheRedPhone", "Hi,Comrad", 0);
    if (returnCode == 200) {
      if (DEBUG) Serial.println("OK.");
    } else {
      if (DEBUG) Serial.print("Error. Server returned: ");
      if (DEBUG) Serial.print(returnCode);
    }
  }
}
