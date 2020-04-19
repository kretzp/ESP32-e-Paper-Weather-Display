#ifndef OPENHAB_H_
#define OPENHAB_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

const char oh_server[] = "openhab";
const int oh_port = 8080;

bool obtain_openhab_data(WiFiClient& client, const String& uri, String& response) {
  client.stop(); // close connection before sending a new request
  HTTPClient http;
  
  //http.begin(uri,test_root_ca); //HTTPS example connection
  http.begin(client, oh_server, oh_port, uri);
  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK) {
    response = http.getString();
    Serial.printf("Response: %s\n", response);
    client.stop();
    http.end();
    return true;
  }
  else
  {
    Serial.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
    client.stop();
    http.end();
    return false;
  }
  http.end();
  return true;
}

#endif /* ifndef OPENHAB_H_ */
