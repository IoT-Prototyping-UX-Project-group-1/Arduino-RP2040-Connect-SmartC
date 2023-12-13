#include <ArduinoJson.h>
#include "software/weather.hpp"

void ConvertWeatherStringToWeatherEnum(const char* weatherString, WeatherState& weatherState) {
    if (strcmp(weatherString, "Clouds") == 0) weatherState = CLOUDY;
    else if (strcmp(weatherString, "Rain") == 0) weatherState = RAINY;
    else if (strcmp(weatherString, "Snow") == 0) weatherState = SNOWY;
    else weatherState = SUNNY;
};


bool convertOpenWeatherToWeatherInformationStruct(const char* response, WeatherInformation& weatherInformation) {

    // Step 1: Convert the response to a JSON object
    StaticJsonDocument<1024* 16 /* 16KB */> jsonDocument;
    DeserializationError error = deserializeJson(jsonDocument, response);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return false;
    }

    // Step 2: Free the memory allocated for the previous weather information
    if (weatherInformation.city != NULL)                free(weatherInformation.city);
    if (weatherInformation.firstForecast != NULL)        free(weatherInformation.firstForecast);
    if (weatherInformation.secondForecast != NULL)      free(weatherInformation.secondForecast);
    if (weatherInformation.thirdForecast != NULL)       free(weatherInformation.thirdForecast);

    // Step 3: Allocate memory for the new weather information
    weatherInformation.city = (char*)calloc(strlen(jsonDocument["city"]["name"]) + 1, sizeof(char));
    weatherInformation.firstForecast = (WeatherForecast*)calloc(1, sizeof(WeatherForecast));
    weatherInformation.secondForecast = (WeatherForecast*)calloc(1, sizeof(WeatherForecast));
    weatherInformation.thirdForecast = (WeatherForecast*)calloc(1, sizeof(WeatherForecast));

    // Step 4: Copy the city name
    strcpy(weatherInformation.city, jsonDocument["city"]["name"]); // DevSkim: ignore DS185832

    // Step 5a: Copy the first forecast
    weatherInformation.firstForecast->epochTime = jsonDocument["list"][0]["dt"];
    weatherInformation.firstForecast->temperature = jsonDocument["list"][0]["main"]["temp"];
    weatherInformation.firstForecast->humidity = jsonDocument["list"][0]["main"]["humidity"];
    weatherInformation.firstForecast->windSpeed = jsonDocument["list"][0]["wind"]["speed"];
    const char* weatherType = jsonDocument["list"][0]["weather"][0]["main"];
    ConvertWeatherStringToWeatherEnum(weatherType, weatherInformation.firstForecast->weatherState);

    // Step 5b: Copy the second forecast
    weatherInformation.secondForecast->epochTime = jsonDocument["list"][1]["dt"];
    weatherInformation.secondForecast->temperature = jsonDocument["list"][1]["main"]["temp"];
    weatherInformation.secondForecast->humidity = jsonDocument["list"][1]["main"]["humidity"];
    weatherInformation.secondForecast->windSpeed = jsonDocument["list"][1]["wind"]["speed"];
    weatherType = jsonDocument["list"][1]["weather"][0]["main"];
    ConvertWeatherStringToWeatherEnum(weatherType, weatherInformation.secondForecast->weatherState);

    // Step 5c: Copy the third forecast
    weatherInformation.thirdForecast->epochTime = jsonDocument["list"][2]["dt"];
    weatherInformation.thirdForecast->temperature = jsonDocument["list"][2]["main"]["temp"];
    weatherInformation.thirdForecast->humidity = jsonDocument["list"][2]["main"]["humidity"];
    weatherInformation.thirdForecast->windSpeed = jsonDocument["list"][2]["wind"]["speed"];
    weatherType = jsonDocument["list"][2]["weather"][0]["main"];
    ConvertWeatherStringToWeatherEnum(weatherType, weatherInformation.thirdForecast->weatherState);

    return true;
}





