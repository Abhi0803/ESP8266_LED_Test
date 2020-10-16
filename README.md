# ESP8266_LED_Test

A simple Application for ESP8266-12E Node MCU to get Weather from OpenWeather API And Show it to the SSD1306 (OLED 128x64).

NOTE: Consider Doing These Changes First in the ESP_Weather_OLED_main.cpp File.

Change the Values Of These MACROS before Building the Project.
    - CITY      (Name of the City You Want Weather Data of)
    - API_KEY   (API key of the OpenWeather API Or You can Add Your Own API)
    - SSID      (SSID Of the Router Or The Wifi Name of Your Hotspot)
    - PASS      (Password of the Wifi Router Or Hotspot)


Search For @Abhi0803_DataStructure in the File And Do these Changes.
    - You Will Find that the Strucutre is made specifically for OpenWeather API Only.
    - You can Chnage It On Your Own but if you want Assistance Go to This Link and Paste the Ouput
      of your API Once to Generate the Structure 
    - Also Change the Buffer Size in the "capacity" Variable.
    Link - https://arduinojson.org/v6/assistant/