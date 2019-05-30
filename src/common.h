#ifndef COMMON_H_
#define COMMON_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include "forecast_record.h"

//#########################################################################################
void Convert_Readings_to_Imperial() {
  WxConditions[0].Pressure = WxConditions[0].Pressure * 0.02953;   // hPa to ins
  WxForecast[1].Rainfall   = WxForecast[1].Rainfall   * 0.0393701; // mm to inches of rainfall
  WxForecast[1].Snowfall   = WxForecast[1].Snowfall   * 0.0393701; // mm to inches of snowfall
}

//#########################################################################################
// Problems with stucturing JSON decodes, see here: https://arduinojson.org/assistant/
bool DecodeWeather(String json, String Type) {
  Serial.println("Received a JSON string of size : " + String(json.length()));
  Serial.print(F("Creating object...and "));
  // allocate the JsonDocument
  DynamicJsonDocument doc(20 * 1024);
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }
  // convert it to a JsonObject
  JsonObject root = doc.as<JsonObject>();
  Serial.println(" Decoding " + Type + " data");
  if (Type == "weather") {
    // All Serial.println statements are for diagnostic purposes and not required, remove if not needed
    WxConditions[0].lon         = root["coord"]["lon"].as<float>();              Serial.println(WxConditions[0].lon);
    WxConditions[0].lat         = root["coord"]["lat"].as<float>();              Serial.println(WxConditions[0].lat);
    WxConditions[0].Main0       = root["weather"][0]["main"].as<char*>();        Serial.println(WxConditions[0].Main0);
    WxConditions[0].Forecast0   = root["weather"][0]["description"].as<char*>(); Serial.println(WxConditions[0].Forecast0);
    WxConditions[0].Icon        = root["weather"][0]["icon"].as<char*>();        Serial.println(WxConditions[0].Icon);
    WxConditions[0].Forecast1   = root["weather"][1]["main"].as<char*>();        Serial.println(WxConditions[0].Forecast1);
    WxConditions[0].Forecast2   = root["weather"][2]["main"].as<char*>();        Serial.println(WxConditions[0].Forecast2);
    WxConditions[0].Temperature = root["main"]["temp"].as<float>();              Serial.println(WxConditions[0].Temperature);
    WxConditions[0].Pressure    = root["main"]["pressure"].as<float>();          Serial.println(WxConditions[0].Pressure);
    WxConditions[0].Humidity    = root["main"]["humidity"].as<float>();          Serial.println(WxConditions[0].Humidity);
    WxConditions[0].Low         = root["main"]["temp_min"].as<float>();          Serial.println(WxConditions[0].Low);
    WxConditions[0].High        = root["main"]["temp_max"].as<float>();          Serial.println(WxConditions[0].High);
    WxConditions[0].Windspeed   = root["wind"]["speed"].as<float>();             Serial.println(WxConditions[0].Windspeed);
    WxConditions[0].Winddir     = root["wind"]["deg"].as<float>();               Serial.println(WxConditions[0].Winddir);
    WxConditions[0].Cloudcover  = root["clouds"]["all"].as<int>();               Serial.println(WxConditions[0].Cloudcover); // in % of cloud cover
    WxConditions[0].Visibility  = root["visibility"].as<int>();                  Serial.println(WxConditions[0].Visibility); // in metres
    WxConditions[0].Country     = root["sys"]["country"].as<char*>();            Serial.println(WxConditions[0].Country);
    WxConditions[0].Sunrise     = root["sys"]["sunrise"].as<int>();              Serial.println(WxConditions[0].Sunrise);
    WxConditions[0].Sunset      = root["sys"]["sunset"].as<int>();               Serial.println(WxConditions[0].Sunset);
  }
  if (Type == "forecast") {
    //Serial.println(json);
    const char* cod                 = root["cod"]; // "200"
    float message                   = root["message"];
    int cnt                         = root["cnt"];
    JsonArray list                  = root["list"];
    Serial.print(F("\nReceiving Forecast period - ")); //------------------------------------------------
    for (byte r = 0; r < max_readings; r++) {
      Serial.println("\nPeriod-" + String(r) + "--------------");
      WxForecast[r].Dt                = list[r]["dt"].as<char*>();
      WxForecast[r].Temperature       = list[r]["main"]["temp"].as<float>();              Serial.println(WxForecast[r].Temperature);
      WxForecast[r].Low               = list[r]["main"]["temp_min"].as<float>();          Serial.println(WxForecast[r].Low);
      WxForecast[r].High              = list[r]["main"]["temp_max"].as<float>();          Serial.println(WxForecast[r].High);
      WxForecast[r].Pressure          = list[r]["main"]["pressure"].as<float>();          Serial.println(WxForecast[r].Pressure);
      WxForecast[r].Humidity          = list[r]["main"]["humidity"].as<float>();          Serial.println(WxForecast[r].Humidity);
      WxForecast[r].Forecast0         = list[r]["weather"][0]["main"].as<char*>();        Serial.println(WxForecast[r].Forecast0);
      WxForecast[r].Forecast0         = list[r]["weather"][1]["main"].as<char*>();        Serial.println(WxForecast[r].Forecast1);
      WxForecast[r].Forecast0         = list[r]["weather"][2]["main"].as<char*>();        Serial.println(WxForecast[r].Forecast2);
      WxForecast[r].Description       = list[r]["weather"][0]["description"].as<char*>(); Serial.println(WxForecast[r].Description);
      WxForecast[r].Icon              = list[r]["weather"][0]["icon"].as<char*>();        Serial.println(WxForecast[r].Icon);
      WxForecast[r].Cloudcover        = list[r]["clouds"]["all"].as<int>();               Serial.println(WxForecast[0].Cloudcover); // in % of cloud cover
      WxForecast[r].Windspeed         = list[r]["wind"]["speed"].as<float>();             Serial.println(WxForecast[r].Windspeed);
      WxForecast[r].Winddir           = list[r]["wind"]["deg"].as<float>();               Serial.println(WxForecast[r].Winddir);
      WxForecast[r].Rainfall          = list[r]["rain"]["3h"].as<float>();                Serial.println(WxForecast[r].Rainfall);
      WxForecast[r].Snowfall          = list[r]["snow"]["3h"].as<float>();                Serial.println(WxForecast[r].Snowfall);
      WxForecast[r].Period            = list[r]["dt_txt"].as<char*>();                    Serial.println(WxForecast[r].Period);
    }
    //------------------------------------------
    float pressure_trend = WxForecast[0].Pressure - WxForecast[2].Pressure; // Measure pressure slope between ~now and later
    pressure_trend = ((int)(pressure_trend * 10)) / 10.0; // Remove any small variations less than 0.1
    WxConditions[0].Trend = "0";
    if (pressure_trend > 0)  WxConditions[0].Trend = "+";
    if (pressure_trend < 0)  WxConditions[0].Trend = "-";
    if (pressure_trend == 0) WxConditions[0].Trend = "0";

    if (Units == "I") Convert_Readings_to_Imperial();
  }
  return true;
}
#endif /* ifndef COMMON_H_ */