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

#include <Arduino.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "EspProwl.h"

#define PROWL_API_HOST "http://api.prowlapp.com"
#define PROWL_PUSH_PATH_URI "/publicapi/add"

#define EspProwl_DEBUG 0

EspProwlClass::EspProwlClass() {
}

void EspProwlClass::begin() {
  begin(PROWL);
}

void EspProwlClass::begin(DeliveryMechanism deliveryMechanism) {

  if (deliveryMechanism == PROWL) {
    notificationServiceHost = PROWL_API_HOST;
    notificationServiceUrlPath = PROWL_PUSH_PATH_URI;
  }
/*
  if (EspProwl_DEBUG) {
    if (ret == 1) {
      Serial.print("The IP address of ");
      Serial.print(notificationServiceHost);
      Serial.print(" is: ");
  //    remote_addr.printTo(Serial);
      Serial.println(".");
    } else if (ret == -1) {
      Serial.println("Timed out.");
    } else {
      Serial.print("Failed with error code: ");
      Serial.print(ret);
      Serial.println(".");
    }
  }  */
}

void EspProwlClass::setApiKey(char *aApiKey) {
  strncpy(apiKey, aApiKey, 51);
}

void EspProwlClass::setApplicationName(char *aApplicationName) {
  strncpy(applicationName, aApplicationName, 257);
}

int EspProwlClass::push(char *eventStr, char *messageStr, int priority) {
  if (EspProwl_DEBUG) {
    Serial.println(apiKey);
    Serial.println(applicationName);
    Serial.println(eventStr);
    Serial.println(messageStr);
    Serial.println(priority);
  }
  char priorityStr[5];
  sprintf(priorityStr, "%d", priority);
  String notifSHSUP;
  notifSHSUP = (String) notificationServiceHost + (String) notificationServiceUrlPath;

  HTTPClient client;
  Serial.println(notifSHSUP);

  client.begin( notifSHSUP );
  client.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String push_arguments;
  push_arguments = "apikey="+ (String) apiKey+"&application="+ (String) applicationName+"&event="+ (String) eventStr +"&description="+ (String) messageStr+ "&priority="+ (String) priorityStr;

    if (EspProwl_DEBUG) {
      Serial.println(push_arguments);
    }

  int result = client.POST(push_arguments);

  if (result==HTTP_CODE_OK) {
    client.writeToStream(&Serial);
    client.end();
  }
  else {
    client.writeToStream(&Serial);
    client.end();
    if (EspProwl_DEBUG) {
      Serial.println("failed to connect");
    }
  }
  if (result==200) {
    if (EspProwl_DEBUG) {
      Serial.println("data uploaded");
    }
  }
  else {
    if (EspProwl_DEBUG) {
      Serial.print("ERROR: Server returned ");
      Serial.println(result);
    }
  }

  return result;
}

EspProwlClass EspProwl;
