/* 
Libraries Required and Includes Used
*/

// I2C and SPI Libraries
#include <Wire.h>
#include <SPI.h>

// ESP8266 Related Libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

// JSON Parsing Library
#include <ArduinoJson.h>

// OLED Display Libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* 
Global Variables & Object Declarations and Defines for the Functions 
*/

#define SCREEN_WIDTH          128 // OLED display width, in pixels
#define SCREEN_HEIGHT         64 // OLED display height, in pixels
#define OLED_RESET            -1    // Reset pin # (or -1 if sharing Arduino reset pin)

// Define the City You want Data of
#define CITY     "Bhopal";

// Weather API Data Variables
float main_temp, humidity, pressure, wind_speed;
int main_pressure, main_humidity, visibility, timezone;
long dt, sys_sunrise, sys_sunset;
const char *City, *Description;
// Size of the Document Used for JSON Parsing(Can be found in the ArduinoJSOn website)
const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(8) + JSON_OBJECT_SIZE(13) + 280;
DynamicJsonDocument doc(capacity);

/*Put your SSID & Password*/
const char *ssid = "Abhinav’s iPhone"; // Enter SSID here
const char *password = "12345679";     //Enter Password here

// Objects
ESP8266WebServer server(80); //Object of class ESP8266WebServer
HTTPClient http;             //Object of class HTTPClient
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* 
Declarations for the Functions Used
*/

void Wifi_Init(void);
void API_Init(void);
void Display_Init(void);

/* 
Declarations for the Private Functions Used
*/
String SendHTML(float temperature, float humidity, float pressure, float visibility);
void handle_OnConnect(void);
void handle_NotFound(void);

void setup()
{
  Serial.begin(9600);
  delay(100);

  Wifi_Init();
  API_Init();
  Display_Init();
}

void loop()
{
  server.handleClient();
}

/* 
Definitions for the Functions Used
*/

void Wifi_Init(void)
{
  Serial.begin(9600);
  delay(100);

  Serial.println("Connecting to ");
  Serial.println(ssid);

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

void API_Init(void)
{
  // API Link
  String LinkInit = "http://api.openweathermap.org/data/2.5/weather?q=";
  String CityName = CITY;
  String EndLink  = "&units=metric&appid=056ceaec162d84d33b4cb29dee7ef1d5#";
  String Link     = LinkInit + CityName + EndLink;

  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http; //Object of class HTTPClient
    http.begin(Link);
    int httpCode = http.GET();
    //Check the returning code
    if (httpCode > 0)
    {
      String json = http.getString();

      deserializeJson(doc, json);

      JsonObject weather_0 = doc["weather"][0];

      const char *weather_0_description = weather_0["description"]; // "clear sky"
      Description = weather_0["description"];

      JsonObject main = doc["main"];
      main_temp = main["temp"];         // 301.03
      main_pressure = main["pressure"]; // 1007
      main_humidity = main["humidity"]; // 70

      visibility = doc["visibility"]; // 10000

      wind_speed = doc["wind"]["speed"]; // 3.24

      dt = doc["dt"]; // 1602772377

      JsonObject sys = doc["sys"];
      const char *sys_country = sys["country"]; // "IN"
      sys_sunrise = sys["sunrise"];             // 1602721950
      sys_sunset = sys["sunset"];               // 1602763695

      int timezone = doc["timezone"]; // 19800
      long id = doc["id"];            // 1258182
      const char *name = doc["name"]; // "Rewa"
      City = doc["name"];
    }
    http.end(); //Close connection
  }
}

void Display_Init(void)
{
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started at http://172.20.10.3/");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.clearDisplay();

  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text

  display.setTextSize(2);
  display.setCursor(10, 0);
  display.println(City);

  display.setTextSize(1);
  display.setCursor(45, 20);
  display.print(main_temp);
  display.write(248);
  display.println("c");

  display.setCursor(15, 30);
  display.println(Description);

  display.setCursor(0, 45);
  display.println("Pressure");

  display.setCursor(0, 54);
  display.print(main_pressure, DEC);
  display.println(" hPa");

  display.setCursor(70, 45);
  display.print("Humidity");

  display.setCursor(90, 54);
  display.print(main_humidity, DEC);
  display.print(" ");
  display.write(37);

  display.display();
  delay(2000);
}

/* 
Definitions for the Private Functions Used
*/

