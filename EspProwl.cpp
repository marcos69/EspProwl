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

#include <string.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include "WiFi.h"
#endif
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

/*
  if (deliveryMechanism == PROWL) {
    notificationServiceHost = PROWL_API_HOST;
    notificationServiceUrlPath = PROWL_PUSH_PATH_URI;
  }

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
  
  int result;
  char priorityStr[5];
  sprintf(priorityStr, "%d", priority);

  String push_arguments;
  push_arguments = "apikey="+ (String) apiKey ;
  push_arguments += "&application="+ (String) applicationName ;
  push_arguments += "&event="+ (String) eventStr ;
  push_arguments += "&description="+ (String) messageStr ;
  push_arguments += "&priority="+ (String) priorityStr ;

  if (EspProwl_DEBUG) {
    Serial.println(push_arguments);
  }

  /* #ifdef ESP8266
  String notifSHSUP = (String) notificationServiceHost + (String) notificationServiceUrlPath;
  Serial.println(notifSHSUP);

  HTTPClient client;
  client.begin( notifSHSUP );
  client.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
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
  #else*/

  WiFiClient client;
  result = 0;
  if (client.connect("api.prowlapp.com", 80)) {
    client.println("POST /publicapi/add HTTP/1.1");
    client.println("Host: api.prowlapp.com" );
    client.println("Cache-Control: no-cache");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(push_arguments.length());
    client.println();
    client.println(push_arguments);

    int incr=0;
    char response_buffer[255];
    char result_buffer[16];

    long interval = 5000;
    unsigned long currentMillis = millis(), previousMillis = millis();

    while(!client.available()){
      if( (currentMillis - previousMillis) > interval ) {
        Serial.println("Timeout");
        client.stop();
        return result=-1;
      }
      currentMillis = millis();
    }

    while (client.connected())
    {
      if ( client.available() )
      {
        char str=client.read();
        response_buffer[incr++]=str;
        if (incr>15) {
          client.stop();
          break;
        }
      }
    }

    response_buffer[incr]=0;
    strncpy(result_buffer, &response_buffer[9], 3);
    result_buffer[3] = '\0';
    result = atoi(result_buffer);

    if (EspProwl_DEBUG) {
      Serial.println("response: ");
      Serial.println(result);
    }
  }

  //#endif
  
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
