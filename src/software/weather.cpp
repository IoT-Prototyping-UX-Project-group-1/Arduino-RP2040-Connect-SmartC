#include "software/weather.hpp"
#include <ArduinoJson.h>

WeatherState ConvertWeatherStringToWeatherStateEnum(float cloudsAll, char *weatherMain, char *weatherDescription)
{
    if (strcmp(weatherMain, "Clouds") == 0)
    {
        if (strcmp(weatherDescription, "few clouds") == 0 || strcmp(weatherDescription, "scattered clouds") == 0)
        {
            return WeatherState::FEW_CLOUDS;
        }
        else
        {
            return WeatherState::CLOUDY;
        }
    }
    else if (strcmp(weatherMain, "Rain") == 0)
    {
        if (strcmp(weatherDescription, "light rain") == 0)
        {
            return WeatherState::LIGHT_RAIN;
        }
        else if (strcmp(weatherDescription, "moderate rain") == 0)
        {
            return WeatherState::RAINY;
        }
        else
        {
            return WeatherState::HEAVY_RAIN;
        }
    }
    else if (strcmp(weatherMain, "Mist") == 0)
    {
        return WeatherState::MISTY;
    }
    else if (strcmp(weatherMain, "Clear") == 0 || cloudsAll < 10 || strcmp(weatherDescription, "clear sky") == 0)
    {
        return WeatherState::CLEAR_SKY;
    }
    return WeatherState::CLEAR_SKY;
}

void convertOpenWeatherToWeatherInformationStruct(char *response, WeatherInformation &weatherInformation)
{
    // Create filter to extract just what we require from the JSON response:
    StaticJsonDocument<256 /* Bytes */> filter;

    JsonObject filter_list_0 = filter["list"].createNestedObject();
    filter_list_0["dt"] = true;

    JsonObject filter_list_0_main = filter_list_0.createNestedObject("main");
    filter_list_0_main["feels_like"] = true;
    filter_list_0["weather"][0]["main"] = true;
    filter_list_0["weather"][0]["description"] = true;
    filter_list_0["clouds"]["all"] = true;
    filter["city"]["name"] = true;

    // Step 1: Convert the response to a JSON object
    StaticJsonDocument<1024 * 16 /* 10KB */> jsonDocument;
    DeserializationError error = deserializeJson(jsonDocument, response, strlen(response), DeserializationOption::Filter(filter));
    response = "";
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
    }
    else
    {
        Serial.println("JSON successfully parsed.");
        WeatherForecast &firstForecast = weatherInformation.firstForecast;
        WeatherForecast &secondForecast = weatherInformation.secondForecast;
        WeatherForecast &thirdForecast = weatherInformation.thirdForecast;

        // Step 2: Extract the data from the JSON object
        weatherInformation.city = (char *)calloc(strlen(jsonDocument["city"]["name"]) + 1, sizeof(char));
        strcpy(weatherInformation.city, jsonDocument["city"]["name"]); // DevSkim: ignore DS185832

        // Step 3: Extract the data from the first forecast block(3hours), index 0
        firstForecast.epochTime = jsonDocument["list"][0]["dt"];
        float feelsLike1 = jsonDocument["list"][0]["main"]["feels_like"];
        float cloudsAll1 = jsonDocument["list"][0]["clouds"]["all"];
        char *weatherMain1 = (char *)calloc(strlen(jsonDocument["list"][0]["weather"][0]["main"]) + 1, sizeof(char));
        strcpy(weatherMain1, jsonDocument["list"][0]["weather"][0]["main"]);
        char *weatherDescription1 = (char *)calloc(strlen(jsonDocument["list"][0]["weather"][0]["description"]) + 1, sizeof(char));
        strcpy(weatherDescription1, jsonDocument["list"][0]["weather"][0]["description"]);

        Serial.println("#1 -> Temperature feels_like: " + String(feelsLike1));
        Serial.println("#1 -> Weather Main: " + String(weatherMain1));
        Serial.println("#1 -> Weather Description: " + String(weatherDescription1));

        firstForecast.temperatureState = feelsLike1 < 1 ? FROST : feelsLike1 < 10 ? TemperatureState::COLD
                                                              : feelsLike1 < 17   ? TemperatureState::MODERATE
                                                              : feelsLike1 < 27   ? TemperatureState::WARM
                                                                                  : TemperatureState::HOT;

        firstForecast.weatherState = ConvertWeatherStringToWeatherStateEnum(cloudsAll1, weatherMain1, weatherDescription1);

        Serial.println("#1 -> Temperature State: " + String(firstForecast.temperatureState));
        Serial.println("#1 -> Weather State: " + String(firstForecast.weatherState));

        // Step 4: Extract the data from the second forecast block(3hours), index 1
        secondForecast.epochTime = jsonDocument["list"][1]["dt"];
        float feelsLike2 = jsonDocument["list"][1]["main"]["feels_like"];
        float cloudsAll2 = jsonDocument["list"][1]["clouds"]["all"];
        char *weatherMain2 = (char *)calloc(strlen(jsonDocument["list"][1]["weather"][0]["main"]) + 1, sizeof(char));
        strcpy(weatherMain2, jsonDocument["list"][1]["weather"][0]["main"]);
        char *weatherDescription2 = (char *)calloc(strlen(jsonDocument["list"][1]["weather"][0]["description"]) + 1, sizeof(char));
        strcpy(weatherDescription2, jsonDocument["list"][1]["weather"][0]["description"]);

        Serial.println("#2 -> Temperature feels_like: " + String(feelsLike2));
        Serial.println("#2 -> Weather Main: " + String(weatherMain2));
        Serial.println("#2 -> Weather Description: " + String(weatherDescription2));

        secondForecast.temperatureState = feelsLike2 < 1 ? FROST : feelsLike2 < 10 ? TemperatureState::COLD
                                                               : feelsLike2 < 17   ? TemperatureState::MODERATE
                                                               : feelsLike2 < 27   ? TemperatureState::WARM
                                                                                   : TemperatureState::HOT;

        secondForecast.weatherState = ConvertWeatherStringToWeatherStateEnum(cloudsAll2, weatherMain2, weatherDescription2);

        Serial.println("#2 -> Temperature State: " + String(secondForecast.temperatureState));
        Serial.println("#2 -> Weather State: " + String(secondForecast.weatherState));

        Serial.println("Weather information successfully converted.");
    }
}

void displayWeatherInfoOnLEDRingStripe()
{
}