String SendHTML(float temperature, float humidity, float pressure, float visibility)
{
  String ptr = "<!DOCTYPE html>";
  ptr += "<html>";
  ptr += "<head>";
  ptr += "<title>ESP8266 Weather Station</title>";
  ptr += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr += "<link href='https://fonts.googleapis.com/css?family=Open+Sans:300,400,600' rel='stylesheet'>";
  ptr += "<style>";
  ptr += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444;}";
  ptr += "body{margin: 0px;} ";
  ptr += "h1 {margin: 50px auto 30px;} ";
  ptr += ".side-by-side{display: table-cell;vertical-align: middle;position: relative;}";
  ptr += ".text{font-weight: 600;font-size: 19px;width: 200px;}";
  ptr += ".reading{font-weight: 300;font-size: 50px;padding-right: 25px;}";
  ptr += ".temperature .reading{color: #F29C1F;}";
  ptr += ".humidity .reading{color: #3B97D3;}";
  ptr += ".pressure .reading{color: #26B99A;}";
  ptr += ".altitude .reading{color: #955BA5;}";
  ptr += ".superscript{font-size: 17px;font-weight: 600;position: absolute;top: 10px;}";
  ptr += ".data{padding: 10px;}";
  ptr += ".container{display: table;margin: 0 auto;}";
  ptr += ".icon{width:65px}";
  ptr += "</style>";
  ptr += "</head>";
  ptr += "<body>";
  ptr += "<h1>ESP8266 Weather Station</h1>";
  ptr += "<div class='container'>";
  ptr += "<div class='data temperature'>";
  ptr += "<div class='side-by-side icon'>";
  ptr += "<svg enable-background='new 0 0 19.438 54.003'height=54.003px id=Layer_1 version=1.1 viewBox='0 0 19.438 54.003'width=19.438px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M11.976,8.82v-2h4.084V6.063C16.06,2.715,13.345,0,9.996,0H9.313C5.965,0,3.252,2.715,3.252,6.063v30.982";
  ptr += "C1.261,38.825,0,41.403,0,44.286c0,5.367,4.351,9.718,9.719,9.718c5.368,0,9.719-4.351,9.719-9.718";
  ptr += "c0-2.943-1.312-5.574-3.378-7.355V18.436h-3.914v-2h3.914v-2.808h-4.084v-2h4.084V8.82H11.976z M15.302,44.833";
  ptr += "c0,3.083-2.5,5.583-5.583,5.583s-5.583-2.5-5.583-5.583c0-2.279,1.368-4.236,3.326-5.104V24.257C7.462,23.01,8.472,22,9.719,22";
  ptr += "s2.257,1.01,2.257,2.257V39.73C13.934,40.597,15.302,42.554,15.302,44.833z'fill=#F29C21 /></g></svg>";
  ptr += "</div>";
  ptr += "<div class='side-by-side text'>Temperature</div>";
  ptr += "<div class='side-by-side reading'>";
  ptr += (float)temperature;
  ptr += "<span class='superscript'>&deg;C</span></div>";
  ptr += "</div>";
  ptr += "<div class='data humidity'>";
  ptr += "<div class='side-by-side icon'>";
  ptr += "<svg enable-background='new 0 0 29.235 40.64'height=40.64px id=Layer_1 version=1.1 viewBox='0 0 29.235 40.64'width=29.235px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><path d='M14.618,0C14.618,0,0,17.95,0,26.022C0,34.096,6.544,40.64,14.618,40.64s14.617-6.544,14.617-14.617";
  ptr += "C29.235,17.95,14.618,0,14.618,0z M13.667,37.135c-5.604,0-10.162-4.56-10.162-10.162c0-0.787,0.638-1.426,1.426-1.426";
  ptr += "c0.787,0,1.425,0.639,1.425,1.426c0,4.031,3.28,7.312,7.311,7.312c0.787,0,1.425,0.638,1.425,1.425";
  ptr += "C15.093,36.497,14.455,37.135,13.667,37.135z'fill=#3C97D3 /></svg>";
  ptr += "</div>";
  ptr += "<div class='side-by-side text'>Humidity</div>";
  ptr += "<div class='side-by-side reading'>";
  ptr += (int)humidity;
  ptr += "<span class='superscript'>%</span></div>";
  ptr += "</div>";
  ptr += "<div class='data pressure'>";
  ptr += "<div class='side-by-side icon'>";
  ptr += "<svg enable-background='new 0 0 40.542 40.541'height=40.541px id=Layer_1 version=1.1 viewBox='0 0 40.542 40.541'width=40.542px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M34.313,20.271c0-0.552,0.447-1,1-1h5.178c-0.236-4.841-2.163-9.228-5.214-12.593l-3.425,3.424";
  ptr += "c-0.195,0.195-0.451,0.293-0.707,0.293s-0.512-0.098-0.707-0.293c-0.391-0.391-0.391-1.023,0-1.414l3.425-3.424";
  ptr += "c-3.375-3.059-7.776-4.987-12.634-5.215c0.015,0.067,0.041,0.13,0.041,0.202v4.687c0,0.552-0.447,1-1,1s-1-0.448-1-1V0.25";
  ptr += "c0-0.071,0.026-0.134,0.041-0.202C14.39,0.279,9.936,2.256,6.544,5.385l3.576,3.577c0.391,0.391,0.391,1.024,0,1.414";
  ptr += "c-0.195,0.195-0.451,0.293-0.707,0.293s-0.512-0.098-0.707-0.293L5.142,6.812c-2.98,3.348-4.858,7.682-5.092,12.459h4.804";
  ptr += "c0.552,0,1,0.448,1,1s-0.448,1-1,1H0.05c0.525,10.728,9.362,19.271,20.22,19.271c10.857,0,19.696-8.543,20.22-19.271h-5.178";
  ptr += "C34.76,21.271,34.313,20.823,34.313,20.271z M23.084,22.037c-0.559,1.561-2.274,2.372-3.833,1.814";
  ptr += "c-1.561-0.557-2.373-2.272-1.815-3.833c0.372-1.041,1.263-1.737,2.277-1.928L25.2,7.202L22.497,19.05";
  ptr += "C23.196,19.843,23.464,20.973,23.084,22.037z'fill=#26B999 /></g></svg>";
  ptr += "</div>";
  ptr += "<div class='side-by-side text'>Pressure</div>";
  ptr += "<div class='side-by-side reading'>";
  ptr += (int)pressure;
  ptr += "<span class='superscript'>hPa</span></div>";
  ptr += "</div>";
  ptr += "<div class='data altitude'>";
  ptr += "<div class='side-by-side icon'>";
  ptr += "<svg enable-background='new 0 0 58.422 40.639'height=40.639px id=Layer_1 version=1.1 viewBox='0 0 58.422 40.639'width=58.422px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M58.203,37.754l0.007-0.004L42.09,9.935l-0.001,0.001c-0.356-0.543-0.969-0.902-1.667-0.902";
  ptr += "c-0.655,0-1.231,0.32-1.595,0.808l-0.011-0.007l-0.039,0.067c-0.021,0.03-0.035,0.063-0.054,0.094L22.78,37.692l0.008,0.004";
  ptr += "c-0.149,0.28-0.242,0.594-0.242,0.934c0,1.102,0.894,1.995,1.994,1.995v0.015h31.888c1.101,0,1.994-0.893,1.994-1.994";
  ptr += "C58.422,38.323,58.339,38.024,58.203,37.754z'fill=#955BA5 /><path d='M19.704,38.674l-0.013-0.004l13.544-23.522L25.13,1.156l-0.002,0.001C24.671,0.459,23.885,0,22.985,0";
  ptr += "c-0.84,0-1.582,0.41-2.051,1.038l-0.016-0.01L20.87,1.114c-0.025,0.039-0.046,0.082-0.068,0.124L0.299,36.851l0.013,0.004";
  ptr += "C0.117,37.215,0,37.62,0,38.059c0,1.412,1.147,2.565,2.565,2.565v0.015h16.989c-0.091-0.256-0.149-0.526-0.149-0.813";
  ptr += "C19.405,39.407,19.518,39.019,19.704,38.674z'fill=#955BA5 /></g></svg>";
  ptr += "</div>";
  ptr += "<div class='side-by-side text'>Visibility</div>";
  ptr += "<div class='side-by-side reading'>";
  ptr += (int)(visibility / 1000);
  ptr += "<span class='superscript'>Km</span></div>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</body>";
  ptr += "</html>";
  return ptr;
}

void handle_OnConnect()
{
  server.send(200, "text/html", SendHTML(main_temp, main_humidity, main_pressure, visibility));
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}