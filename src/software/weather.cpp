#include "software/weather.hpp"
#include <ArduinoJson.h>

void ConvertWeatherStringToWeatherEnum(const char *weatherString, WeatherState &weatherState)
{
    if (strcmp(weatherString, "Clouds") == 0)
        weatherState = CLOUDY;
    else if (strcmp(weatherString, "Rain") == 0)
        weatherState = RAINY;
    else if (strcmp(weatherString, "Snow") == 0)
        weatherState = SNOWY;
    else
        weatherState = SUNNY;
};

void convertOpenWeatherToWeatherInformationStruct(char* response, WeatherInformation &weatherInformation)
{
    // Create filter to extract just what we require from the JSON response:
    StaticJsonDocument<256 /* Bytes */> filter;

    JsonObject filter_list_0 = filter["list"].createNestedObject();
    filter_list_0["dt"] = true;

    JsonObject filter_list_0_main = filter_list_0.createNestedObject("main");
    filter_list_0_main["temp"] = true;
    filter_list_0_main["humidity"] = true;
    filter_list_0["weather"][0]["main"] = true;
    filter_list_0["wind"]["speed"] = true;
    filter["city"]["name"] = true;

    // Step 1: Convert the response to a JSON object
    StaticJsonDocument<1024 * 10 /* 10KB */> jsonDocument;
    DeserializationError error = deserializeJson(jsonDocument, response, strlen(response), DeserializationOption::Filter(filter));
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
    }
    else {
        Serial.println("JSON successfully parsed.");
        WeatherForecast& firstForecast = weatherInformation.firstForecast;
        WeatherForecast& secondForecast = weatherInformation.secondForecast;
        WeatherForecast& thirdForecast = weatherInformation.thirdForecast;

        // Step 2: Extract the data from the JSON object
        weatherInformation.city = (char *)calloc(strlen(jsonDocument["city"]["name"])+1, sizeof(char));
        strcpy(weatherInformation.city, jsonDocument["city"]["name"]);  // DevSkim: ignore DS185832

        firstForecast.epochTime = jsonDocument["list"][0]["dt"];
        firstForecast.temperature = jsonDocument["list"][0]["main"]["temp"];
        firstForecast.humidity = jsonDocument["list"][0]["main"]["humidity"];
        Serial.println("#1 -> Temperature: " + String(firstForecast.temperature));
        Serial.println("#1 -> Humidity: " + String(firstForecast.humidity));
        
        secondForecast.epochTime = jsonDocument["list"][1]["dt"];
        secondForecast.temperature = jsonDocument["list"][1]["main"]["temp"];
        secondForecast.humidity = jsonDocument["list"][1]["main"]["humidity"];
        Serial.println("#2 -> Temperature: " + String(secondForecast.temperature));
        Serial.println("#2 -> Humidity: " + String(secondForecast.humidity));

        thirdForecast.epochTime = jsonDocument["list"][2]["dt"];
        thirdForecast.temperature = jsonDocument["list"][2]["main"]["temp"];
        thirdForecast.humidity = jsonDocument["list"][2]["main"]["humidity"];
        Serial.println("#3 -> Temperature: " + String(thirdForecast.temperature));
        Serial.println("#3 -> Humidity: " + String(thirdForecast.humidity));

        Serial.println("Weather information successfully converted.");
    }
}
