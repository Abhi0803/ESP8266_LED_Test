#include <ESP8266WebServer.h>
#include <Wire.h>
#include <SPI.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
float temperature, humidity, pressure, altitude;

/*Put your SSID & Password*/
const char *ssid = "Abhinav’s iPhone"; // Enter SSID here
const char *password = "123456798";    //Enter Password here

ESP8266WebServer server(80);

// const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(13) + 280;
// DynamicJsonBuffer doc(capacity);
// DynamicJsonBuffer jsonBuffer(791);
const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(13) + 280;
DynamicJsonDocument doc(capacity);

void setup()
{
  Serial.begin(9600);
  delay(100);

  Serial.println("Connecting to ");
  Serial.println(ssid);
  //  http.begin("http://api.openweathermap.org/data/2.5/weather?q=rewa&appid=056ceaec162d84d33b4cb29dee7ef1d5"); //Specify request destination

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());
}
void loop()
{
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http; //Object of class HTTPClient
    http.begin("http://api.openweathermap.org/data/2.5/weather?q=rewa&units=metric&appid=056ceaec162d84d33b4cb29dee7ef1d5#");
    int httpCode = http.GET();
    //Check the returning code
    if (httpCode > 0)
    {
      String json = http.getString();

      deserializeJson(doc, json);

      float coord_lon = doc["coord"]["lon"]; // 81.3
      float coord_lat = doc["coord"]["lat"]; // 24.53

      JsonObject weather_0 = doc["weather"][0];
      int weather_0_id = weather_0["id"];                           // 800
      const char *weather_0_main = weather_0["main"];               // "Clear"
      const char *weather_0_description = weather_0["description"]; // "clear sky"
      const char *weather_0_icon = weather_0["icon"];               // "01n"

      const char *base = doc["base"]; // "stations"

      JsonObject main = doc["main"];
      float main_temp = main["temp"];             // 301.03
      float main_feels_like = main["feels_like"]; // 303.4
      float main_temp_min = main["temp_min"];     // 301.03
      float main_temp_max = main["temp_max"];     // 301.03
      int main_pressure = main["pressure"];       // 1007
      int main_humidity = main["humidity"];       // 70
      int main_sea_level = main["sea_level"];     // 1007
      int main_grnd_level = main["grnd_level"];   // 972

      int visibility = doc["visibility"]; // 10000

      float wind_speed = doc["wind"]["speed"]; // 3.24
      int wind_deg = doc["wind"]["deg"];       // 68

      int clouds_all = doc["clouds"]["all"]; // 3

      long dt = doc["dt"]; // 1602772377

      JsonObject sys = doc["sys"];
      const char *sys_country = sys["country"]; // "IN"
      long sys_sunrise = sys["sunrise"];        // 1602721950
      long sys_sunset = sys["sunset"];          // 1602763695

      int timezone = doc["timezone"]; // 19800
      long id = doc["id"];            // 1258182
      const char *name = doc["name"]; // "Rewa"
      int cod = doc["cod"];           // 200

      Serial.print("main_temp:");
      Serial.println(main_temp);
      Serial.print("visibility:");
      Serial.println(visibility);
      Serial.print("name:");
      Serial.println(name);
    }
    http.end(); //Close connection
  }
  // Delay
  delay(60000);
}

/*

SSID:	Abhinav’s iPhone
Protocol:	Wi-Fi 4 (802.11n)
Security type:	WPA2-Personal
Network band:	2.4 GHz
Network channel:	6
IPv6 address:	2401:4900:51c2:1b9a:94ef:324b:ded2:9c2
Link-local IPv6 address:	fe80::94ef:324b:ded2:9c2%4
IPv6 DNS servers:	fe80::418:d364:7819:1715%4
IPv4 address:	172.20.10.2
IPv4 DNS servers:	172.20.10.1
Manufacturer:	Qualcomm Atheros Communications Inc.
Description:	Qualcomm Atheros QCA9377 Wireless Network Adapter
Driver version:	12.0.0.722
Physical address (MAC):	E8-2A-44-61-A9-97

 */
